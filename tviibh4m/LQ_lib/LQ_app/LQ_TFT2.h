/*******************************************************************************
LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技CYT4BB7核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V0.1版权所有，单位使用请先联系授权
【最后更新】2025年2月24日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】IAR Systems 9.40.1版本
【Target 】 CYT4BB7(Dual Core M7, cFlash:4MB wFlash:256KB RAM:758KB  144Pins)
【Crystal】 16.000Mhz
【SYS PLL】 100MHz + 250MHz + 250MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
*******************************************************************************/
#ifndef __LQ_TFT2_TFT2SPI_H__
#define __LQ_TFT2_TFT2SPI_H__

#include "stdint.h"

#define TFT20W        220
#define TFT20H        176

#define u16RED		0xf800
#define u16GREEN	0x07e0
#define u16BLUE	        0x001f
#define u16PURPLE	0xf81f
#define u16YELLOW	0xffe0
#define u16CYAN	        0x07ff 		
#define u16ORANGE	0xfc08
#define u16BLACK	0x0000
#define u16WHITE	0xffff


void TFT2SPI_Init(unsigned char type);

void TFT2SPI_Write_Cmd(unsigned char cmd);

void TFT2SPI_Write_Byte(unsigned char dat);

void TFT2SPI_Write_Word(unsigned short dat);

void TFT2SPI_Set_Pos(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye);

void TFT2SPI_Fill_Area(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color);

void TFT2SPI_CLS(unsigned short color);

void TFT2SPI_Draw_Part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

void TFT2SPI_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

void TFT2SPI_Draw_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned short color_dat);

void TFT2SPI_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);

void TFT2SPI_Draw_Dot(unsigned char x,unsigned char y,unsigned short color_dat);

void TFT2SPI_Show_Pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,const unsigned char *ppic);

void TFT2SPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);

void TFT2SPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);

void TFT2SPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);


void TFT2SPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned short word_color,unsigned short back_color);

void TFT2SPI_Show_Pic2(unsigned char xs,unsigned char ys,unsigned char w,unsigned char h,const unsigned char *ppic) ;

void TFT2SPI_Show_Battery_Icon(void);

void TFT2SPI_Show_Title(void);

void TFT2SPI_Show_Logo(unsigned char xs,unsigned char ys);

void TFT2SPI_Road(unsigned char high_start, unsigned char wide_start, unsigned char high, unsigned char wide, unsigned char *Pixle);

void TFT2SPI_BinRoad(unsigned char high_start, unsigned char wide_start, unsigned char high, unsigned char wide, unsigned char *Pixle);
void write_command(unsigned char LCD_Reg,uint16_t LCD_RegValue) ;

void Test_TFT2(void);


#endif /*SGP18T_ILI9163B.h*/
