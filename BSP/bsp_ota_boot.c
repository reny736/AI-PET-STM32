/**
  * @file    bsp_ota_boot.c
  * @brief   OTA bootloader UART通信实现
  * @details 该文件实现了通过串口接收特定序列(0x5A, 0xA5, 0x5A, 0xA5)来触发
  *          进入系统bootloader进行固件升级的功能。成功匹配后，会禁用所有外设
  *          并跳转到STM32系统bootloader (地址: 0x1FFFF000)。
  * @author  HAL Team
  */
#include "bsp_ota_boot.h"

#include "bsp_uart.h"
#include "cmsis_os.h"
#include "tim.h"
#include "usart.h"

/* OTA bootloader常量定义 */
#define OTA_BOOT_REQUEST_SIZE        4U    // 触发序列长度
#define OTA_BOOT_REQUEST_QUIET_MS  1000U    // 静默窗口时间(1000ms)
#define OTA_BOOT_ACK_BYTE          0x79U    // ACK应答字节
#define OTA_SYSTEM_MEMORY_BASE     0x1FFFF000U  // STM32系统bootloader地址

/* 静态变量定义 */
static uint32_t ota_uart_quiet_until_tick = 0;  // 静默窗口结束时间戳
static uint8_t ota_boot_request_index = 0;       // 当前匹配的序列索引

/* 静态函数声明 */
static void BSP_OTA_UART_JumpToSystemBootloader(void);

/**
  * @brief  重置OTA UART状态
  * @param  None
  * @retval None
  * @note   清空静默窗口计时器和匹配索引，用于重新开始匹配序列
  */
void BSP_OTA_UART_Reset(void)
{
    ota_uart_quiet_until_tick = 0;
    ota_boot_request_index = 0;
}

/**
  * @brief  处理接收到的字节数据
  * @param  rx_byte: 接收到的字节数据
  * @retval OTA UART状态 (IDLE/MATCHING/MATCHED)
  * @note   使用特定序列(0x5A, 0xA5, 0x5A, 0xA5)进行匹配
  *         每次接收到匹配字节都会重置静默窗口计时器
  */
BSP_OtaUartState_t BSP_OTA_UART_HandleByte(uint8_t rx_byte)
{
    static const uint8_t kOtaBootRequest[OTA_BOOT_REQUEST_SIZE] = {0x5AU, 0xA5U, 0x5AU, 0xA5U};

    if (rx_byte == kOtaBootRequest[ota_boot_request_index]) {
        ota_boot_request_index++;
        ota_uart_quiet_until_tick = HAL_GetTick() + OTA_BOOT_REQUEST_QUIET_MS;
        if (ota_boot_request_index >= OTA_BOOT_REQUEST_SIZE) {
            ota_boot_request_index = 0;
            return BSP_OTA_UART_MATCHED;
        }
        return BSP_OTA_UART_MATCHING;
    }

    ota_boot_request_index = (rx_byte == kOtaBootRequest[0]) ? 1U : 0U;
    if (ota_boot_request_index != 0U) {
        ota_uart_quiet_until_tick = HAL_GetTick() + OTA_BOOT_REQUEST_QUIET_MS;
        return BSP_OTA_UART_MATCHING;
    }

    return BSP_OTA_UART_IDLE;
}

/**
  * @brief  检查静默窗口是否激活
  * @param  None
  * @retval 1-静默窗口激活，0-静默窗口未激活
  * @note   在静默窗口期间(接收到特定字节后的1000ms内)，不处理其他数据
  *         用于防止误触发和数据干扰
  */
uint8_t BSP_OTA_UART_IsQuietWindowActive(void)
{
    return ((int32_t)(HAL_GetTick() - ota_uart_quiet_until_tick) < 0) ? 1U : 0U;
}

/**
  * @brief  进入系统bootloader模式
  * @param  None
  * @retval None
  * @note   发送ACK响应后调用跳转函数，进入STM32系统bootloader
  *         跳转前会重置静默窗口状态
  */
void BSP_OTA_UART_EnterBootloader(void)
{
    uint8_t ack = OTA_BOOT_ACK_BYTE;

    ota_boot_request_index = 0;
    ota_uart_quiet_until_tick = HAL_GetTick() + OTA_BOOT_REQUEST_QUIET_MS;

    BSP_UART_Send(&ack, 1);
    osDelay(20);

    BSP_OTA_UART_JumpToSystemBootloader();
}

/**
  * @brief  跳转到系统bootloader
  * @param  None
  * @retval None
  * @note   执行完整的bootloader跳转流程：
  *         1. 停止UART和PWM外设
  *         2. 复位RCC和HAL库
  *         3. 禁用所有中断
  *         4. 配置中断向量表到系统bootloader地址
  *         5. 设置MSP并跳转到reset handler
  */
static void BSP_OTA_UART_JumpToSystemBootloader(void)
{
    typedef void (*BootJumpFunc_t)(void);

    const uint32_t stack_pointer = *(__IO uint32_t *)OTA_SYSTEM_MEMORY_BASE;
    const uint32_t reset_handler = *(__IO uint32_t *)(OTA_SYSTEM_MEMORY_BASE + 4U);
    BootJumpFunc_t boot_jump = (BootJumpFunc_t)reset_handler;

    // 停止UART接收
    HAL_UART_AbortReceive_IT(&huart1);
    // 关闭UART外设
    HAL_UART_DeInit(&huart1);

    // 停止所有PWM输出 (舵机控制使用的定时器)
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);

    // 复位RCC和HAL库
    HAL_RCC_DeInit();
    HAL_DeInit();

    // 禁用所有中断
    __disable_irq();

    // 禁用SysTick
    SysTick->CTRL = 0U;
    SysTick->LOAD = 0U;
    SysTick->VAL = 0U;

    // 清除所有中断挂起位
    for (uint32_t i = 0; i < 8U; ++i) {
        NVIC->ICER[i] = 0xFFFFFFFFU;
        NVIC->ICPR[i] = 0xFFFFFFFFU;
    }

    // 配置中断向量表到系统bootloader地址
    SCB->VTOR = OTA_SYSTEM_MEMORY_BASE;
    // 设置主堆栈指针
    __set_MSP(stack_pointer);
    // 内存屏障确保指令顺序执行
    __DSB();
    __ISB();
    // 使能中断
    __enable_irq();

    // 跳转到系统bootloader的reset handler
    boot_jump();

    // 跳转失败时进入死循环
    while (1) {
    }
}
