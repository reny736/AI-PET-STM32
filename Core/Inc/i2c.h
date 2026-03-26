/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   I2C1 configuration header file
  *          This file contains all the function prototypes for the i2c.c file
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
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* External variables --------------------------------------------------------*/

/**
  * @brief I2C1句柄定义
  * @note  I2C1用于与OLED显示屏通信
  *        SCL: PB6, SDA: PB7
  */
extern I2C_HandleTypeDef hi2c1;

/* Function prototypes -------------------------------------------------------*/

/**
  * @brief  I2C1初始化函数
  * @retval None
  * @note   配置I2C1为快速模式(400KHz)
  *         用于控制OLED显示屏
  */
void MX_I2C1_Init(void);

/* USER CODE BEGIN Prototypes */
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */
