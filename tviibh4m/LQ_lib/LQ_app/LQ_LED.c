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
#include "LQ_LED.h"

/*************************************************************************
*  函数名称：void LED_Init(void)
*  功能说明：GPIO初始化函数 LED灯所用P05.4 P08.3 P19.4初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月22日
*  备  注：
*************************************************************************/
void GPIO_LED_Init(void)
{
  // 初始化,输入口，高电平
  LQ_GPIO_Init_Config(LED0p, GPIO_OUT_STRONG);
  LQ_GPIO_Init_Config(LED1p, GPIO_OUT_STRONG);
  LQ_GPIO_Init_Config(LED2p, GPIO_OUT_STRONG);
}

/*************************************************************************
*  函数名称：void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
*  功能说明：LED控制
*  参数说明：LEDn_e LEDno编号,LEDs_e sta状态亮/灭/翻转
*  函数返回：按键状态，0/1
*  修改时间：2023年11月22日
*  备    注：
*************************************************************************/
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
{
  switch(LEDno)
  {
    case LED0:
      if(sta==ON)       LQ_Pin_Write(LED0p,0);
      else if(sta==OFF) LQ_Pin_Write(LED0p,1);
      else if(sta==RVS) LQ_Pin_Inv(LED0p);
      break;

    case LED1:
      if(sta==ON)       LQ_Pin_Write(LED1p,0);
      else if(sta==OFF) LQ_Pin_Write(LED1p,1);
      else if(sta==RVS) LQ_Pin_Inv(LED1p);
      break;

    case LED2:
      if(sta==ON)       LQ_Pin_Write(LED2p,0);
      else if(sta==OFF) LQ_Pin_Write(LED2p,1);
      else if(sta==RVS) LQ_Pin_Inv(LED2p);
      break;

    case LEDALL:
      if(sta==ON)
      {
        LQ_Pin_Write(LED0p,0);
        LQ_Pin_Write(LED1p,0);
        LQ_Pin_Write(LED2p,0);
      }
      else if(sta==OFF)
      {
        LQ_Pin_Write(LED0p,1);
        LQ_Pin_Write(LED1p,1);
        LQ_Pin_Write(LED2p,1);
      }
      else if(sta==RVS)
      {
        LQ_Pin_Inv(LED0p);
        LQ_Pin_Inv(LED1p);
        LQ_Pin_Inv(LED2p);
      }
      break;
    default:
      break;
  }
}
/*************************************************************************
*  函数名称：void Test_GPIO_LED(void)
*  功能说明：测试核心板LED程序
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月22日
*  备    注：核心板上的LED灯闪烁--3个LED同时闪烁
*************************************************************************/
void Test_LED(void)
 {
  GPIO_LED_Init();
  while(1)
  {
    LED_Ctrl(LEDALL,RVS);        //3个LED同时闪烁
    Cy_SysTick_DelayInUs(100000);       //延时等待100ms
  }
 }

/*************************************************************************
*  函数名称：void Test_buzzer(void)
*  功能说明：测试程序
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月22日
*  备    注：核心板上的LED灯闪烁--3个LED同时闪烁
*************************************************************************/
void Test_buzzer(void)
{
  Buzzer_Init();
  while(1)
  {
    Buzzer_OFF;
    Cy_SysTick_DelayInUs(1000000);       //延时等待1s
    Buzzer_ON;
    Cy_SysTick_DelayInUs(1000000);       //延时等待1s
    
  }
}





















