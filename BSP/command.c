#include "command.h"
#include "cmsis_os2.h"
#include "main.h"  // 包含main.h以访问CMDHandle

/* 全局变量定义 */
CommandType_t CurrentCommand = CMD_NONE;  // 当前执行的指令类型，初始为无指令
/* 使用系统生成的CMD队列，无需额外定义 */

/**
  * @brief  发送指令到FreeRTOS消息队列
  * @param  cmd: 指令类型
  * @retval None
  * @note   将指令发送到系统消息队列，由动作任务处理
  *         使用1000ms超时，防止队列满时阻塞
  */
void SendCommand(CommandType_t cmd)
{
    extern osMessageQueueId_t CMDHandle;  // 声明外部CMD队列
    
    if (CMDHandle != NULL)
    {
        CommandType_t cmd_value = cmd;
        osMessageQueuePut(CMDHandle, &cmd_value, 0, 1000);
    }
}

/**
  * @brief  获取当前正在执行的指令
  * @param  None
  * @retval 当前指令类型
  * @note   返回全局变量CurrentCommand的值
  */
CommandType_t GetCurrentCommand(void)
{
    return CurrentCommand;
}

/**
  * @brief  根据指令类型在映射表中查找对应的数据
  * @param  cmd: 指令类型
  * @retval 指令数据指针，如果找不到返回NULL
  * @note   遍历CommandMap数组，查找匹配的指令类型
  *         返回对应的表情地址、动作模式和持续时间
  */
const CommandData_t* GetCommandData(CommandType_t cmd)
{
    for (int i = 0; i < sizeof(CommandMap) / sizeof(CommandMap[0]); i++)
    {
        if (CommandMap[i].type == cmd)
        {
            return &CommandMap[i];
        }
    }
    return NULL;
}