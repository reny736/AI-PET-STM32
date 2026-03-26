#ifndef __ACTION_H
#define __ACTION_H

#include "main.h"

/* 动作模式定义 */
#define MODE_SIT        0
#define MODE_RELAX      1
#define MODE_DOWN       2
#define MODE_STAND      3
#define MODE_ADVANCE    4
#define MODE_BACK       5
#define MODE_LROTATION  6
#define MODE_RROTATION  7
#define MODE_SWING      8
#define MODE_SWINGTAIL  9

/* 舵机角度定义 */
#define ANGLE_STAND     90
#define ANGLE_SIT       20
#define ANGLE_FORWARD   45
#define ANGLE_BACKWARD  135

/* 全局变量声明 */
extern uint8_t Action_Mode;
extern uint16_t SwingDelay;
extern uint16_t SpeedDelay;

/* 动作状态管理 */
typedef struct {
    uint8_t current_step;     // 当前动作步骤
    uint8_t total_steps;      // 总步骤数
    uint32_t last_step_time;  // 上次执行步骤的时间
    uint16_t step_delay;      // 步骤间延时(ms)
} ActionState_t;

/* 动作状态变量 */
extern ActionState_t g_advance_state;
extern ActionState_t g_back_state;
extern ActionState_t g_lrotation_state;
extern ActionState_t g_rrotation_state;
extern ActionState_t g_swing_state;
extern ActionState_t g_swingtail_state;

/* 动作状态管理函数 */
void Action_InitState(ActionState_t* state, uint8_t total_steps, uint16_t step_delay);
uint8_t Action_ShouldExecuteStep(ActionState_t* state);
void Action_NextStep(ActionState_t* state);

/* 舵机控制函数声明 */
void Servo_1(float Angle);      // 左上舵机
void Servo_2(float Angle);      // 右上舵机
void Servo_3(float Angle);      // 左下舵机
void Servo_4(float Angle);      // 右下舵机
void Servo_5(float Angle);      // 尾巴舵机

/* 基本动作函数声明 */
void Action_relaxed_getdowm(void);  // 放松趴下
void Action_upright(void);          // 站立
void Action_getdowm(void);          // 趴下
void Action_sit(void);              // 坐下

/* 运动动作函数声明 */
void Action_Swing(void);            // 摇摆
void Action_advance(void);          // 前进
void Action_back(void);             // 后退
void Action_Lrotation(void);        // 向左旋转
void Action_Rrotation(void);        // 向右旋转
void Action_SwingTail(void);        // 摇尾巴

#endif /* __ACTION_H */

