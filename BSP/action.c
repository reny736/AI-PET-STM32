#include "main.h"
#include "tim.h"
#include "cmsis_os.h"
#include "action.h"

uint8_t Action_Mode=0;
uint16_t SwingDelay = 8;  // 摇摆延迟(ms)
uint16_t SpeedDelay = 200; // 运动延迟(ms)

/* 动作状态变量定义 */
ActionState_t g_advance_state = {0};
ActionState_t g_back_state = {0};
ActionState_t g_lrotation_state = {0};
ActionState_t g_rrotation_state = {0};
ActionState_t g_swing_state = {0};
ActionState_t g_swingtail_state = {0};

/**
  * @brief  初始化动作状态
  * @param  state: 动作状态指针
  * @param  total_steps: 总步骤数
  * @param  step_delay: 步骤间延时
  * @retval None
  */
void Action_InitState(ActionState_t* state, uint8_t total_steps, uint16_t step_delay)
{
    state->current_step = 0;
    state->total_steps = total_steps;
    state->last_step_time = 0;
    state->step_delay = step_delay;
}

/**
  * @brief  检查是否应该执行下一步
  * @param  state: 动作状态指针
  * @retval 1-应该执行，0-不执行
  */
uint8_t Action_ShouldExecuteStep(ActionState_t* state)
{
    uint32_t current_time = osKernelGetTickCount();
    if (current_time - state->last_step_time >= state->step_delay)
    {
        state->last_step_time = current_time;
        return 1;
    }
    return 0;
}

/**
  * @brief  进入下一步
  * @param  state: 动作状态指针
  * @retval None
  */
void Action_NextStep(ActionState_t* state)
{
    state->current_step = (state->current_step + 1) % state->total_steps;
}

void Servo_1(float Angle)//左上
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Angle / 180 * 2000 + 500);
}

void Servo_2(float Angle)//右上
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (180-Angle) / 180 * 2000 + 500);
}

void Servo_3(float Angle)//左下
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Angle / 180 * 2000 + 500);
}

void Servo_4(float Angle)//右下
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, (180-Angle) / 180 * 2000 + 500);
}

void Servo_5(float Angle)//尾巴
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, Angle / 180 * 2000 + 500);
}


/* 辅助函数：设置四个舵机角度 */
static void SetAllServos(float angle1, float angle2, float angle3, float angle4)
{
    Servo_1(angle1);
    Servo_2(angle2);
    Servo_3(angle3);
    Servo_4(angle4);
}






/*舵机位置
	    1        2
	
	

	
	    3        4
	*/
	
	//舵机充当腿，括号里的值为0时表示的是腿向前进的方向甩，90度是站立

void Action_relaxed_getdowm(void)
{
	SetAllServos(20, 20, 160, 160);
}

void Action_upright(void)//站立
{
	SetAllServos(90, 90, 90, 90);
}

void Action_getdowm(void)//趴下
{
	SetAllServos(20, 20, 20, 20);
}

void Action_sit(void)//坐下
{
	SetAllServos(90, 90, 20, 20);
}
void Action_Swing(void)//摇摆
{
	// 初始化状态（仅在模式改变时）
	static uint8_t last_mode = 0xFF;
	if (last_mode != Action_Mode)
	{
		Action_InitState(&g_swing_state, 240, SwingDelay); // 240步 (30->149->30)
		last_mode = Action_Mode;
	}
	
	// 检查是否应该执行下一步
	if (!Action_ShouldExecuteStep(&g_swing_state))
		return;
	
	// 计算当前角度
	uint8_t angle;
	if (g_swing_state.current_step < 120) // 30->149
	{
		angle = 30 + g_swing_state.current_step;
	}
	else // 149->30
	{
		angle = 149 - (g_swing_state.current_step - 120);
	}
	
	// 设置舵机角度
	SetAllServos(angle, angle, angle, angle);
	
	// 进入下一步
	Action_NextStep(&g_swing_state);
}

void Action_advance(void)//前进
{
	// 初始化状态（仅在模式改变时）
	static uint8_t last_mode = 0xFF;
	if (last_mode != Action_Mode)
	{
		Action_InitState(&g_advance_state, 8, SpeedDelay);
		last_mode = Action_Mode;
	}
	
	// 检查是否应该执行下一步
	if (!Action_ShouldExecuteStep(&g_advance_state))
		return;
	
	// 前进动作序列
	static const float steps[][4] = {
		{ANGLE_FORWARD, ANGLE_STAND, ANGLE_STAND, ANGLE_FORWARD},      // 步骤1
		{ANGLE_FORWARD, ANGLE_BACKWARD, ANGLE_BACKWARD, ANGLE_FORWARD},    // 步骤2
		{ANGLE_STAND, ANGLE_BACKWARD, ANGLE_BACKWARD, ANGLE_STAND},          // 步骤3
		{ANGLE_STAND, ANGLE_STAND, ANGLE_STAND, ANGLE_STAND},          // 步骤4
		{ANGLE_STAND, ANGLE_FORWARD, ANGLE_FORWARD, ANGLE_STAND},      // 步骤5
		{ANGLE_BACKWARD, ANGLE_FORWARD, ANGLE_FORWARD, ANGLE_BACKWARD},    // 步骤6
		{ANGLE_BACKWARD, ANGLE_STAND, ANGLE_STAND, ANGLE_BACKWARD},          // 步骤7
		{ANGLE_STAND, ANGLE_STAND, ANGLE_STAND, ANGLE_STAND}           // 步骤8
	};
	
	// 执行当前步骤
	uint8_t step = g_advance_state.current_step;
	SetAllServos(steps[step][0], steps[step][1], steps[step][2], steps[step][3]);
	
	// 进入下一步
	Action_NextStep(&g_advance_state);
}

void Action_back(void)//后退
{
	// 初始化状态（仅在模式改变时）
	static uint8_t last_mode = 0xFF;
	if (last_mode != Action_Mode)
	{
		Action_InitState(&g_back_state, 8, SpeedDelay);
		last_mode = Action_Mode;
	}
	
	// 检查是否应该执行下一步
	if (!Action_ShouldExecuteStep(&g_back_state))
		return;
	
	// 后退动作序列
	static const float steps[][4] = {
		{ANGLE_BACKWARD, ANGLE_STAND, ANGLE_STAND, ANGLE_BACKWARD},    // 步骤1
		{ANGLE_BACKWARD, ANGLE_FORWARD, ANGLE_FORWARD, ANGLE_BACKWARD},      // 步骤2
		{ANGLE_STAND, ANGLE_FORWARD, ANGLE_FORWARD, ANGLE_STAND},          // 步骤3
		{ANGLE_STAND, ANGLE_STAND, ANGLE_STAND, ANGLE_STAND},          // 步骤4
		{ANGLE_STAND, ANGLE_BACKWARD, ANGLE_BACKWARD, ANGLE_STAND},    // 步骤5
		{ANGLE_FORWARD, ANGLE_BACKWARD, ANGLE_BACKWARD, ANGLE_FORWARD},      // 步骤6
		{ANGLE_FORWARD, ANGLE_STAND, ANGLE_STAND, ANGLE_FORWARD},          // 步骤7
		{ANGLE_STAND, ANGLE_STAND, ANGLE_STAND, ANGLE_STAND}           // 步骤8
	};
	
	// 执行当前步骤
	uint8_t step = g_back_state.current_step;
	SetAllServos(steps[step][0], steps[step][1], steps[step][2], steps[step][3]);
	
	// 进入下一步
	Action_NextStep(&g_back_state);
}

void Action_Lrotation(void)//向左旋转
{
	// 初始化状态（仅在模式改变时）
	static uint8_t last_mode = 0xFF;
	if (last_mode != Action_Mode)
	{
		Action_InitState(&g_lrotation_state, 4, SpeedDelay);
		last_mode = Action_Mode;
	}
	
	// 检查是否应该执行下一步
	if (!Action_ShouldExecuteStep(&g_lrotation_state))
		return;
	
	// 向左旋转动作序列
	static const float steps[][4] = {
		{135, ANGLE_STAND, ANGLE_STAND, 45},     // 步骤1
		{135, 135, 45, 45},     // 步骤2
		{ANGLE_STAND, 135, 45, ANGLE_STAND},          // 步骤3
		{ANGLE_STAND, ANGLE_STAND, ANGLE_STAND, ANGLE_STAND}           // 步骤4
	};
	
	// 执行当前步骤
	uint8_t step = g_lrotation_state.current_step;
	SetAllServos(steps[step][0], steps[step][1], steps[step][2], steps[step][3]);
	
	// 进入下一步
	Action_NextStep(&g_lrotation_state);
}


void Action_Rrotation(void)//向右旋转
{
	// 初始化状态（仅在模式改变时）
	static uint8_t last_mode = 0xFF;
	if (last_mode != Action_Mode)
	{
		Action_InitState(&g_rrotation_state, 4, SpeedDelay);
		last_mode = Action_Mode;
	}
	
	// 检查是否应该执行下一步
	if (!Action_ShouldExecuteStep(&g_rrotation_state))
		return;
	
	// 向右旋转动作序列
	static const float steps[][4] = {
		{ANGLE_FORWARD, 90, 90, ANGLE_BACKWARD},     // 步骤1
		{45, ANGLE_FORWARD, ANGLE_BACKWARD, 135},     // 步骤2
		{ANGLE_STAND, 45, 135, ANGLE_STAND},          // 步骤3
		{90, ANGLE_STAND, ANGLE_STAND, 90}           // 步骤4
	};
	
	// 执行当前步骤
	uint8_t step = g_rrotation_state.current_step;
	SetAllServos(steps[step][0], steps[step][1], steps[step][2], steps[step][3]);
	
	// 进入下一步
	Action_NextStep(&g_rrotation_state);
}

void Action_SwingTail(void)//摇尾巴
{
	// 初始化状态（仅在模式改变时）
	static uint8_t last_mode = 0xFF;
	if (last_mode != Action_Mode)
	{
		Action_InitState(&g_swingtail_state, 240, SwingDelay); // 240步 (30->149->30)
		last_mode = Action_Mode;
	}
	
	// 检查是否应该执行下一步
	if (!Action_ShouldExecuteStep(&g_swingtail_state))
		return;
	
	// 计算当前角度
	uint8_t angle;
	if (g_swingtail_state.current_step < 120) // 30->149
	{
		angle = 30 + g_swingtail_state.current_step;
	}
	else // 149->30
	{
		angle = 149 - (g_swingtail_state.current_step - 120);
	}
	
	// 设置舵机角度
	Servo_5(angle);
	
	// 进入下一步
	Action_NextStep(&g_swingtail_state);
}


