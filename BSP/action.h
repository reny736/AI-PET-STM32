/**
  * @file    action.h
  * @brief   动作控制模块头文件
  * @details 该文件包含了机器人动作控制相关的宏定义、结构体定义和函数声明
  * @author  HAL Team
  */
#ifndef __ACTION_H
#define __ACTION_H

#include "main.h"

/* 动作模式定义 */
#define MODE_SIT        0   // 坐下模式
#define MODE_RELAX      1   // 放松模式
#define MODE_DOWN       2   // 趴下模式
#define MODE_STAND      3   // 站立模式
#define MODE_ADVANCE    4   // 前进模式
#define MODE_BACK       5   // 后退模式
#define MODE_LROTATION  6   // 向左旋转模式
#define MODE_RROTATION  7   // 向右旋转模式
#define MODE_SWING      8   // 摇摆模式
#define MODE_SWINGTAIL  9   // 摇尾巴模式

/* 舵机角度定义 */
#define ANGLE_STAND     90  // 站立角度
#define ANGLE_SIT       20  // 坐下角度
#define ANGLE_FORWARD   45  // 向前角度
#define ANGLE_BACKWARD  135 // 向后角度

/* 全局变量声明 */
extern uint8_t Action_Mode;     // 当前动作模式
extern uint16_t SwingDelay;     // 摇摆动作延迟时间(ms)
extern uint16_t SpeedDelay;     // 基本运动延迟时间(ms)

/* 动作状态管理结构体 */
typedef struct {
    uint8_t current_step;     // 当前动作步骤
    uint8_t total_steps;      // 总步骤数
    uint32_t last_step_time;  // 上次执行步骤的时间
    uint16_t step_delay;      // 步骤间延时(ms)
} ActionState_t;

/* 动作状态变量 */
extern ActionState_t g_advance_state;     // 前进动作状态
extern ActionState_t g_back_state;        // 后退动作状态
extern ActionState_t g_lrotation_state;   // 左旋转动作状态
extern ActionState_t g_rrotation_state;   // 右旋转动作状态
extern ActionState_t g_swing_state;       // 摇摆动作状态
extern ActionState_t g_swingtail_state;   // 摇尾巴动作状态

/* 动作状态管理函数 */
/**
  * @brief  初始化动作状态
  * @param  state: 动作状态指针
  * @param  total_steps: 总步骤数
  * @param  step_delay: 步骤间延时
  * @retval None
  */
void Action_InitState(ActionState_t* state, uint8_t total_steps, uint16_t step_delay);

/**
  * @brief  检查是否应该执行下一步
  * @param  state: 动作状态指针
  * @retval 1-应该执行，0-不执行
  */
uint8_t Action_ShouldExecuteStep(ActionState_t* state);

/**
  * @brief  进入下一步
  * @param  state: 动作状态指针
  * @retval None
  */
void Action_NextStep(ActionState_t* state);

/* 舵机控制函数声明 */
/**
  * @brief  控制左上舵机角度
  * @param  Angle: 舵机角度(0-180度)
  * @retval None
  */
void Servo_1(float Angle);      // 左上舵机

/**
  * @brief  控制右上舵机角度
  * @param  Angle: 舵机角度(0-180度)
  * @retval None
  */
void Servo_2(float Angle);      // 右上舵机

/**
  * @brief  控制左下舵机角度
  * @param  Angle: 舵机角度(0-180度)
  * @retval None
  */
void Servo_3(float Angle);      // 左下舵机

/**
  * @brief  控制右下舵机角度
  * @param  Angle: 舵机角度(0-180度)
  * @retval None
  */
void Servo_4(float Angle);      // 右下舵机

/**
  * @brief  控制尾巴舵机角度
  * @param  Angle: 舵机角度(0-180度)
  * @retval None
  */
void Servo_5(float Angle);      // 尾巴舵机

/* 基本动作函数声明 */
/**
  * @brief  放松趴下动作
  * @retval None
  */
void Action_relaxed_getdowm(void);  // 放松趴下

/**
  * @brief  站立动作
  * @retval None
  */
void Action_upright(void);          // 站立

/**
  * @brief  趴下动作
  * @retval None
  */
void Action_getdowm(void);          // 趴下

/**
  * @brief  坐下动作
  * @retval None
  */
void Action_sit(void);              // 坐下

/* 运动动作函数声明 */
/**
  * @brief  摇摆动作
  * @retval None
  */
void Action_Swing(void);            // 摇摆

/**
  * @brief  前进动作
  * @retval None
  */
void Action_advance(void);          // 前进

/**
  * @brief  后退动作
  * @retval None
  */
void Action_back(void);             // 后退

/**
  * @brief  向左旋转动作
  * @retval None
  */
void Action_Lrotation(void);        // 向左旋转

/**
  * @brief  向右旋转动作
  * @retval None
  */
void Action_Rrotation(void);        // 向右旋转

/**
  * @brief  摇尾巴动作
  * @retval None
  */
void Action_SwingTail(void);        // 摇尾巴

#endif /* __ACTION_H */

