#ifndef __COMMAND_H
#define __COMMAND_H

#include "main.h"
#include "bsp_oled_debug.h"
#include "action.h"

/* 指令类型定义 */
typedef enum {
    CMD_NONE = 0,           // 无指令
    CMD_HAPPY = 1,          // 开心表情 + 前进动作
    CMD_SAD = 2,            // 悲伤表情 + 坐下动作
    CMD_ANGRY = 3,          // 生气表情 + 站立动作
    CMD_SURPRISE = 4,       // 惊讶表情 + 左旋转动作
    CMD_LOVE = 5,           // 爱心表情 + 摇尾巴动作
    CMD_SHY = 6,       // 害羞表情 + 右旋转动作
    CMD_FEAR = 7,       // 害怕表情 + 后退动作
    CMD_RELAX = 8,       // 放松表情 + 放松动作
    CMD_EXCITED = 9,       // 激动表情 + 摇摆动作
	CMD_DOWN = 10,         //悠闲表情+趴下动作
    CMD_STOP = 11           // 停止动作
} CommandType_t;

/* 指令数据结构 */
typedef struct {
    CommandType_t type;      // 指令类型
    uint32_t face_addr;      // 表情存储地址
    uint8_t action_mode;    // 动作模式
    uint16_t duration;       // 持续时间(ms)
} CommandData_t;

/* 指令与表情/动作映射表 */
static const CommandData_t CommandMap[] = {
    {CMD_HAPPY,     BMP3_STORAGE_ADDR,  MODE_ADVANCE,    0},  // 前进动作，5秒后停止
    {CMD_SAD,       BMP6_STORAGE_ADDR,  MODE_SIT,        0},     // 坐下动作，持续执行
    {CMD_ANGRY,     BMP9_STORAGE_ADDR,  MODE_STAND,      0},     // 站立动作，持续执行
    {CMD_SURPRISE,  BMP13_STORAGE_ADDR,  MODE_LROTATION,  0},  // 左旋转，5秒后停止
    {CMD_LOVE,      BMP7_STORAGE_ADDR,  MODE_SWINGTAIL,  0},  // 摇尾巴，5秒后停止
    {CMD_SHY,  BMP16_STORAGE_ADDR,  MODE_RROTATION,    0},  // 右旋转，5秒后停止
    {CMD_FEAR,  BMP5_STORAGE_ADDR,  MODE_BACK,  0},  // 后退，5秒后停止
    {CMD_RELAX,  BMP2_STORAGE_ADDR,  MODE_RELAX,      0},     // 放松，持续执行
    {CMD_EXCITED,  BMP13_STORAGE_ADDR, MODE_SWING,  0},  // 摇摆，5秒后停止
	{CMD_DOWN,  BMP15_STORAGE_ADDR, MODE_DOWN,  0},     // 趴下，持续执行
    {CMD_STOP,      0,                  0,               0}      // 停止命令
};

/* 全局变量声明 */
extern CommandType_t CurrentCommand;
/* 使用系统生成的CMD队列，无需额外声明 */

/* 函数声明 */
void SendCommand(CommandType_t cmd);
CommandType_t GetCurrentCommand(void);
const CommandData_t* GetCommandData(CommandType_t cmd);

#endif /* __COMMAND_H */

