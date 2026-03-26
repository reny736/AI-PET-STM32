#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "main.h"
#include "usart.h"

/* 串口缓冲区大小定义 */
#define UART_RX_BUFFER_SIZE  256
#define UART_TX_BUFFER_SIZE  256

/* 串口状态枚举 */
typedef enum {
    UART_OK = 0,
    UART_ERROR,
    UART_BUSY,
    UART_TIMEOUT
} UART_Status_t;

/* 串口数据结构 */
typedef struct {
    uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
    uint16_t rx_index;
    uint16_t rx_length;
    uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
    uint16_t tx_index;
    uint16_t tx_length;
    uint8_t rx_complete;
    uint8_t tx_complete;
} UART_Buffer_t;

/* 函数声明 */
void BSP_UART_Init(void);
UART_Status_t BSP_UART_Send(uint8_t *data, uint16_t length);
UART_Status_t BSP_UART_Receive(uint8_t *buffer, uint16_t length, uint32_t timeout);
uint16_t BSP_UART_GetRxLength(void);
void BSP_UART_ClearRxBuffer(void);
void BSP_UART_SendString(char *str);
void BSP_UART_Printf(const char *fmt, ...);

/* 中断回调函数 */
void BSP_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void BSP_UART_TxCpltCallback(UART_HandleTypeDef *huart);

#endif /* __BSP_UART_H */


