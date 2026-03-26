/**
  ******************************************************************************
  * @file    tim.h
  * @brief   Timer configuration header file
  *          This file contains all the function prototypes for the tim.c file
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
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* External variables --------------------------------------------------------*/

/**
  * @brief TIM2句柄定义
  * @note  TIM2用于产生PWM信号控制4个腿部舵机
  *        CH1: PA0, CH2: PA1, CH3: PA2, CH4: PA3
  */
extern TIM_HandleTypeDef htim2;

/**
  * @brief TIM3句柄定义
  * @note  TIM3用于产生PWM信号控制尾巴舵机
  *        CH1: PB4
  */
extern TIM_HandleTypeDef htim3;

/* Function prototypes -------------------------------------------------------*/

/**
  * @brief  TIM2初始化函数
  * @retval None
  * @note   配置TIM2为PWM模式，用于控制4个腿部舵机
  *         频率: 50Hz (周期20ms)，与舵机控制标准兼容
  */
void MX_TIM2_Init(void);

/**
  * @brief  TIM3初始化函数
  * @retval None
  * @note   配置TIM3为PWM模式，用于控制尾巴舵机
  *         频率: 50Hz (周期20ms)，与舵机控制标准兼容
  */
void MX_TIM3_Init(void);

/**
  * @brief  Timer GPIO初始化后回调函数
  * @param  htim: Timer句柄指针
  * @retval None
  * @note   在Timer GPIO初始化完成后自动调用，配置相关GPIO引脚
  */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */
