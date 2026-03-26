/**
  ******************************************************************************
  * @file    usart.h
  * @brief   USART1 configuration header file
  *          This file contains all the function prototypes for the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* External variables --------------------------------------------------------*/

/**
  * @brief USART1句柄定义
  * @note  USART1用于与上位机通信，支持115200波特率
  */
extern UART_HandleTypeDef huart1;

/* Function prototypes -------------------------------------------------------*/

/**
  * @brief  USART1初始化函数
  * @retval None
  * @note   配置USART1为UART模式，115200-8-N-1
  *         TX引脚: PA9, RX引脚: PA10
  */
void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
