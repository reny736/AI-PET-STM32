/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_oled_debug.h"
#include "w25q64.h"
#include "tim.h"
#include "action.h"
#include "command.h"
#include "bsp_ota_boot.h"
#include "bsp_uart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
/* 串口接收完成标志 */
volatile uint8_t uart_rx_complete = 0;  // 用于指示串口接收是否完成
/* USER CODE END Variables */
/* FreeRTOS任务和队列定义 */

/* OLED显示任务定义 */
osThreadId_t OLEDHandle;  // OLED任务句柄
const osThreadAttr_t OLED_attributes = {
  .name = "OLED",           // 任务名称
  .stack_size = 128 * 4,     // 堆栈大小(512字节)
  .priority = (osPriority_t) osPriorityNormal1,  // 任务优先级
};
/* 动作控制任务定义 */
osThreadId_t ACTIONHandle;  // 动作任务句柄
const osThreadAttr_t ACTION_attributes = {
  .name = "ACTION",          // 任务名称
  .stack_size = 128 * 4,     // 堆栈大小(512字节)
  .priority = (osPriority_t) osPriorityNormal3,  // 任务优先级(高于OLED任务)
};
/* 串口通信任务定义 */
osThreadId_t UARTHandle;  // 串口任务句柄
const osThreadAttr_t UART_attributes = {
  .name = "UART",           // 任务名称
  .stack_size = 128 * 4,     // 堆栈大小(512字节)
  .priority = (osPriority_t) osPriorityNormal5,  // 任务优先级(最高)
};
/* 指令消息队列定义 */
osMessageQueueId_t CMDHandle;  // 指令队列句柄
const osMessageQueueAttr_t CMD_attributes = {
  .name = "CMD"  // 队列名称
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/**
  * @brief  串口调试打印函数
  * @param  fmt: 格式化字符串
  * @param  ...: 可变参数
  * @retval None
  * @note   用于通过串口输出调试信息，支持格式化字符串
  */
static void UART_DebugPrintf(const char *fmt, ...);

/* USER CODE END FunctionPrototypes */

void StartOLED(void *argument);
void StartAction(void *argument);
void StartUART(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of CMD */
  CMDHandle = osMessageQueueNew (16, sizeof(CommandType_t), &CMD_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* 使用系统生成的CMD队列，无需创建额外队列 */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of OLED */
  OLEDHandle = osThreadNew(StartOLED, NULL, &OLED_attributes);

  /* creation of ACTION */
  ACTIONHandle = osThreadNew(StartAction, NULL, &ACTION_attributes);

  /* creation of UART */
  UARTHandle = osThreadNew(StartUART, NULL, &UART_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

static void UART_DebugPrintf(const char *fmt, ...)
{
  char buffer[UART_TX_BUFFER_SIZE];
  va_list args;

  if (BSP_OTA_UART_IsQuietWindowActive()) {
    return;
  }

  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  BSP_UART_Send((uint8_t *)buffer, (uint16_t)strlen(buffer));
}

/* USER CODE BEGIN Header_StartOLED */
/**
  * @brief  Function implementing the OLED thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartOLED */
__weak void StartOLED(void *argument)
{
  /* USER CODE BEGIN StartOLED */
  static unsigned char face_buffer[1024]; // 表情数据缓冲区
  CommandType_t last_command = CMD_NONE; // 记录上次显示的指令
  
  // OLED初始化
  OLED_Init();
  OLED_CLS();
  
  // 显示初始表情
  W25Q64_Read_Data(BMP4_STORAGE_ADDR, face_buffer, FACE_SIZE);
  OLED_DrawBMP(0, 0, 128, 8, face_buffer);
  
  /* Infinite loop */
  for(;;)
  {
    // 检查当前指令状态
    CommandType_t current_cmd = GetCurrentCommand();
    
    // 只有当指令发生变化时才更新显示
    if (current_cmd != last_command)
    {
      last_command = current_cmd;
      
      if (current_cmd != CMD_NONE && current_cmd != CMD_STOP)
      {
        // 根据指令获取对应的表情数据
        const CommandData_t* cmd_data = GetCommandData(current_cmd);
        if (cmd_data && cmd_data->face_addr != 0)
        {
          // 读取并显示对应的表情
          W25Q64_Read_Data(cmd_data->face_addr, face_buffer, FACE_SIZE);
          OLED_CLS();
		  osDelay(100);
          OLED_DrawBMP(0, 0, 128, 8, face_buffer);
        }
      }
      else if (current_cmd == CMD_STOP || current_cmd == CMD_NONE)
      {
        // 停止显示，清屏，显示初始表情
        W25Q64_Read_Data(BMP4_STORAGE_ADDR, face_buffer, FACE_SIZE);
        OLED_CLS();
		osDelay(100);
        OLED_DrawBMP(0, 0, 128, 8, face_buffer);
      }
    }
    
    // 短暂延迟
    osDelay(100);
  }
  /* USER CODE END StartOLED */
}

/* USER CODE BEGIN Header_StartAction */
/**
* @brief Function implementing the ACTION thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAction */
__weak void StartAction(void *argument)
{
  /* USER CODE BEGIN StartAction */
  CommandType_t received_cmd;
  uint32_t action_start_time = 0;
  uint16_t action_duration = 0;
  CommandType_t current_processing_cmd = CMD_NONE;
  
  // 初始化舵机到站立位置
  Action_upright();
  osDelay(500);
  
  /* Infinite loop */
  for(;;)
  {
    // 非阻塞方式检查是否有新指令（不等待，立即返回）
    if (osMessageQueueGet(CMDHandle, &received_cmd, NULL, 0) == osOK)
    {
      UART_DebugPrintf("[ACTION] Got cmd: %d, current: %d\r\n", received_cmd, current_processing_cmd);
      
      // 如果收到停止命令
      if (received_cmd == CMD_STOP)
      {
        UART_DebugPrintf("[ACTION] Stopping current action\r\n");
        Action_Mode = 0;
        Action_upright();
        osDelay(100); // 确保动作停止
        current_processing_cmd = CMD_NONE;
        CurrentCommand = CMD_NONE;
      }
      // 如果收到新命令且与当前处理命令不同
      else if (received_cmd != current_processing_cmd)
      {
        UART_DebugPrintf("[ACTION] Switching to new action\r\n");
        // 先停止当前动作
        Action_Mode = 0;
        Action_upright();
        osDelay(100); // 确保动作停止
        
        // 设置新命令
        current_processing_cmd = received_cmd;
        CurrentCommand = received_cmd;
        
        // 获取命令数据并开始执行
        const CommandData_t* cmd_data = GetCommandData(current_processing_cmd);
        if (cmd_data)
        {
          Action_Mode = cmd_data->action_mode;
          action_duration = cmd_data->duration;
          action_start_time = osKernelGetTickCount();
          UART_DebugPrintf("[ACTION] Starting action mode: %d, duration: %d\r\n",
                           Action_Mode, action_duration);
        }
      }
      // 收到相同命令，重新开始计时
      else if (received_cmd == current_processing_cmd)
      {
        UART_DebugPrintf("[ACTION] Restarting current action timer\r\n");
        const CommandData_t* cmd_data = GetCommandData(current_processing_cmd);
        if (cmd_data)
        {
          action_start_time = osKernelGetTickCount();
        }
      }
    }
    
    // 执行当前动作
    if (current_processing_cmd != CMD_NONE && current_processing_cmd != CMD_STOP)
    {
      // 检查动作持续时间
      if (action_duration > 0 && 
          (osKernelGetTickCount() - action_start_time) >= action_duration)
      {
        // 动作时间到，停止动作
        UART_DebugPrintf("[ACTION] Action timeout, stopping\r\n");
        Action_Mode = 0;
        Action_upright();
        current_processing_cmd = CMD_NONE;
        CurrentCommand = CMD_NONE;
      }
      else
      {
        // 执行对应的动作（非阻塞式）
        switch(Action_Mode)
        {
          case MODE_ADVANCE: 
            Action_advance(); 
            break;
          case MODE_BACK: 
            Action_back(); 
            break;
          case MODE_LROTATION: 
            Action_Lrotation(); 
            break;
          case MODE_RROTATION: 
            Action_Rrotation(); 
            break;
          case MODE_SWING: 
            Action_Swing(); 
            break;
          case MODE_SWINGTAIL: 
            Action_SwingTail(); 
            break;
		  case MODE_SIT: 
            Action_sit(); 
            break;
		  case MODE_RELAX: 
            Action_relaxed_getdowm(); 
            break;
		  case MODE_DOWN: 
            Action_getdowm(); 
            break;
		  case MODE_STAND: 
            Action_upright(); 
            break;
          default: 
            UART_DebugPrintf("[ACTION] Unknown action mode: %d\r\n", Action_Mode);
            break;
        }
      }
    }
    
    // 短暂延迟，让出CPU时间片，同时允许其他任务执行
    osDelay(10);
  }
  /* USER CODE END StartAction */
}

/* USER CODE BEGIN Header_StartUART */
/**
* @brief Function implementing the UART thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUART */
void StartUART(void *argument)
{
  /* USER CODE BEGIN StartUART */
  // 串口任务变量
  uint8_t uart_rx_char;
  uint32_t last_status_time = 0;
  
  BSP_OTA_UART_Reset();
  
  // 发送欢迎信息
  UART_DebugPrintf("\r\n=== UART Task Started ===\r\n");
  UART_DebugPrintf("System Clock: %lu Hz\r\n", HAL_RCC_GetSysClockFreq());
  UART_DebugPrintf("Baud Rate: 115200\r\n");
  UART_DebugPrintf("Protocol: Single byte command (0x11, 0x22, ...)\r\n");
  UART_DebugPrintf("=========================\r\n\r\n");
  
  // 启动串口接收中断
  HAL_UART_Receive_IT(&huart1, &uart_rx_char, 1);
  
  /* Infinite loop */
  for(;;)
  {
    // 检查是否有数据接收完成
    if (uart_rx_complete)
    {
      uart_rx_complete = 0;

      BSP_OtaUartState_t ota_state = BSP_OTA_UART_HandleByte(uart_rx_char);
      if (ota_state == BSP_OTA_UART_MATCHED)
      {
        BSP_OTA_UART_EnterBootloader();
      }
      if (ota_state != BSP_OTA_UART_IDLE)
      {
        HAL_UART_Receive_IT(&huart1, &uart_rx_char, 1);
        continue;
      }
      
      UART_DebugPrintf("[RX] Received: 0x%02X\r\n", uart_rx_char);
      
      // 根据接收到的字节执行对应命令
      switch(uart_rx_char)
      {
        case 0x11: 
          UART_DebugPrintf("[CMD] Sending Happy command\r\n");
          SendCommand(CMD_HAPPY); 
          break;
        case 0x22: 
          UART_DebugPrintf("[CMD] Sending Sad command\r\n");
          SendCommand(CMD_SAD); 
          break;
        case 0x33: 
          UART_DebugPrintf("[CMD] Sending Angry command\r\n");
          SendCommand(CMD_ANGRY); 
          break;
        case 0x44: 
          UART_DebugPrintf("[CMD] Sending Surprise command\r\n");
          SendCommand(CMD_SURPRISE); 
          break;
        case 0xBB: 
          UART_DebugPrintf("[CMD] Sending Love command\r\n");
          SendCommand(CMD_LOVE); 
          break;
        case 0x66: 
          UART_DebugPrintf("[CMD] Sending Shy command\r\n");
          SendCommand(CMD_SHY); 
          break;
        case 0x77: 
          UART_DebugPrintf("[CMD] Sending Fear command\r\n");
          SendCommand(CMD_FEAR); 
          break;
        case 0x88: 
          UART_DebugPrintf("[CMD] Sending Relax command\r\n");
          SendCommand(CMD_RELAX); 
          break;
        case 0x99: 
          UART_DebugPrintf("[CMD] Sending Excited command\r\n");
          SendCommand(CMD_EXCITED); 
          break;
        case 0xCC: 
          UART_DebugPrintf("[CMD] Sending Down command\r\n");
          SendCommand(CMD_DOWN); 
          break;
        case 0xDD: 
          UART_DebugPrintf("[CMD] Sending Stop command\r\n");
          SendCommand(CMD_STOP); 
          break;
        default:
          UART_DebugPrintf("[CMD] Unknown command: 0x%02X\r\n", uart_rx_char);
          break;
      }
      
      // 重新启动接收中断
      HAL_UART_Receive_IT(&huart1, &uart_rx_char, 1);
    }
    
    // 定期发送状态信息（每15秒）
    if ((HAL_GetTick() - last_status_time > 15000) &&
        !BSP_OTA_UART_IsQuietWindowActive()) {
      last_status_time = HAL_GetTick();
      UART_DebugPrintf("[Status] UART Task Running, Tick: %lu, Free Heap: %u\r\n",
                       HAL_GetTick(), xPortGetFreeHeapSize());
    }
    
    // 短暂延时，让其他任务有机会运行
    osDelay(10);
  }
  /* USER CODE END StartUART */
}

/**
  * @brief  串口接收完成中断回调函数
  * @param  huart: 串口句柄
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        /* 设置接收完成标志 */
        uart_rx_complete = 1;
    }
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */