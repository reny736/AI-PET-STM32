#ifndef __BSP_OTA_BOOT_H
#define __BSP_OTA_BOOT_H

#include "main.h"

typedef enum {
    BSP_OTA_UART_IDLE = 0,
    BSP_OTA_UART_MATCHING,
    BSP_OTA_UART_MATCHED
} BSP_OtaUartState_t;

void BSP_OTA_UART_Reset(void);
BSP_OtaUartState_t BSP_OTA_UART_HandleByte(uint8_t rx_byte);
uint8_t BSP_OTA_UART_IsQuietWindowActive(void);
void BSP_OTA_UART_EnterBootloader(void);

#endif /* __BSP_OTA_BOOT_H */
