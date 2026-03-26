/**
  * @file    bsp_uart.h
  * @brief   串口通信BSP层头文件
  * @details 该文件包含了串口缓冲区管理、数据收发以及状态监控的相关定义和函数声明
  * @author  HAL Team
  */
#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "main.h"
#include "usart.h"

/* 串口缓冲区大小定义 */
#define UART_RX_BUFFER_SIZE  256  // 接收缓冲区大小
#define UART_TX_BUFFER_SIZE  256  // 发送缓冲区大小

/* 串口状态枚举定义 */
typedef enum {
    UART_OK = 0,      // 操作成功
    UART_ERROR,       // 操作错误
    UART_BUSY,        // 串口忙
    UART_TIMEOUT      // 操作超时
} UART_Status_t;

/**
  * @brief  串口缓冲区数据结构
  * @note   用于管理串口接收和发送的数据缓冲
  */
typedef struct {
    uint8_t rx_buffer[UART_RX_BUFFER_SIZE];  // 接收数据缓冲区
    uint16_t rx_index;                        // 接收缓冲区当前索引
    uint16_t rx_length;                       // 已接收数据长度
    uint8_t tx_buffer[UART_TX_BUFFER_SIZE];  // 发送数据缓冲区
    uint16_t tx_index;                        // 发送缓冲区当前索引
    uint16_t tx_length;                       // 已发送数据长度
    uint8_t rx_complete;                      // 接收完成标志
    uint8_t tx_complete;                      // 发送完成标志
} UART_Buffer_t;

/* 函数声明 */
/**
  * @brief  串口初始化函数
  * @param  None
  * @retval None
  * @note   初始化串口缓冲区，清零所有状态
  */
void BSP_UART_Init(void);

/**
  * @brief  串口发送数据函数
  * @param  data: 要发送的数据指针
  * @param  length: 数据长度
  * @retval 发送状态 (UART_OK/UART_ERROR/UART_BUSY/UART_TIMEOUT)
  */
UART_Status_t BSP_UART_Send(uint8_t *data, uint16_t length);

/**
  * @brief  串口接收数据函数
  * @param  buffer: 接收缓冲区指针
  * @param  length: 期望接收的数据长度
  * @param  timeout: 超时时间(ms)
  * @retval 接收状态 (UART_OK/UART_ERROR/UART_BUSY/UART_TIMEOUT)
  */
UART_Status_t BSP_UART_Receive(uint8_t *buffer, uint16_t length, uint32_t timeout);

/**
  * @brief  获取接收缓冲区数据长度
  * @param  None
  * @retval 接收到的数据长度
  */
uint16_t BSP_UART_GetRxLength(void);

/**
  * @brief  清空接收缓冲区
  * @param  None
  * @retval None
  */
void BSP_UART_ClearRxBuffer(void);

/**
  * @brief  发送字符串函数
  * @param  str: 要发送的以空字符结尾的字符串
  * @retval None
  */
void BSP_UART_SendString(char *str);

/**
  * @brief  格式化打印函数(类似printf)
  * @param  fmt: 格式化字符串
  * @retval None
  * @note   支持基本的格式化输出，如%d、%s、%x等
  */
void BSP_UART_Printf(const char *fmt, ...);

/* 中断回调函数声明 */
/**
  * @brief  串口接收完成中断回调函数
  * @param  huart: 串口句柄指针
  * @retval None
  * @note   在接收完成中断中调用，设置接收完成标志
  */
void BSP_UART_RxCpltCallback(UART_HandleTypeDef *huart);

/**
  * @brief  串口发送完成中断回调函数
  * @param  huart: 串口句柄指针
  * @retval None
  * @note   在发送完成中断中调用，设置发送完成标志
  */
void BSP_UART_TxCpltCallback(UART_HandleTypeDef *huart);

#endif /* __BSP_UART_H */
