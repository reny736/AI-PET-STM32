#include "bsp_uart.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/* 全局变量定义 */
static UART_Buffer_t uart_buffer = {0};  // 串口缓冲区结构体，用于管理接收和发送数据

/**
  * @brief  串口初始化
  * @param  None
  * @retval None
  */
void BSP_UART_Init(void)
{
    /* 清空缓冲区 */
    memset(&uart_buffer, 0, sizeof(UART_Buffer_t));
    
    /* 注意：在UART任务中启动中断接收，这里不启动 */
    /* HAL_UART_Receive_IT(&huart1, uart_buffer.rx_buffer, 1); */
}

/**
  * @brief  发送数据
  * @param  data: 要发送的数据指针
  * @param  length: 数据长度
  * @retval 发送状态
  */
UART_Status_t BSP_UART_Send(uint8_t *data, uint16_t length)
{
    if (length == 0 || data == NULL) {
        return UART_ERROR;
    }
    
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, data, length, 1000);
    
    switch (status) {
        case HAL_OK:
            return UART_OK;
        case HAL_BUSY:
            return UART_BUSY;
        case HAL_TIMEOUT:
            return UART_TIMEOUT;
        default:
            return UART_ERROR;
    }
}

/**
  * @brief  接收数据
  * @param  buffer: 接收缓冲区指针
  * @param  length: 期望接收的数据长度
  * @param  timeout: 超时时间(ms)
  * @retval 接收状态
  */
UART_Status_t BSP_UART_Receive(uint8_t *buffer, uint16_t length, uint32_t timeout)
{
    if (length == 0 || buffer == NULL) {
        return UART_ERROR;
    }
    
    HAL_StatusTypeDef status = HAL_UART_Receive(&huart1, buffer, length, timeout);
    
    switch (status) {
        case HAL_OK:
            return UART_OK;
        case HAL_BUSY:
            return UART_BUSY;
        case HAL_TIMEOUT:
            return UART_TIMEOUT;
        default:
            return UART_ERROR;
    }
}

/**
  * @brief  获取接收缓冲区数据长度
  * @param  None
  * @retval 接收到的数据长度
  */
uint16_t BSP_UART_GetRxLength(void)
{
    return uart_buffer.rx_length;
}

/**
  * @brief  清空接收缓冲区
  * @param  None
  * @retval None
  */
void BSP_UART_ClearRxBuffer(void)
{
    memset(uart_buffer.rx_buffer, 0, UART_RX_BUFFER_SIZE);
    uart_buffer.rx_index = 0;
    uart_buffer.rx_length = 0;
}

/**
  * @brief  发送字符串
  * @param  str: 要发送的字符串
  * @retval None
  */
void BSP_UART_SendString(char *str)
{
    if (str != NULL) {
        BSP_UART_Send((uint8_t*)str, strlen(str));
    }
}

/**
  * @brief  格式化输出(类似printf)
  * @param  fmt: 格式化字符串
  * @retval None
  */
void BSP_UART_Printf(const char *fmt, ...)
{
    char buffer[UART_TX_BUFFER_SIZE];
    va_list args;
    
    va_start(args, fmt);
    vsnprintf(buffer, UART_TX_BUFFER_SIZE, fmt, args);
    va_end(args);
    
    BSP_UART_SendString(buffer);
}

/* 注意：中断回调函数已移到freertos.c中，确保能正确访问uart_rx_complete变量 */
/* void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        extern volatile uint8_t uart_rx_complete;
        uart_rx_complete = 1;
    }
} */

/**
  * @brief  串口发送完成中断回调函数
  * @param  huart: 串口句柄
  * @retval None
  */
void BSP_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        uart_buffer.tx_complete = 1;
    }
}