/**
  ******************************************************************************
  * @file    spi.h
  * @brief   SPI1 configuration header file
  *          This file contains all the function prototypes for the spi.c file
  * @author  HAL Team
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* External variables --------------------------------------------------------*/

/**
  * @brief SPI1句柄定义
  * @note  SPI1用于与W25Q64 Flash通信
  *        SCK: PA5, MISO: PA6, MOSI: PA7
  */
extern SPI_HandleTypeDef hspi1;

/* Function prototypes -------------------------------------------------------*/

/**
  * @brief  SPI1初始化函数
  * @retval None
  * @note   配置SPI1为主机模式，8位数据，MSB优先
  *         时钟分频: 8 (约9Mbps)
  */
void MX_SPI1_Init(void);

/* USER CODE BEGIN Prototypes */
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
