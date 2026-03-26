#ifndef __W25Q64_H
#define __W25Q64_H

#include "main.h" // 包含HAL库定义

/* W25Q64 存储参数定义 */
#define W25Q64_PAGE_SIZE       256
#define W25Q64_SECTOR_SIZE     4096
#define W25Q64_BLOCK_SIZE      65536
#define W25Q64_PAGE_COUNT      32768
#define W25Q64_SECTOR_COUNT    2048
#define W25Q64_BLOCK_COUNT     128
#define W25Q64_CAPACITY        (8 * 1024 * 1024) // 8MB

/* 指令定义 */
#define W25X_WriteEnable       0x06
#define W25X_WriteDisable      0x04
#define W25X_ReadStatusReg1    0x05
#define W25X_ReadStatusReg2    0x35
#define W25X_WriteStatusReg    0x01
#define W25X_ReadData          0x03
#define W25X_FastReadData      0x0B
#define W25X_FastReadDual      0x3B
#define W25X_PageProgram       0x02
#define W25X_BlockErase        0xD8
#define W25X_SectorErase       0x20
#define W25X_ChipErase         0xC7
#define W25X_PowerDown         0xB9
#define W25X_ReleasePowerDown  0xAB
#define W25X_DeviceID          0xAB
#define W25X_ManufactDeviceID  0x90
#define W25X_JedecDeviceID     0x9F

/* CS引脚定义 (根据你的要求使用PA4) */
#define W25Q64_CS_GPIO_Port    GPIOA
#define W25Q64_CS_Pin          GPIO_PIN_4

/* 状态 */
#define W25Q64_OK              0
#define W25Q64_ERROR           1
#define W25Q64_TIMEOUT         2

/* 函数声明 */
uint32_t W25Q64_ReadID(void);
uint8_t  W25Q64_Erase_Sector(uint32_t Address);
uint8_t  W25Q64_Erase_Block(uint32_t Address);
uint8_t  W25Q64_Erase_Chip(void);

void     W25Q64_Read_Data(uint32_t ReadAddr, uint8_t *pBuffer, uint32_t NumByteToRead);
void     W25Q64_Write_Data(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);

#endif

