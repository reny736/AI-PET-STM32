/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   GPIO configuration header file
  *          This file contains all the function prototypes for the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Function prototypes --------------------------------------------------------*/

/**
  * @brief  GPIO初始化函数
  * @retval None
  * @note   配置GPIO引脚的模式和参数
  *         本项目中用于配置W25Q64 Flash的片选引脚(PA4)
  */
void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */
