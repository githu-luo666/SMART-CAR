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
#ifndef _LQ_LED_H_
#define _LQ_LED_H_

#include "LQ_GPIO.h"
#include "systick/cy_systick.h"


//定义模块号
typedef enum
{
    LED0=0,  //核心板LED0         DG灯
    LED1=1,  //核心板LED1         DB灯
    LED2=2,  //核心板LED2         DR灯
    LEDALL=4
} LEDn_e;

typedef enum
{
    ON=0,  //开
    OFF=1, //关
    RVS=2, //闪烁
}LEDs_e;

//定义的管脚要对应实际按键
#define LED0p      P05_4   //核心板LED0         DG灯
#define LED1p      P08_3   //核心板LED1         DB灯
#define LED2p      P19_4   //核心板LED2         DR灯
#define buzzer     P03_0   //蜂鸣器        

#define Buzzer_Init()   LQ_GPIO_Init_Config(buzzer, GPIO_OUT_STRONG)
#define Buzzer_ON       LQ_Pin_Write(buzzer, 1)
#define Buzzer_OFF      LQ_Pin_Write(buzzer, 0)

void GPIO_LED_Init(void);
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta);
void Test_LED(void);
void Test_buzzer(void);








#endif




