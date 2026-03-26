/**
  ******************************************************************************
  * @file    usart.c
  * @brief   USART1 configuration file
  *          This file provides code for the configuration of the USART1 instance
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
#include "usart.h"

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief USART1句柄定义
  * @note  全局变量，供其他模块使用
  */
UART_HandleTypeDef huart1;

/**
  * @brief  USART1初始化函数
  * @retval None
  * @note   配置USART1为UART模式
  *         波特率: 115200
  *         数据位: 8
  *         停止位: 1
  *         校验位: 无
  *         TX引脚: PA9 (复用推挽输出)
  *         RX引脚: PA10 (浮空输入)
  */
void MX_USART1_UART_Init(void)
{
  /* USER CODE BEGIN USART1_Init 0 */
  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */
  /* USER CODE END USART1_Init 1 */

  /* 配置USART1寄存器 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;               // 波特率115200
  huart1.Init.WordLength = UART_WORDLENGTH_8B; // 8位数据位
  huart1.Init.StopBits = UART_STOPBITS_1;     // 1位停止位
  huart1.Init.Parity = UART_PARITY_NONE;      // 无校验
  huart1.Init.Mode = UART_MODE_TX_RX;         // 收发模式
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 无硬件流控制
  huart1.Init.OverSampling = UART_OVERSAMPLING_16; // 16倍过采样

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  /* USER CODE END USART1_Init 2 */
}

/**
  * @brief  USART1 MSP初始化回调函数
  * @param  uartHandle: USART句柄指针
  * @retval None
  * @note   在HAL_UART_Init内部调用，用于配置USART1的GPIO和时钟
  *         PA9 -> USART1_TX (复用推挽输出)
  *         PA10 -> USART1_RX (浮空输入)
  */
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */
  /* USER CODE END USART1_MspInit 0 */

    /* 使能USART1时钟 */
    __HAL_RCC_USART1_CLK_ENABLE();
    /* 使能GPIOA时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* 配置USART1_TX引脚 (PA9) */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;      // 复用推挽输出
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* 配置USART1_RX引脚 (PA10) */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;      // 浮空输入
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // 无上下拉
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */
    /* 配置USART1中断优先级并使能中断 */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE END USART1_MspInit 1 */
  }
}

/**
  * @brief  USART1 MSP反初始化回调函数
  * @param  uartHandle: USART句柄指针
  * @retval None
  * @note   在HAL_UART_DeInit内部调用，用于关闭USART1的GPIO和时钟
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */
  /* USER CODE END USART1_MspDeInit 0 */

    /* 关闭USART1时钟 */
    __HAL_RCC_USART1_CLK_DISABLE();

    /* 恢复GPIOA9和GPIOA10到默认状态 */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART1_MspDeInit 1 */
  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */
