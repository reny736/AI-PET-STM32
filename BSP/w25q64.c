#include "w25q64.h"
#include "spi.h" // 确保包含CubeMX生成的spi.h，以便引用hspi1

// 引用外部定义的SPI句柄
extern SPI_HandleTypeDef hspi1; 
#define W25Q64_SPI_HANDLE &hspi1

// 超时时间定义 (ms)
#define W25Q64_TIMEOUT_VALUE 1000 

/**
  * @brief  CS片选控制
  */
static void W25Q64_CS_Low(void)
{
    HAL_GPIO_WritePin(W25Q64_CS_GPIO_Port, W25Q64_CS_Pin, GPIO_PIN_RESET);
}

static void W25Q64_CS_High(void)
{
    HAL_GPIO_WritePin(W25Q64_CS_GPIO_Port, W25Q64_CS_Pin, GPIO_PIN_SET);
}

/**
  * @brief  SPI 发送接收一个字节
  */
static uint8_t W25Q64_ReadWriteByte(uint8_t txData)
{
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(W25Q64_SPI_HANDLE, &txData, &rxData, 1, 100);
    return rxData;
}

/**
  * @brief  等待芯片忙碌结束 (WIP位)
  */
static uint8_t W25Q64_Wait_Busy(void)
{
    uint8_t status = 0;
    uint32_t tickstart = HAL_GetTick();

    W25Q64_CS_Low();
    W25Q64_ReadWriteByte(W25X_ReadStatusReg1);
    
    do {
        status = W25Q64_ReadWriteByte(0xFF); // 发送Dummy读取状态
        
        // 超时机制
        if((HAL_GetTick() - tickstart) > W25Q64_TIMEOUT_VALUE)
        {
            W25Q64_CS_High();
            return W25Q64_TIMEOUT; 
        }
    } while ((status & 0x01) == 0x01); // 判断WIP位 (Write In Progress)

    W25Q64_CS_High();
    return W25Q64_OK;
}

/**
  * @brief  写使能
  */
static void W25Q64_Write_Enable(void)
{
    W25Q64_CS_Low();
    W25Q64_ReadWriteByte(W25X_WriteEnable);
    W25Q64_CS_High();
}

/**
  * @brief  读取W25Q64 ID
  * @retval 32位ID (Manufacture ID + Memory Type + Capacity)
  * 例如: 0xEF4017 (Winbond, W25Q64)
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
  * @brief  擦除一个扇区 (4KB)
  * @param  Address: 扇区地址 (需对齐4KB)
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
  * @brief  擦除整个芯片 (耗时较长)
  */
uint8_t W25Q64_Erase_Chip(void)
{
    if (W25Q64_Wait_Busy() != W25Q64_OK) return W25Q64_TIMEOUT;

    W25Q64_Write_Enable();
    W25Q64_Wait_Busy();

    W25Q64_CS_Low();
    W25Q64_ReadWriteByte(W25X_ChipErase);
    W25Q64_CS_High();

    // 芯片擦除时间较长，可能需要增加超时阈值或在此处单独处理
    if (W25Q64_Wait_Busy() != W25Q64_OK) return W25Q64_TIMEOUT;
    return W25Q64_OK;
}

/**
  * @brief  读取数据
  * @param  ReadAddr: 开始读取的地址
  * @param  pBuffer: 数据存储指针
  * @param  NumByteToRead: 读取字节数
  */
void W25Q64_Read_Data(uint32_t ReadAddr, uint8_t *pBuffer, uint32_t NumByteToRead)
{
    W25Q64_CS_Low();
    W25Q64_ReadWriteByte(W25X_ReadData);
    W25Q64_ReadWriteByte((uint8_t)((ReadAddr) >> 16));
    W25Q64_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    W25Q64_ReadWriteByte((uint8_t)ReadAddr);

    // 使用HAL库的高速接收以提高效率，也可以用循环逐字节读
    HAL_SPI_Receive(W25Q64_SPI_HANDLE, pBuffer, NumByteToRead, 2000);
    
    W25Q64_CS_High();
}

/**
  * @brief  页写入 (内部函数，最大256字节，不可跨页)
  * @param  pBuffer: 数据指针
  * @param  WriteAddr: 写入地址
  * @param  NumByteToWrite: 写入字节数 (0-256)
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
    W25Q64_Wait_Busy(); // 等待写入结束
}

/**
  * @brief  写入任意长度数据 (自动处理跨页和翻页)
  * @param  pBuffer: 数据源指针
  * @param  WriteAddr: 写入起始地址
  * @param  NumByteToWrite: 写入字节数
  */
void W25Q64_Write_Data(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
    uint16_t pageremain; // 当前页剩余字节数
    pageremain = 256 - (WriteAddr % 256); // 计算当前页还能写多少

    if (NumByteToWrite <= pageremain) pageremain = NumByteToWrite; // 如果不够一页，直接写

    while (1)
    {
        W25Q64_Write_Page(pBuffer, WriteAddr, pageremain);
        
        if (NumByteToWrite == pageremain) break; // 写入完成

        // 更新数据和指针
        pBuffer += pageremain;
        WriteAddr += pageremain;
        NumByteToWrite -= pageremain;

        // 下一次写入的长度
        if (NumByteToWrite > 256) 
            pageremain = 256;
        else 
            pageremain = NumByteToWrite;
    }
}

