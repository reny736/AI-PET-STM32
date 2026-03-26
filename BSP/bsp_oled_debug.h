#ifndef __BSP_OLED_DEBUG_H
#define __BSP_OLED_DEBUG_H

#include "stm32f1xx.h"

/* 软件/硬件IIC切换宏 0：软件 1：硬件 */
#define IIC_SELECT 1

#define OLED_ID          0x78
#define OLED_WR_CMD      0x00
#define OLED_WR_DATA     0x40

#define FACE_SIZE          1024
#define BMP1_STORAGE_ADDR           0x000000  
#define BMP2_STORAGE_ADDR           0x000400  // 表情1
#define BMP3_STORAGE_ADDR           0x000800  // 表情2
#define BMP4_STORAGE_ADDR           0x000C00  // 表情3
#define BMP5_STORAGE_ADDR           0x001000  // 表情4
#define BMP6_STORAGE_ADDR           0x001400  // 表情5
#define BMP7_STORAGE_ADDR           0x001800  // 表情6
#define BMP8_STORAGE_ADDR           0x001C00  // 表情7
#define BMP9_STORAGE_ADDR           0x002000  // 表情8
#define BMP10_STORAGE_ADDR          0x002400  // 表情9
#define BMP11_STORAGE_ADDR          0x002800  // 表情10
#define BMP12_STORAGE_ADDR          0x002C00  // 表情11
#define BMP13_STORAGE_ADDR          0x003000  // 表情12
#define BMP14_STORAGE_ADDR          0x003400  // 表情13
#define BMP15_STORAGE_ADDR          0x003800  // 表情14
#define BMP16_STORAGE_ADDR          0x003C00  // 表情15
#define BMP17_STORAGE_ADDR          0x004000  // 表情16
#define BMP18_STORAGE_ADDR          0x004400  // 表情17


void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char textsize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char n);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_ShowCN31x31(unsigned char x, unsigned char y, unsigned char n);
#endif
