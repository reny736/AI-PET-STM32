/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   I2C1 configuration file
  *          This file provides code for the configuration of the I2C1 instance
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
#include "i2c.h"

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief I2C1句柄定义
  * @note  全局变量，供其他模块使用(OLED显示屏驱动)
  */
I2C_HandleTypeDef hi2c1;

/**
  * @brief  I2C1初始化函数
  * @retval None
  * @note   配置I2C1为快速模式(400KHz)
  *         SCL: PB6, SDA: PB7
  *         用于控制OLED显示屏
  */
void MX_I2C1_Init(void)
{
  /* USER CODE BEGIN I2C1_Init 0 */
  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */
  /* USER CODE END I2C1_Init 1 */

  /* 配置I2C1寄存器 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;                // 400KHz快速模式
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;      // 2:1占空比
  hi2c1.Init.OwnAddress1 = 0;                   // 主设备不使用自身地址
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; // 7位地址模式
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // 禁用双地址模式
  hi2c1.Init.OwnAddress2 = 0;                   // 不使用第二地址
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; // 禁用通用呼叫
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE; // 启用时钟拉伸

  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */
  /* USER CODE END I2C1_Init 2 */
}

/**
  * @brief  I2C1 MSP初始化回调函数
  * @param  i2cHandle: I2C句柄指针
  * @retval None
  * @note   在HAL_I2C_Init内部调用，用于配置I2C1的GPIO和时钟
  *         SCL: PB6 (复用开漏输出)
  *         SDA: PB7 (复用开漏输出)
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */
  /* USER CODE END I2C1_MspInit 0 */

    /* 使能GPIOB时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /* 使能I2C1时钟 */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* 配置I2C1引脚 */
    /* PB6: I2C1_SCL, PB7: I2C1_SDA - 复用开漏输出 */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;      // 复用开漏输出(I2C需要)
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN I2C1_MspInit 1 */
  /* USER CODE END I2C1_MspInit 1 */
  }
}

/**
  * @brief  I2C1 MSP反初始化回调函数
  * @param  i2cHandle: I2C句柄指针
  * @retval None
  * @note   在HAL_I2C_DeInit内部调用，用于关闭I2C1的GPIO和时钟
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */
  /* USER CODE END I2C1_MspDeInit 0 */

    /* 关闭I2C1时钟 */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /* 恢复GPIOB引脚到默认状态 */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */
  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */
