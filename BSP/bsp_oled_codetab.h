/**
  * @file    bsp_oled_codetab.h
  * @brief   OLED字模数据头文件
  * @details 该文件声明了OLED显示所使用的字模数据，包括16x16点阵汉字库(F16x16)、
  *          6x8ASCII字符集(F6x8)以及8x16ASCII字符集(F8X16)
  * @author  HAL Team
  */

#ifndef __BSP_OLED_CODETAB_H
#define __BSP_OLED_CODETAB_H


/* 16x16点阵汉字库数组
 * 每个汉字使用32字节表示(16行x16列)
 * 可通过OLED_ShowCN()函数调用显示
 */
extern unsigned char F16x16[];

/* 6x8 ASCII字符集数组
 * 每个字符使用6字节表示(6列x8行)
 * 可通过OLED_ShowStr()函数调用显示textsize=1时使用
 */
extern const unsigned char F6x8[][6];

/* 8x16 ASCII字符集数组
 * 每个字符使用16字节表示(8列x16行)
 * 可通过OLED_ShowStr()函数调用显示textsize=2时使用
 */
extern const unsigned char F8X16[];

#endif
