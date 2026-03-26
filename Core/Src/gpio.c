/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   GPIO configuration file
  *          This file provides code for the configuration of all used GPIO pins
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
#include "gpio.h"

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/

/**
  * @brief  GPIO初始化函数
  * @retval None
  * @note   配置所有使用的GPIO引脚
  *         本项目中主要配置了W25Q64 Flash的片选引脚(PA4)
  */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* 使能GPIO时钟 */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* 配置GPIO引脚输出电平 - 初始化时片选引脚拉高(释放Flash) */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  /* 配置PA4引脚 - W25Q64 Flash片选引脚 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;    // 推挽输出
  GPIO_InitStruct.Pull = GPIO_PULLUP;            // 上拉
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 高速
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
/* USER CODE END 2 */
