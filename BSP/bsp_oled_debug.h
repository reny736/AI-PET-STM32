/**
  * @file    bsp_oled_debug.h
  * @brief   OLED显示驱动头文件
  * @details 该文件包含了OLED显示屏的初始化、绘图、显示字符/汉字/图片的函数声明
  *          以及表情图片存储地址的定义。OLED用于显示机器人的表情。
  * @author  HAL Team
  */
#ifndef __BSP_OLED_DEBUG_H
#define __BSP_OLED_DEBUG_H

#include "stm32f1xx.h"

/* IIC通信方式选择宏: 0-软件IIC, 1-硬件IIC */
#define IIC_SELECT 1

/* OLED器件地址定义 */
#define OLED_ID          0x78    // OLED从机地址(7位)
#define OLED_WR_CMD      0x00    // 写命令标志
#define OLED_WR_DATA     0x40    // 写数据标志

/* 表情图片存储参数定义 */
#define FACE_SIZE          1024    // 单个表情图片大小 (128x64/8 = 1024 bytes)

/* 表情图片在Flash中的存储地址定义
 * 每个表情占用1024字节，地址间隔0x400 (1024)
 * BMP1作为默认起始地址
 */
#define BMP1_STORAGE_ADDR           0x000000  // 表情1地址
#define BMP2_STORAGE_ADDR           0x000400  // 表情2地址
#define BMP3_STORAGE_ADDR           0x000800  // 表情3地址
#define BMP4_STORAGE_ADDR           0x000C00  // 表情4地址
#define BMP5_STORAGE_ADDR           0x001000  // 表情5地址
#define BMP6_STORAGE_ADDR           0x001400  // 表情6地址
#define BMP7_STORAGE_ADDR           0x001800  // 表情7地址
#define BMP8_STORAGE_ADDR           0x001C00  // 表情8地址
#define BMP9_STORAGE_ADDR           0x002000  // 表情9地址
#define BMP10_STORAGE_ADDR          0x002400  // 表情10地址
#define BMP11_STORAGE_ADDR          0x002800  // 表情11地址
#define BMP12_STORAGE_ADDR          0x002C00  // 表情12地址
#define BMP13_STORAGE_ADDR          0x003000  // 表情13地址
#define BMP14_STORAGE_ADDR          0x003400  // 表情14地址
#define BMP15_STORAGE_ADDR          0x003800  // 表情15地址
#define BMP16_STORAGE_ADDR          0x003C00  // 表情16地址
#define BMP17_STORAGE_ADDR          0x004000  // 表情17地址
#define BMP18_STORAGE_ADDR          0x004400  // 表情18地址

/* 函数声明 */

/**
  * @brief  OLED初始化函数
  * @param  None
  * @retval None
  * @note   初始化OLED显示屏，设置对比度、显示模式、滚动等参数
  */
void OLED_Init(void);

/**
  * @brief  设置OLED显示位置
  * @param  x: 列位置 (0-127)
  * @param  y: 页位置 (0-7)
  * @retval None
  * @note   OLED屏幕分为128列、8页，每页8行像素
  */
void OLED_SetPos(unsigned char x, unsigned char y);

/**
  * @brief  全屏填充函数
  * @param  fill_data: 填充数据 (0x00清屏, 0xFF全亮)
  * @retval None
  */
void OLED_Fill(unsigned char fill_data);

/**
  * @brief  清屏函数
  * @param  None
  * @retval None
  * @note   将整个屏幕所有像素设置为0
  */
void OLED_CLS(void);

/**
  * @brief  开启OLED显示
  * @param  None
  * @retval None
  * @note   从休眠模式唤醒OLED
  */
void OLED_ON(void);

/**
  * @brief  关闭OLED显示
  * @param  None
  * @retval None
  * @note   进入休眠模式，功耗降至最低
  */
void OLED_OFF(void);

/**
  * @brief  显示ASCII字符串
  * @param  x: 起始列位置 (0-127)
  * @param  y: 起始页位置 (0-7)
  * @param  ch[]: 要显示的字符串
  * @param  textsize: 字符大小 (1: 6x8点阵, 2: 8x16点阵)
  * @retval None
  */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char textsize);

/**
  * @brief  显示16x16点阵汉字
  * @param  x: 起始列位置 (0-127)
  * @param  y: 起始页位置 (0-7)
  * @param  n: 汉字在字库中的索引
  * @retval None
  */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char n);

/**
  * @brief  显示128x64位图
  * @param  x0: 起始列位置
  * @param  y0: 起始页位置
  * @param  x1: 结束列位置
  * @param  y1: 结束页位置
  * @param  BMP[]: 位图数据数组
  * @retval None
  * @note   用于显示存储在Flash中的表情图片
  */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);

/**
  * @brief  显示31x31点阵汉字
  * @param  x: 起始列位置
  * @param  y: 起始页位置
  * @param  n: 汉字在字库中的索引
  * @retval None
  */
void OLED_ShowCN31x31(unsigned char x, unsigned char y, unsigned char n);
#endif
