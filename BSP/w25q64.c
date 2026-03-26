/**
  * @file    w25q64.c
  * @brief   W25Q64 Flash存储器驱动实现
  * @details 该文件实现了W25Q64 SPI Flash芯片的读写操作，包括芯片ID读取、
  *          扇区/块/整片擦除、数据读写等功能。W25Q64是一款8Mbit(1MB)的
  *          SPI接口Flash芯片，通过SPI总线与STM32通信。
  * @author  HAL Team
  */
#include "w25q64.h"
#include "spi.h"

extern SPI_HandleTypeDef hspi1;
#define W25Q64_SPI_HANDLE &hspi1

/* 超时时间定义 (ms) */
#define W25Q64_TIMEOUT_VALUE 1000

/**
  * @brief  CS片选拉低函数
  * @param  None
  * @retval None
  * @note   拉低CS引脚选中W25Q64芯片，开始SPI通信
  */
static void W25Q64_CS_Low(void)
{
    HAL_GPIO_WritePin(W25Q64_CS_GPIO_Port, W25Q64_CS_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  CS片选拉高函数
  * @param  None
  * @retval None
  * @note   拉高CS引脚释放W25Q64芯片，结束SPI通信
  */
static void W25Q64_CS_High(void)
{
    HAL_GPIO_WritePin(W25Q64_CS_GPIO_Port, W25Q64_CS_Pin, GPIO_PIN_SET);
}

/**
  * @brief  SPI发送接收一个字节
  * @param  txData: 要发送的数据
  * @retval 接收到的数据
  * @note   使用HAL库的SPI全双工收发函数，同时完成发送和接收
  */
static uint8_t W25Q64_ReadWriteByte(uint8_t txData)
{
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(W25Q64_SPI_HANDLE, &txData, &rxData, 1, 100);
    return rxData;
}

/**
  * @brief  等待芯片空闲函数
  * @param  None
  * @retval 操作状态 (W25Q64_OK/W25Q64_TIMEOUT)
  * @note   通过读取状态寄存器的WIP位(Write In Progress)来判断芯片是否空闲
  *         WIP=1表示正在写入，WIP=0表示空闲
  */
static uint8_t W25Q64_Wait_Busy(void)
{
    uint8_t status = 0;
    uint32_t tickstart = HAL_GetTick();

    W25Q64_CS_Low();
    W25Q64_ReadWriteByte(W25X_ReadStatusReg1);

    do {
        status = W25Q64_ReadWriteByte(0xFF);

        if((HAL_GetTick() - tickstart) > W25Q64_TIMEOUT_VALUE)
        {
            W25Q64_CS_High();
            return W25Q64_TIMEOUT;
        }
    } while ((status & 0x01) == 0x01);

    W25Q64_CS_High();
    return W25Q64_OK;
}

/**
  * @brief  写使能函数
  * @param  None
  * @retval None
  * @note   在执行擦除或编程操作前必须调用此函数使能写入
  */
static void W25Q64_Write_Enable(void)
{
    W25Q64_CS_Low();
    W25Q64_ReadWriteByte(W25X_WriteEnable);
    W25Q64_CS_High();
}

/**
  * @brief  读取W25Q64芯片ID
  * @param  None
  * @retval 32位芯片ID (Manufacture ID[23:16] + Memory Type[15:8] + Capacity[7:0])
  * @note   使用JEDEC ID命令读取芯片标识，正常返回值: 0xEF4017 (Winbond W25Q64)
  */
uint32_t W25Q64_ReadID(void)
{
    uint32_t temp = 0, temp0 = 0, temp1 = 0, temp2 = 0;

    W25Q64_CS_Low();
    W25Q64_ReadWriteByte(W25X_JedecDeviceID);
    temp0 = W25Q64_ReadWriteByte(0xFF);
    temp1 = W25Q64_ReadWriteByte(0xFF);
    temp2 = W25Q64_ReadWriteByte(0xFF);
    W25Q64_CS_High();

    temp = (temp0 << 16) | (temp1 << 8) | temp2;
    return temp;
}

/**
  * @brief  擦除指定扇区 (4KB)
  * @param  Address: 扇区起始地址 (必须为4096的倍数，即4KB对齐)
  * @retval 操作状态 (W25Q64_OK/W25Q64_TIMEOUT)
  * @note   扇区擦除是最小的擦除单位，擦除后该扇区所有数据变为0xFF
  */
uint8_t W25Q64_Erase_Sector(uint32_t Address)
{
    if (W25Q64_Wait_Busy() != W25Q64_OK) return W25Q64_TIMEOUT;

    W25Q64_Write_Enable();

    W25Q64_CS_Low();
    W25Q64_ReadWriteByte(W25X_SectorErase);
    W25Q64_ReadWriteByte((uint8_t)((Address) >> 16));
    W25Q64_ReadWriteByte((uint8_t)((Address) >> 8));
    W25Q64_ReadWriteByte((uint8_t)Address);
    W25Q64_CS_High();

    if (W25Q64_Wait_Busy() != W25Q64_OK) return W25Q64_TIMEOUT;
    return W25Q64_OK;
}

/**
  * @brief  擦除整个芯片
  * @param  None
  * @retval 操作状态 (W25Q64_OK/W25Q64_TIMEOUT)
  * @note   整片擦除可能需要较长时间(数秒到数十秒)，内部会等待完成
  */
uint8_t W25Q64_Erase_Chip(void)
{
    if (W25Q64_Wait_Busy() != W25Q64_OK) return W25Q64_TIMEOUT;

    W25Q64_Write_Enable();
    W25Q64_Wait_Busy();

    W25Q64_CS_Low();
    W25Q64_ReadWriteByte(W25X_ChipErase);
    W25Q64_CS_High();

    if (W25Q64_Wait_Busy() != W25Q64_OK) return W25Q64_TIMEOUT;
    return W25Q64_OK;
}

/**
  * @brief  读取Flash数据
  * @param  ReadAddr: 读取起始地址
  * @param  pBuffer: 数据接收缓冲区指针
  * @param  NumByteToRead: 要读取的字节数
  * @retval None
  * @note   使用标准读取命令(0x03)，地址自动递增，可跨页读取
  */
void W25Q64_Read_Data(uint32_t ReadAddr, uint8_t *pBuffer, uint32_t NumByteToRead)
{
    W25Q64_CS_Low();
    W25Q64_ReadWriteByte(W25X_ReadData);
    W25Q64_ReadWriteByte((uint8_t)((ReadAddr) >> 16));
    W25Q64_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    W25Q64_ReadWriteByte((uint8_t)ReadAddr);

    HAL_SPI_Receive(W25Q64_SPI_HANDLE, pBuffer, NumByteToRead, 2000);

    W25Q64_CS_High();
}

/**
  * @brief  页编程函数
  * @param  pBuffer: 数据源指针
  * @param  WriteAddr: 写入地址
  * @param  NumByteToWrite: 写入字节数 (最大256)
  * @retval None
  * @note   内部函数，单次最多写入256字节(1页)，地址必须在本页范围内
  */
static void W25Q64_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    W25Q64_Write_Enable();

    W25Q64_CS_Low();
    W25Q64_ReadWriteByte(W25X_PageProgram);
    W25Q64_ReadWriteByte((uint8_t)((WriteAddr) >> 16));
    W25Q64_ReadWriteByte((uint8_t)((WriteAddr) >> 8));
    W25Q64_ReadWriteByte((uint8_t)WriteAddr);

    if(NumByteToWrite > 256) NumByteToWrite = 256;

    HAL_SPI_Transmit(W25Q64_SPI_HANDLE, pBuffer, NumByteToWrite, 1000);

    W25Q64_CS_High();
    W25Q64_Wait_Busy();
}

/**
  * @brief  写入数据到Flash (自动处理跨页写入)
  * @param  pBuffer: 数据源缓冲区指针
  * @param  WriteAddr: 写入起始地址
  * @param  NumByteToWrite: 要写入的字节数
  * @retval None
  * @note   自动处理跨页边界，将数据分成多个页写入
  *         写入前无需手动擦除(Flash特性决定必须先擦后写)
  */
void W25Q64_Write_Data(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
    uint16_t pageremain;

    pageremain = 256 - (WriteAddr % 256);

    if (NumByteToWrite <= pageremain) pageremain = NumByteToWrite;

    while (1)
    {
        W25Q64_Write_Page(pBuffer, WriteAddr, pageremain);

        if (NumByteToWrite == pageremain) break;

        pBuffer += pageremain;
        WriteAddr += pageremain;
        NumByteToWrite -= pageremain;

        if (NumByteToWrite > 256)
            pageremain = 256;
        else
            pageremain = NumByteToWrite;
    }
}
