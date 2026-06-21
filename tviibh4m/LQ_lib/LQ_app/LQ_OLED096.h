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
#ifndef _LQOLED_H
#define _LQOLED_H

#include <stdint.h>


void OLED_Init(void);			//OLED初始化
void OLED_Fill(void);			//OLED全亮
void OLED_CLS(void);      //OLED清屏
void OLED_PutPixel(unsigned char x,unsigned char y);      //画一个点
void OLED_ClrDot(unsigned char x,unsigned char y);        //清除8个点
void OLED_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif);//画矩形
void OLED_P6x8Str(unsigned char x,unsigned char y, char ch[]);                                             //写入一组标准ASCII字符串
void OLED_P8x16Str(unsigned char x,unsigned char y, char ch[]);                                            //写入一组标准ASCII字符串
void OLED_P14x16Str(unsigned char x,unsigned char y,unsigned char ch[]);                                   //输出汉字字符串
void OLED_Show_BMP(unsigned char x0,unsigned char y0,unsigned char wide,unsigned char high,const unsigned char * bmp);//显示BMP图片 最大 128×64
void OLED_Show_LQLogo(void);                                   //显示龙邱图片128×64
void OLED_Show_Frame80(void);                                  //画边线 OV7725显示图像时 可以用这个画边线
void OLED_Show_Frame94(void);                                  //画边线 MT9V034显示图像时 可以用这个画边线
void OLED_Road(unsigned short high, unsigned short wide, unsigned char *Pixle);//OLED 二值化图像显示
void OLED_WrCmd(unsigned char cmd);                 //OLED命令
void OLED_Set_Pos(unsigned char x, unsigned char y);//OLED设置点
void Test_OLED(void);                               //OLED 测试函数

#endif


