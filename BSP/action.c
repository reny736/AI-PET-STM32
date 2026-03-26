/**
  * @file    action.c
  * @brief   动作控制模块实现
  * @details 该文件包含了机器人的各种动作控制函数，包括站立、趴下、前进、后退、旋转等基本动作
  *          以及摇摆、摇尾巴等特殊动作的实现。
  * @author  HAL Team
  */
#include "main.h"
#include "tim.h"
#include "cmsis_os.h"
#include "action.h"

/* 全局变量定义 */
uint8_t Action_Mode = 0;        // 当前动作模式，用于控制机器人执行不同的动作
uint16_t SwingDelay = 8;        // 摇摆动作延迟时间(ms)，控制摇摆动作的速度
uint16_t SpeedDelay = 200;      // 基本运动延迟时间(ms)，控制前进、后退、旋转等动作的速度

/* 动作状态变量定义 - 用于管理不同动作的执行状态 */
ActionState_t g_advance_state = {0};      // 前进动作状态，记录前进动作的执行进度
ActionState_t g_back_state = {0};         // 后退动作状态，记录后退动作的执行进度
ActionState_t g_lrotation_state = {0};    // 左旋转动作状态，记录左旋转动作的执行进度
ActionState_t g_rrotation_state = {0};    // 右旋转动作状态，记录右旋转动作的执行进度
ActionState_t g_swing_state = {0};        // 摇摆动作状态，记录摇摆动作的执行进度
ActionState_t g_swingtail_state = {0};    // 摇尾巴动作状态，记录摇尾巴动作的执行进度

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

/**
  * @brief  控制左上舵机角度
  * @param  Angle: 舵机角度(0-180度)
  * @retval None
  * @note   舵机控制原理：PWM占空比 = (角度/180)*2000 + 500
  *         500-2500us对应0-180度角度范围
  */
void Servo_1(float Angle)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Angle / 180 * 2000 + 500);
}

/**
  * @brief  控制右上舵机角度
  * @param  Angle: 舵机角度(0-180度)
  * @retval None
  * @note   右上舵机安装方向与左上相反，需要角度反转
  */
void Servo_2(float Angle)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (180-Angle) / 180 * 2000 + 500);
}

/**
  * @brief  控制左下舵机角度
  * @param  Angle: 舵机角度(0-180度)
  * @retval None
  */
void Servo_3(float Angle)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Angle / 180 * 2000 + 500);
}

/**
  * @brief  控制右下舵机角度
  * @param  Angle: 舵机角度(0-180度)
  * @retval None
  * @note   右下舵机安装方向与左下相反，需要角度反转
  */
void Servo_4(float Angle)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, (180-Angle) / 180 * 2000 + 500);
}

/**
  * @brief  控制尾巴舵机角度
  * @param  Angle: 舵机角度(0-180度)
  * @retval None
  * @note   使用TIM3通道1控制尾巴舵机
  */
void Servo_5(float Angle)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, Angle / 180 * 2000 + 500);
}


/**
  * @brief  同时设置四个腿部舵机角度
  * @param  angle1: 左上舵机角度(0-180度)
  * @param  angle2: 右上舵机角度(0-180度)
  * @param  angle3: 左下舵机角度(0-180度)
  * @param  angle4: 右下舵机角度(0-180度)
  * @retval None
  * @note   用于协调控制四个腿部舵机，实现整体动作
  *         该函数是静态函数，仅在action.c内部使用
  */
static void SetAllServos(float angle1, float angle2, float angle3, float angle4)
{
    Servo_1(angle1);  // 控制左上舵机
    Servo_2(angle2);  // 控制右上舵机
    Servo_3(angle3);  // 控制左下舵机
    Servo_4(angle4);  // 控制右下舵机
}






/**
  * @brief  舵机位置布局
  * @note   机器人腿部舵机的物理布局如下：
  *         
  *         1        2
  *         
  *         
  *         3        4
  *         
  *         其中：
  *         1 - 左上舵机
  *         2 - 右上舵机
  *         3 - 左下舵机
  *         4 - 右下舵机
  *         
  *         舵机角度说明：
  *         - 0度：腿向前进方向甩
  *         - 90度：站立状态
  *         - 180度：腿向后甩
  */

/**
  * @brief  放松趴下动作
  * @retval None
  * @note   使机器人处于放松的趴下状态
  *         左上和右上舵机角度为20度，左下和右下舵机角度为160度
  */
void Action_relaxed_getdowm(void)
{
	SetAllServos(20, 20, 160, 160);
}

/**
  * @brief  站立动作
  * @retval None
  * @note   使机器人处于站立状态
  *         所有腿部舵机角度均为90度
  */
void Action_upright(void)
{
	SetAllServos(90, 90, 90, 90);
}

/**
  * @brief  趴下动作
  * @retval None
  * @note   使机器人处于趴下状态
  *         所有腿部舵机角度均为20度
  */
void Action_getdowm(void)
{
	SetAllServos(20, 20, 20, 20);
}

/**
  * @brief  坐下动作
  * @retval None
  * @note   使机器人处于坐下状态
  *         左上和右上舵机角度为90度，左下和右下舵机角度为20度
  */
void Action_sit(void)
{
	SetAllServos(90, 90, 20, 20);
}
/**
  * @brief  摇摆动作
  * @retval None
  * @note   使机器人进行摇摆动作，所有腿部舵机同步在30-149度之间摆动
  *         该动作使用240个步骤完成一个完整的摇摆周期
  */
void Action_Swing(void)
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
	if (g_swing_state.current_step < 120) // 30->149度，逐渐增加角度
	{
		angle = 30 + g_swing_state.current_step;
	}
	else // 149->30度，逐渐减小角度
	{
		angle = 149 - (g_swing_state.current_step - 120);
	}
	
	// 设置舵机角度，所有腿部舵机使用相同的角度
	SetAllServos(angle, angle, angle, angle);
	
	// 进入下一步
	Action_NextStep(&g_swing_state);
}

/**
  * @brief  前进动作
  * @retval None
  * @note   使机器人执行前进动作，通过8个步骤的序列实现
  *         每个步骤控制四个腿部舵机的不同角度组合
  */
void Action_advance(void)
{
	// 初始化状态（仅在模式改变时）
	static uint8_t last_mode = 0xFF;
	if (last_mode != Action_Mode)
	{
		Action_InitState(&g_advance_state, 8, SpeedDelay); // 8个步骤，使用SpeedDelay作为延迟
		last_mode = Action_Mode;
	}
	
	// 检查是否应该执行下一步
	if (!Action_ShouldExecuteStep(&g_advance_state))
		return;
	
	// 前进动作序列
	static const float steps[][4] = {
		{ANGLE_FORWARD, ANGLE_STAND, ANGLE_STAND, ANGLE_FORWARD},      // 步骤1：左上和右下舵机向前，其他站立
		{ANGLE_FORWARD, ANGLE_BACKWARD, ANGLE_BACKWARD, ANGLE_FORWARD},    // 步骤2：左上和右下舵机向前，右上和左下舵机向后
		{ANGLE_STAND, ANGLE_BACKWARD, ANGLE_BACKWARD, ANGLE_STAND},          // 步骤3：右上和左下舵机向后，其他站立
		{ANGLE_STAND, ANGLE_STAND, ANGLE_STAND, ANGLE_STAND},          // 步骤4：所有舵机站立
		{ANGLE_STAND, ANGLE_FORWARD, ANGLE_FORWARD, ANGLE_STAND},      // 步骤5：右上和左下舵机向前，其他站立
		{ANGLE_BACKWARD, ANGLE_FORWARD, ANGLE_FORWARD, ANGLE_BACKWARD},    // 步骤6：右上和左下舵机向前，左上和右下舵机向后
		{ANGLE_BACKWARD, ANGLE_STAND, ANGLE_STAND, ANGLE_BACKWARD},          // 步骤7：左上和右下舵机向后，其他站立
		{ANGLE_STAND, ANGLE_STAND, ANGLE_STAND, ANGLE_STAND}           // 步骤8：所有舵机站立
	};
	
	// 执行当前步骤
	uint8_t step = g_advance_state.current_step;
	SetAllServos(steps[step][0], steps[step][1], steps[step][2], steps[step][3]);
	
	// 进入下一步
	Action_NextStep(&g_advance_state);
}

/**
  * @brief  后退动作
  * @retval None
  * @note   使机器人执行后退动作，通过8个步骤的序列实现
  *         每个步骤控制四个腿部舵机的不同角度组合
  */
void Action_back(void)
{
	// 初始化状态（仅在模式改变时）
	static uint8_t last_mode = 0xFF;
	if (last_mode != Action_Mode)
	{
		Action_InitState(&g_back_state, 8, SpeedDelay); // 8个步骤，使用SpeedDelay作为延迟
		last_mode = Action_Mode;
	}
	
	// 检查是否应该执行下一步
	if (!Action_ShouldExecuteStep(&g_back_state))
		return;
	
	// 后退动作序列
	static const float steps[][4] = {
		{ANGLE_BACKWARD, ANGLE_STAND, ANGLE_STAND, ANGLE_BACKWARD},    // 步骤1：左上和右下舵机向后，其他站立
		{ANGLE_BACKWARD, ANGLE_FORWARD, ANGLE_FORWARD, ANGLE_BACKWARD},      // 步骤2：左上和右下舵机向后，右上和左下舵机向前
		{ANGLE_STAND, ANGLE_FORWARD, ANGLE_FORWARD, ANGLE_STAND},          // 步骤3：右上和左下舵机向前，其他站立
		{ANGLE_STAND, ANGLE_STAND, ANGLE_STAND, ANGLE_STAND},          // 步骤4：所有舵机站立
		{ANGLE_STAND, ANGLE_BACKWARD, ANGLE_BACKWARD, ANGLE_STAND},    // 步骤5：右上和左下舵机向后，其他站立
		{ANGLE_FORWARD, ANGLE_BACKWARD, ANGLE_BACKWARD, ANGLE_FORWARD},      // 步骤6：右上和左下舵机向后，左上和右下舵机向前
		{ANGLE_FORWARD, ANGLE_STAND, ANGLE_STAND, ANGLE_FORWARD},          // 步骤7：左上和右下舵机向前，其他站立
		{ANGLE_STAND, ANGLE_STAND, ANGLE_STAND, ANGLE_STAND}           // 步骤8：所有舵机站立
	};
	
	// 执行当前步骤
	uint8_t step = g_back_state.current_step;
	SetAllServos(steps[step][0], steps[step][1], steps[step][2], steps[step][3]);
	
	// 进入下一步
	Action_NextStep(&g_back_state);
}

/**
  * @brief  向左旋转动作
  * @retval None
  * @note   使机器人执行向左旋转动作，通过4个步骤的序列实现
  *         每个步骤控制四个腿部舵机的不同角度组合
  */
void Action_Lrotation(void)
{
	// 初始化状态（仅在模式改变时）
	static uint8_t last_mode = 0xFF;
	if (last_mode != Action_Mode)
	{
		Action_InitState(&g_lrotation_state, 4, SpeedDelay); // 4个步骤，使用SpeedDelay作为延迟
		last_mode = Action_Mode;
	}
	
	// 检查是否应该执行下一步
	if (!Action_ShouldExecuteStep(&g_lrotation_state))
		return;
	
	// 向左旋转动作序列
	static const float steps[][4] = {
		{135, ANGLE_STAND, ANGLE_STAND, 45},     // 步骤1：左上舵机向后，右下舵机向前，其他站立
		{135, 135, 45, 45},     // 步骤2：左上和右上舵机向后，左下和右下舵机向前
		{ANGLE_STAND, 135, 45, ANGLE_STAND},          // 步骤3：右上舵机向后，左下舵机向前，其他站立
		{ANGLE_STAND, ANGLE_STAND, ANGLE_STAND, ANGLE_STAND}           // 步骤4：所有舵机站立
	};
	
	// 执行当前步骤
	uint8_t step = g_lrotation_state.current_step;
	SetAllServos(steps[step][0], steps[step][1], steps[step][2], steps[step][3]);
	
	// 进入下一步
	Action_NextStep(&g_lrotation_state);
}


/**
  * @brief  向右旋转动作
  * @retval None
  * @note   使机器人执行向右旋转动作，通过4个步骤的序列实现
  *         每个步骤控制四个腿部舵机的不同角度组合
  */
void Action_Rrotation(void)
{
	// 初始化状态（仅在模式改变时）
	static uint8_t last_mode = 0xFF;
	if (last_mode != Action_Mode)
	{
		Action_InitState(&g_rrotation_state, 4, SpeedDelay); // 4个步骤，使用SpeedDelay作为延迟
		last_mode = Action_Mode;
	}
	
	// 检查是否应该执行下一步
	if (!Action_ShouldExecuteStep(&g_rrotation_state))
		return;
	
	// 向右旋转动作序列
	static const float steps[][4] = {
		{ANGLE_FORWARD, 90, 90, ANGLE_BACKWARD},     // 步骤1：左上舵机向前，右下舵机向后，其他站立
		{45, ANGLE_FORWARD, ANGLE_BACKWARD, 135},     // 步骤2：左上和右上舵机向前，左下和右下舵机向后
		{ANGLE_STAND, 45, 135, ANGLE_STAND},          // 步骤3：右上舵机向前，左下舵机向后，其他站立
		{90, ANGLE_STAND, ANGLE_STAND, 90}           // 步骤4：所有舵机站立
	};
	
	// 执行当前步骤
	uint8_t step = g_rrotation_state.current_step;
	SetAllServos(steps[step][0], steps[step][1], steps[step][2], steps[step][3]);
	
	// 进入下一步
	Action_NextStep(&g_rrotation_state);
}

/**
  * @brief  摇尾巴动作
  * @retval None
  * @note   使机器人执行摇尾巴动作，通过240个步骤完成一个完整的摇摆周期
  *         尾巴舵机在30-149度之间摆动
  */
void Action_SwingTail(void)
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
	if (g_swingtail_state.current_step < 120) // 30->149度，逐渐增加角度
	{
		angle = 30 + g_swingtail_state.current_step;
	}
	else // 149->30度，逐渐减小角度
	{
		angle = 149 - (g_swingtail_state.current_step - 120);
	}
	
	// 设置尾巴舵机角度
	Servo_5(angle);
	
	// 进入下一步
	Action_NextStep(&g_swingtail_state);
}