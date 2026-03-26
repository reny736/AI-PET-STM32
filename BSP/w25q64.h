/**
  * @file    w25q64.h
  * @brief   W25Q64 Flash存储器驱动头文件
  * @details 该文件包含了W25Q64系列SPI Flash芯片的常量定义、命令定义和函数声明
  *          W25Q64是一款8Mbit(1MB)的SPI接口Flash芯片
  * @author  HAL Team
  */
#ifndef __W25Q64_H
#define __W25Q64_H

#include "main.h"

/* W25Q64 存储器容量参数定义 */
#define W25Q64_PAGE_SIZE       256     // 页大小 (bytes)
#define W25Q64_SECTOR_SIZE     4096    // 扇区大小 (bytes)
#define W25Q64_BLOCK_SIZE      65536   // 块大小 (bytes)
#define W25Q64_PAGE_COUNT      32768   // 总页数
#define W25Q64_SECTOR_COUNT    2048    // 总扇区数
#define W25Q64_BLOCK_COUNT     128     // 总块数
#define W25Q64_CAPACITY        (8 * 1024 * 1024) // 8Mbit容量

/* W25Q64 命令定义 */
#define W25X_WriteEnable       0x06   // 写使能命令
#define W25X_WriteDisable      0x04   // 写禁止命令
#define W25X_ReadStatusReg1    0x05   // 读取状态寄存器1
#define W25X_ReadStatusReg2    0x35   // 读取状态寄存器2
#define W25X_WriteStatusReg    0x01   // 写入状态寄存器
#define W25X_ReadData          0x03   // 读取数据命令
#define W25X_FastReadData      0x0B   // 快速读取数据命令
#define W25X_FastReadDual      0x3B   // 快速双通道读取命令
#define W25X_PageProgram       0x02   // 页编程命令
#define W25X_BlockErase        0xD8   // 块擦除命令 (64KB)
#define W25X_SectorErase       0x20   // 扇区擦除命令 (4KB)
#define W25X_ChipErase         0xC7   // 整片擦除命令
#define W25X_PowerDown         0xB9   // 掉电命令
#define W25X_ReleasePowerDown  0xAB   // 释放掉电命令
#define W25X_DeviceID          0xAB   // 设备ID命令
#define W25X_ManufactDeviceID  0x90   // 制造商设备ID命令
#define W25X_JedecDeviceID     0x9F   // JEDEC ID命令

/* CS片选引脚定义 */
#define W25Q64_CS_GPIO_Port    GPIOA   // 片选GPIO端口
#define W25Q64_CS_Pin          GPIO_PIN_4  // 片选GPIO引脚

/* 操作状态返回值定义 */
#define W25Q64_OK              0       // 操作成功
#define W25Q64_ERROR           1       // 操作错误
#define W25Q64_TIMEOUT         2       // 操作超时

/* 函数声明 */

/**
  * @brief  读取W25Q64芯片ID
  * @param  None
  * @retval 32位芯片ID (包含Manufacture ID + Memory Type + Capacity)
  * @note   正常返回值: 0xEF4017 (Winbond W25Q64)
  */
uint32_t W25Q64_ReadID(void);

/**
  * @brief  擦除指定扇区 (4KB)
  * @param  Address: 扇区起始地址 (必须为4096的倍数)
  * @retval 操作状态 (W25Q64_OK/W25Q64_ERROR/W25Q64_TIMEOUT)
  * @note   擦除操作会将扇区内所有数据置为0xFF
  */
uint8_t  W25Q64_Erase_Sector(uint32_t Address);

/**
  * @brief  擦除指定块 (64KB)
  * @param  Address: 块起始地址 (必须为65536的倍数)
  * @retval 操作状态 (W25Q64_OK/W25Q64_ERROR/W25Q64_TIMEOUT)
  * @note   块擦除比扇区擦除更快，但会擦除更多数据
  */
uint8_t  W25Q64_Erase_Block(uint32_t Address);

/**
  * @brief  擦除整片Flash
  * @param  None
  * @retval 操作状态 (W25Q64_OK/W25Q64_ERROR/W25Q64_TIMEOUT)
  * @note   整片擦除可能需要较长时间(数十秒)，需等待完成
  */
uint8_t  W25Q64_Erase_Chip(void);

/**
  * @brief  读取Flash数据
  * @param  ReadAddr: 读取起始地址
  * @param  pBuffer: 数据接收缓冲区指针
  * @param  NumByteToRead: 要读取的字节数
  * @retval None
  * @note   读取操作无需等待，地址和长度自动递增
  */
void     W25Q64_Read_Data(uint32_t ReadAddr, uint8_t *pBuffer, uint32_t NumByteToRead);

/**
  * @brief  写入Flash数据
  * @param  pBuffer: 数据源缓冲区指针
  * @param  WriteAddr: 写入起始地址
  * @param  NumByteToWrite: 要写入的字节数
  * @retval None
  * @note   写入前会自动擦除需要写入的页/扇区
  *         最大单次写入256字节(1页)
  */
void     W25Q64_Write_Data(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);

#endif
