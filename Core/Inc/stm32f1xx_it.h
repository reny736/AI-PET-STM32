/**
  ******************************************************************************
  * @file    stm32f1xx_it.h
  * @brief   Interrupt handlers header file
  *          This file contains the headers of the interrupt handlers
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
#ifndef __STM32F1xx_IT_H
#define __STM32F1xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  Non-maskable interrupt handler
  * @retval None
  */
void NMI_Handler(void);

/**
  * @brief  Hard fault interrupt handler
  * @retval None
  */
void HardFault_Handler(void);

/**
  * @brief  Memory management fault handler
  * @retval None
  */
void MemManage_Handler(void);

/**
  * @brief  Prefetch fault/memory access fault handler
  * @retval None
  */
void BusFault_Handler(void);

/**
  * @brief  Undefined instruction/illegal state handler
  * @retval None
  */
void UsageFault_Handler(void);

/**
  * @brief  Debug monitor handler
  * @retval None
  */
void DebugMon_Handler(void);

/**
  * @brief  TIM4 global interrupt handler
  * @retval None
  * @note   TIM4用于HAL库的毫秒级tick计数
  */
void TIM4_IRQHandler(void);

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_IT_H */
