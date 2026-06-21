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
此为144引脚GPIO分布
*******************************************************************************/
#ifndef _LQ_GPIO_H_
#define _LQ_GPIO_H_

#include <stddef.h>
#include <stdint.h>
#include "syslib/cy_syslib.h"
#include "cy_device_headers.h"
#include "gpio/cy_gpio.h"
#include "sysint/cy_sysint.h"

#define  GPIO_PIN_RESET  0
#define  GPIO_PIN_SET    1

extern volatile stc_GPIO_PRT_t* PORTX[33];
/* 获取GPIO对应 MODULE 宏定义L */
#define PIN_GetModule(GPIO_NAME) ((GPIO_NAME >>4)& 0xFF)

/* 获取GPIO对应 管脚序列号 宏定义Q */
#define PIN_GetIndex(GPIO_NAME)   (GPIO_NAME & 0x00F)

typedef enum		// 枚举引脚模式
{
  GPIO_ANALOG,          //模拟输入
  GPIO_OUT_PP_UP,       //上拉推挽输出
  GPIO_OUT_PP_DOWN,     //下拉推挽输出
  GPIO_OUT_OD_LOW,      //下拉开漏输出
  GPIO_OUT_OD_HIGH,     //上拉开漏输出
  GPIO_OUT_STRONG,      //强输出模式（LED、蜂鸣器等输出用此模式）
  GPIO_OUT_UP_DOWN,     //电阻上下拉（）
  GPIO_IN_HIGHZ,        //高阻态输入（可能是这意思，按键输入等用此模式）
  GPIO_IN_UP,           //上拉输入
  GPIO_IN_DOWN,         //下拉输入
  GPIO_IN_OD_LOW,
  GPIO_IN_OD_HIGH,
  GPIO_IN_STRONG,       //强力输入？
  GPIO_IN_UP_DOWN,
  
}GPIO_MODE_t;

typedef enum //
{
    P00_0 = 0x000,  P00_1 = 0x001,  P00_2 = 0x002,  P00_3 = 0x003,
    
    P01_0 = 0x010,  P01_1 = 0x011,
    
    P02_0 = 0x020,  P02_1 = 0x021,  P02_2 = 0x022,  P02_3 = 0x023,  P02_4 = 0x024,
    
    P03_0 = 0x030,  P03_1 = 0x031,  P03_2 = 0x032,  P03_3 = 0x033,  P03_4 = 0x034,
    
    P04_0 = 0x040,  P04_1 = 0x041,
    
    P05_0 = 0x050,  P05_1 = 0x051,  P05_2 = 0x052,  P05_3 = 0x053,  P05_4 = 0x054,
    
    P06_0 = 0x060,  P06_1 = 0x061,  P06_2 = 0x062,  P06_3 = 0x063,  P06_4 = 0x064,  P06_5 = 0x065,  P06_6 = 0x066,  P06_7 = 0x067,
    
    P07_0 = 0x070,  P07_1 = 0x071,  P07_2 = 0x072,  P07_3 = 0x073,  P07_4 = 0x074,  P07_5 = 0x075,  P07_6 = 0x076,  P07_7 = 0x077,
    
    P08_0 = 0x080,  P08_1 = 0x081,  P08_2 = 0x082,  P08_3 = 0x083,
    
    P09_0 = 0x090,  P09_1 = 0x091,
    
    P10_0 = 0x0A0,  P10_1 = 0x0A1,  P10_2 = 0x0A2,  P10_3 = 0x0A3,  P10_4 = 0x0A4,
    
    P11_0 = 0x0B0,  P11_1 = 0x0B1,  P11_2 = 0x0B2,
    
    P12_0 = 0x0C0,  P12_1 = 0x0C1,  P12_2 = 0x0C2,  P12_3 = 0x0C3,  P12_4 = 0x0C4,  P12_5 = 0x0C5,
    
    P13_0 = 0x0D0,  P13_1 = 0x0D1,  P13_2 = 0x0D2,  P13_3 = 0x0D3,  P13_4 = 0x0D4,  P13_5 = 0x0D5,  P13_6 = 0x0D6,  P13_7 = 0x0D7,
    
    P14_0 = 0x0E0,  P14_1 = 0x0E1,                                  P14_4 = 0x0E4,  P14_5 = 0x0E5,
    
    P15_0 = 0x0F0,  P15_1 = 0x0F1,  P15_2 = 0x0F2,
    
    
    
    P17_0 = 0x110,  P17_1 = 0x111,  P17_2 = 0x112,  P17_3 = 0x113,  P17_4 = 0x114,
    
    P18_0 = 0x120,  P18_1 = 0x121,  P18_2 = 0x122,  P18_3 = 0x123,  P18_4 = 0x124,  P18_5 = 0x125,  P18_6 = 0x126,  P18_7 = 0x127, 
    
    P19_0 = 0x130,  P19_1 = 0x131,  P19_2 = 0x132,  P19_3 = 0x133,  P19_4 = 0x134,
    
    P20_0 = 0x140,  P20_1 = 0x141,  P20_2 = 0x142,  P20_3 = 0x143,
    
    P21_0 = 0x150,  P21_1 = 0x151,  P21_2 = 0x152,  P21_3 = 0x153,                  P21_5 = 0x155,  P21_6 = 0x156,  
    
                    P22_1 = 0x161,  P22_2 = 0x162,  P22_3 = 0x163,  P22_4 = 0x164,  P22_5 = 0x165,  P22_6 = 0x166,
    
    P23_0 = 0x170,  P23_1 = 0x171,  P23_2 = 0x172,  P23_3 = 0x173,  P23_4 = 0x174,  P23_5 = 0x175,  P23_6 = 0x176,  P23_7 = 0x177,

}GPIO_Name_t;

typedef enum
{
  LQ_GPIO_INTR_DISABLE  = 0x00,         //0x00ul禁止中断0x00ul
  LQ_GPIO_INTR_RISING   = 0x01,         //上升沿触发
  LQ_GPIO_INTR_FALLING  = 0x02,         //下降沿触发
  LQ_GPIO_INTR_BOTH     = 0x03          //双边沿触发
    
}GPIO_ExitMode_t;

void LQ_GPIO_Init_Config(GPIO_Name_t pin, GPIO_MODE_t dir);
void LQ_Pin_Write(GPIO_Name_t pin, uint8_t dir);
void LQ_Pin_Inv(GPIO_Name_t pin);
uint32_t LQ_Pin_Read_Out(GPIO_Name_t pin);
uint32_t LQ_Pin_Read_In(GPIO_Name_t pin);

void LQ_GPIO_Exit_InitConfig(GPIO_Name_t pin, GPIO_ExitMode_t mode);
void LQ_GPIO_ClearInterrupt(GPIO_Name_t pin);
uint32_t LQ_GPIO_GetInterruptStatusMasked(GPIO_Name_t pin);


#endif






