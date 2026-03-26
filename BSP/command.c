#include "command.h"
#include "cmsis_os2.h"
#include "main.h"  // 包含main.h以访问CMDHandle

/* 全局变量定义 */
CommandType_t CurrentCommand = CMD_NONE;
/* 使用系统生成的CMD队列，无需额外定义 */

/**
  * @brief  发送指令到队列
  * @param  cmd: 指令类型
  * @retval None
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
  * @brief  获取当前指令
  * @param  None
  * @retval 当前指令类型
  */
CommandType_t GetCurrentCommand(void)
{
    return CurrentCommand;
}

/**
  * @brief  根据指令类型获取对应的数据
  * @param  cmd: 指令类型
  * @retval 指令数据指针，如果找不到返回NULL
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

