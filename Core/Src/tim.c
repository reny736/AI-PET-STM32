/**
  ******************************************************************************
  * @file    tim.c
  * @brief   Timer configuration file
  *          This file provides code for the configuration of the TIM instances
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
#include "tim.h"

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief TIM2句柄定义
  * @note  全局变量，用于产生PWM控制4个腿部舵机
  */
TIM_HandleTypeDef htim2;

/**
  * @brief TIM3句柄定义
  * @note  全局变量，用于产生PWM控制尾巴舵机
  */
TIM_HandleTypeDef htim3;

/**
  * @brief  TIM2初始化函数 - 腿部舵机PWM
  * @retval None
  * @note   配置TIM2为PWM模式，用于控制4个腿部舵机
  *         CH1: PA0 - 左上舵机
  *         CH2: PA1 - 右上舵机
  *         CH3: PA2 - 左下舵机
  *         CH4: PA3 - 右下舵机
  *         PWM频率: 50Hz (周期20ms)
  *         PWM分辨率: 20000/72MHz ≈ 20ms
  */
void MX_TIM2_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */
  /* USER CODE END TIM2_Init 1 */

  /* 配置TIM2寄存器 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;                    // 预分频: 72MHz/72 = 1MHz
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;  // 向上计数
  htim2.Init.Period = 20000-1;                  // 自动重装载值: 20000 (20ms)
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 时钟不分频
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // 使能自动重装载预装载

  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  /* 配置主从模式 */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* 配置PWM通道1 - 左上舵机 (PA0) */
  sConfigOC.OCMode = TIM_OCMODE_PWM1;         // PWM模式1
  sConfigOC.Pulse = 0;                         // 初始脉宽为0
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; // 高电平有效
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;  // 禁用快速输出
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* 配置PWM通道2 - 右上舵机 (PA1) */
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

  /* 配置PWM通道3 - 左下舵机 (PA2) */
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  /* 配置PWM通道4 - 右下舵机 (PA3) */
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN TIM2_Init 2 */
  /* USER CODE END TIM2_Init 2 */

  /* 调用GPIO初始化回调函数 */
  HAL_TIM_MspPostInit(&htim2);
}

/**
  * @brief  TIM3初始化函数 - 尾巴舵机PWM
  * @retval None
  * @note   配置TIM3为PWM模式，用于控制尾巴舵机
  *         CH1: PB4 - 尾巴舵机
  *         PWM频率: 50Hz (周期20ms)
  */
void MX_TIM3_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */
  /* USER CODE END TIM3_Init 1 */

  /* 配置TIM3寄存器 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 71;                    // 预分频: 72MHz/72 = 1MHz
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;  // 向上计数
  htim3.Init.Period = 20000-1;                  // 自动重装载值: 20000 (20ms)
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 时钟不分频
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // 使能自动重装载预装载

  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }

  /* 配置主从模式 */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* 配置PWM通道1 - 尾巴舵机 (PB4) */
  sConfigOC.OCMode = TIM_OCMODE_PWM1;         // PWM模式1
  sConfigOC.Pulse = 0;                         // 初始脉宽为0
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; // 高电平有效
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;  // 禁用快速输出
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN TIM3_Init 2 */
  /* USER CODE END TIM3_Init 2 */

  /* 调用GPIO初始化回调函数 */
  HAL_TIM_MspPostInit(&htim3);
}

/**
  * @brief  Timer PWM MSP初始化函数
  * @param  tim_pwmHandle: Timer句柄指针
  * @retval None
  * @note   在HAL_TIM_PWM_Init内部调用，使能Timer时钟
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{
  if(tim_pwmHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */
  /* USER CODE END TIM2_MspInit 0 */
    /* 使能TIM2时钟 */
    __HAL_RCC_TIM2_CLK_ENABLE();
  /* USER CODE BEGIN TIM2_MspInit 1 */
  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(tim_pwmHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */
  /* USER CODE END TIM3_MspInit 0 */
    /* 使能TIM3时钟 */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */
  /* USER CODE END TIM3_MspInit 1 */
  }
}

/**
  * @brief  Timer GPIO初始化后回调函数
  * @param  timHandle: Timer句柄指针
  * @retval None
  * @note   配置Timer的GPIO引脚为复用推挽输出模式
  *         TIM2: PA0, PA1, PA2, PA3
  *         TIM3: PB4 (需要部分重映射)
  */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(timHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspPostInit 0 */
  /* USER CODE END TIM2_MspPostInit 0 */
    /* 使能GPIOA时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* 配置TIM2 GPIO引脚 */
    /* PA0: TIM2_CH1, PA1: TIM2_CH2, PA2: TIM2_CH3, PA3: TIM2_CH4 */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;      // 复用推挽输出
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 低速(舵机不需要高速)
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspPostInit 1 */
  /* USER CODE END TIM2_MspPostInit 1 */
  }
  else if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */
  /* USER CODE END TIM3_MspPostInit 0 */

    /* 使能GPIOB时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* 配置TIM3 GPIO引脚 */
    /* PB4: TIM3_CH1 (需要部分重映射) */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;      // 复用推挽输出
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 低速
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* 使能TIM3部分重映射，将CH1输出到PB4 */
    __HAL_AFIO_REMAP_TIM3_PARTIAL();

  /* USER CODE BEGIN TIM3_MspPostInit 1 */
  /* USER CODE END TIM3_MspPostInit 1 */
  }
}

/**
  * @brief  Timer PWM MSP反初始化函数
  * @param  tim_pwmHandle: Timer句柄指针
  * @retval None
  * @note   在HAL_TIM_PWM_DeInit内部调用，关闭Timer时钟
  */
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{
  if(tim_pwmHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */
  /* USER CODE END TIM2_MspDeInit 0 */
    /* 关闭TIM2时钟 */
    __HAL_RCC_TIM2_CLK_DISABLE();
  /* USER CODE BEGIN TIM2_MspDeInit 1 */
  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_pwmHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */
  /* USER CODE END TIM3_MspDeInit 0 */
    /* 关闭TIM3时钟 */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */
  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */
