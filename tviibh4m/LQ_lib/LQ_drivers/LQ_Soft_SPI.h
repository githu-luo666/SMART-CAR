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
#ifndef __LQ_Soft_SPI_H_
#define __LQ_Soft_SPI_H_
#include "include.h"

//                   单片机  ---------     龙邱模块
#define Soft_SPI_SCK_PIN      P01_0     //SCK
#define Soft_SPI_MISO_PIN     P00_2     //单片机MISO接模块的--SDO
#define Soft_SPI_MOSI_PIN     P00_3     //单片机MOSI接模块的--SDI
#define Soft_SPI_CS_PIN       P01_1     //CS

#define Soft_SPI_SCK_OUT_H    LQ_Pin_Write(Soft_SPI_SCK_PIN, 1)        //配置输出高电平
#define Soft_SPI_SCK_OUT_L    LQ_Pin_Write(Soft_SPI_SCK_PIN, 0)        //配置输出低电平

#define Soft_SPI_MISO_IN      LQ_Pin_Read_In(Soft_SPI_MISO_PIN)            //读取引脚上的引脚状态
//#define Soft_SPI_MISO_IN      (GPIO_PRT0->unIN.u32Register& 0x04)            //读取引脚上的引脚状态

#define Soft_SPI_MOSI_OUT_H   LQ_Pin_Write(Soft_SPI_MOSI_PIN, 1)       //配置输出高电平
#define Soft_SPI_MOSI_OUT_L   LQ_Pin_Write(Soft_SPI_MOSI_PIN, 0)       //配置输出低电平

#define Soft_SPI_CS_OUT_H     LQ_Pin_Write(Soft_SPI_CS_PIN, 1)         //配置输出高电平
#define Soft_SPI_CS_OUT_L     LQ_Pin_Write(Soft_SPI_CS_PIN, 0)         //配置输出低电平

void Soft_SPI_Init(void);
void Soft_SPI_ReadWriteNbyte(unsigned char *lqbuff, unsigned char len);
void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf);
void LQ_SPI_Write(unsigned char reg, unsigned char value);
#endif

















