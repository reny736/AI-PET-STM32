/**
  * @file    bsp_ota_boot.h
  * @brief   OTA bootloader UART通信头文件
  * @details 该文件包含了通过串口触发进入系统bootloader进行固件升级的相关定义和函数声明
  * @author  HAL Team
  */
#ifndef __BSP_OTA_BOOT_H
#define __BSP_OTA_BOOT_H

#include "main.h"

/**
  * @brief  OTA UART状态枚举
  * @note   用于跟踪OTA bootloader匹配协议的状态
  */
typedef enum {
    BSP_OTA_UART_IDLE = 0,      // 空闲状态，等待接收数据
    BSP_OTA_UART_MATCHING,      // 正在匹配接收序列
    BSP_OTA_UART_MATCHED        // 序列匹配成功，准备进入bootloader
} BSP_OtaUartState_t;

/* 函数声明 */

/**
  * @brief  重置OTA UART状态
  * @param  None
  * @retval None
  * @note   清空静默窗口计时器和匹配索引，准备重新开始匹配
  */
void BSP_OTA_UART_Reset(void);

/**
  * @brief  处理接收到的字节数据
  * @param  rx_byte: 接收到的字节数据
  * @retval OTA UART状态
  * @note   使用特定序列(0x5A, 0xA5, 0x5A, 0xA5)进行匹配
  *         成功匹配后返回BSP_OTA_UART_MATCHED
  */
BSP_OtaUartState_t BSP_OTA_UART_HandleByte(uint8_t rx_byte);

/**
  * @brief  检查静默窗口是否激活
  * @param  None
  * @retval 1-静默窗口激活，0-静默窗口未激活
  * @note   在静默窗口期间(接收到特定字节后的1000ms内)，不处理其他数据
  */
uint8_t BSP_OTA_UART_IsQuietWindowActive(void);

/**
  * @brief  进入系统bootloader模式
  * @param  None
  * @retval None
  * @note   发送ACK响应后，禁用所有外设，配置中断向量表，
  *         然后跳转到系统bootloader (地址: 0x1FFFF000)
  */
void BSP_OTA_UART_EnterBootloader(void);

#endif /* __BSP_OTA_BOOT_H */
