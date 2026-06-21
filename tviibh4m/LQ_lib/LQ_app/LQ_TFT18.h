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
#ifndef _LQ_TFT18_H_
#define _LQ_TFT18_H_	

#include "stdint.h"
//#include "LQ_SPI.h"
/*******************接口定义******************************/

#define APP_LCD_WIDTH  LCD_WIDTH//液晶屏宽度
#define APP_LCD_HEIGHT LCD_HEIGHT//液晶屏高度

#define TFT18W        162
#define TFT18H        132

#define u16RED		0xf800
#define u16GREEN	0x07e0
#define u16BLUE	        0x001f
#define u16PURPLE	0xf81f
#define u16YELLOW	0xffe0
#define u16CYAN	        0x07ff 		//蓝绿色
#define u16ORANGE	0xfc08
#define u16BLACK	0x0000
#define u16WHITE	0xffff

void TFTSPI_Init(unsigned char type);
void TFTSPI_Write_Cmd(unsigned char cmd);
void TFTSPI_Write_Byte(unsigned char dat);
void TFTSPI_Write_Word(unsigned short dat);
void TFTSPI_Set_Pos(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye);
void TFTSPI_Fill_Area(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color);
void TFTSPI_CLS(unsigned short color);
void TFTSPI_Draw_Dot(unsigned char x,unsigned char y,unsigned short color_dat);
void TFTSPI_Draw_Part(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
void TFTSPI_Draw_Rectangle(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
void TFTSPI_Draw_Circle(unsigned char x,unsigned char y,unsigned char r,unsigned short color_dat);
void TFTSPI_Draw_Line(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,unsigned short color_dat);
void TFTSPI_Show_Pic(unsigned char xs,unsigned char ys,unsigned char xe,unsigned char ye,const unsigned char *ppic);
void TFTSPI_P6X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P8X8Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P8X16Str(unsigned char x, unsigned char y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P16x16Str(unsigned char x, unsigned char y, unsigned char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_Show_Pic2(unsigned char xs,unsigned char ys,unsigned char w,unsigned char h,const unsigned char *ppic) ;
void TFTSPI_Show_Battery_Icon(void);
void TFTSPI_Show_Title(void);
void TFTSPI_Show_Logo(unsigned char xs,unsigned char ys);
void TFTSPI_Road( unsigned char wide_start, unsigned char high_start,unsigned char high, unsigned char wide, unsigned char *Pixle);
void TFTSPI_BinRoad( unsigned char wide_start, unsigned char high_start,unsigned char high, unsigned char wide, unsigned char *Pixle);
void TFTSPI_ShowBeacon(void );
void Test_TFT18(void);

extern volatile int test_num;

#endif /*SGP18T_ILI9163B.h*/
