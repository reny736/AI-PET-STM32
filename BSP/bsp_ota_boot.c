#include "bsp_ota_boot.h"

#include "bsp_uart.h"
#include "cmsis_os.h"
#include "tim.h"
#include "usart.h"

#define OTA_BOOT_REQUEST_SIZE        4U
#define OTA_BOOT_REQUEST_QUIET_MS  1000U
#define OTA_BOOT_ACK_BYTE          0x79U
#define OTA_SYSTEM_MEMORY_BASE     0x1FFFF000U

static uint32_t ota_uart_quiet_until_tick = 0;
static uint8_t ota_boot_request_index = 0;

static void BSP_OTA_UART_JumpToSystemBootloader(void);

void BSP_OTA_UART_Reset(void)
{
    ota_uart_quiet_until_tick = 0;
    ota_boot_request_index = 0;
}

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

uint8_t BSP_OTA_UART_IsQuietWindowActive(void)
{
    return ((int32_t)(HAL_GetTick() - ota_uart_quiet_until_tick) < 0) ? 1U : 0U;
}

void BSP_OTA_UART_EnterBootloader(void)
{
    uint8_t ack = OTA_BOOT_ACK_BYTE;

    ota_boot_request_index = 0;
    ota_uart_quiet_until_tick = HAL_GetTick() + OTA_BOOT_REQUEST_QUIET_MS;

    BSP_UART_Send(&ack, 1);
    osDelay(20);

    BSP_OTA_UART_JumpToSystemBootloader();
}

static void BSP_OTA_UART_JumpToSystemBootloader(void)
{
    typedef void (*BootJumpFunc_t)(void);

    const uint32_t stack_pointer = *(__IO uint32_t *)OTA_SYSTEM_MEMORY_BASE;
    const uint32_t reset_handler = *(__IO uint32_t *)(OTA_SYSTEM_MEMORY_BASE + 4U);
    BootJumpFunc_t boot_jump = (BootJumpFunc_t)reset_handler;

    HAL_UART_AbortReceive_IT(&huart1);
    HAL_UART_DeInit(&huart1);

    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);

    HAL_RCC_DeInit();
    HAL_DeInit();

    __disable_irq();

    SysTick->CTRL = 0U;
    SysTick->LOAD = 0U;
    SysTick->VAL = 0U;

    for (uint32_t i = 0; i < 8U; ++i) {
        NVIC->ICER[i] = 0xFFFFFFFFU;
        NVIC->ICPR[i] = 0xFFFFFFFFU;
    }

    SCB->VTOR = OTA_SYSTEM_MEMORY_BASE;
    __set_MSP(stack_pointer);
    __DSB();
    __ISB();
    __enable_irq();

    boot_jump();

    while (1) {
    }
}
