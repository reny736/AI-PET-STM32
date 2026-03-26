/**
  ******************************************************************************
  * @file    spi.c
  * @brief   SPI1 configuration file
  *          This file provides code for the configuration of the SPI1 instance
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
#include "spi.h"

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief SPI1句柄定义
  * @note  全局变量，供其他模块使用(W25Q64 Flash驱动)
  */
SPI_HandleTypeDef hspi1;

/**
  * @brief  SPI1初始化函数
  * @retval None
  * @note   配置SPI1为主机模式
  *         SCK: PA5, MISO: PA6, MOSI: PA7
  *         时钟分频: 8 (APB2总线72MHz/8=9MHz)
  *         数据位: 8位, MSB优先, 模式0
  */
void MX_SPI1_Init(void)
{
  /* USER CODE BEGIN SPI1_Init 0 */
  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */
  /* USER CODE END SPI1_Init 1 */

  /* 配置SPI1寄存器 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;                    // 主机模式
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;         // 全双工
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;             // 8位数据
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;          // 时钟低空闲
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;              // 第一个边沿采样
  hspi1.Init.NSS = SPI_NSS_SOFT;                      // 软件控制片选
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; // 9MHz
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;              // MSB优先
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;             // 禁用TI模式
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; // 禁用CRC
  hspi1.Init.CRCPolynomial = 10;                       // CRC多项式

  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */
  /* USER CODE END SPI1_Init 2 */
}

/**
  * @brief  SPI1 MSP初始化回调函数
  * @param  spiHandle: SPI句柄指针
  * @retval None
  * @note   在HAL_SPI_Init内部调用，用于配置SPI1的GPIO和时钟
  *         SCK: PA5 (复用推挽输出)
  *         MISO: PA6 (浮空输入)
  *         MOSI: PA7 (复用推挽输出)
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */
  /* USER CODE END SPI1_MspInit 0 */

    /* 使能SPI1时钟 */
    __HAL_RCC_SPI1_CLK_ENABLE();
    /* 使能GPIOA时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* 配置SPI1引脚 */
    /* PA5: SPI1_SCK, PA7: SPI1_MOSI - 复用推挽输出 */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA6: SPI1_MISO - 浮空输入 */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */
  /* USER CODE END SPI1_MspInit 1 */
  }
}

/**
  * @brief  SPI1 MSP反初始化回调函数
  * @param  spiHandle: SPI句柄指针
  * @retval None
  * @note   在HAL_SPI_DeInit内部调用，用于关闭SPI1的GPIO和时钟
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */
  /* USER CODE END SPI1_MspDeInit 0 */

    /* 关闭SPI1时钟 */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /* 恢复GPIOA引脚到默认状态 */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */
  /* USER CODE END SPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */
