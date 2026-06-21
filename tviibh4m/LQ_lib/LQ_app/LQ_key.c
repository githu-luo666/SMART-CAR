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
#include "LQ_key.h"
#include "include.h"

/*************************************************************************
*  函数名称：void GPIO_KEY_Init(void)
*  功能说明：按键初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void GPIO_KEY_Init(void)
{
  // 引脚初始化
  LQ_GPIO_Init_Config(KEY0p, GPIO_IN_HIGHZ);
  LQ_GPIO_Init_Config(KEY1p, GPIO_IN_HIGHZ);
  LQ_GPIO_Init_Config(KEY2p, GPIO_IN_HIGHZ);
  LQ_GPIO_Init_Config(DSW0p, GPIO_IN_HIGHZ);
  LQ_GPIO_Init_Config(DSW1p, GPIO_IN_HIGHZ);
}

/*************************************************************************
*  函数名称：unsigned char KEY_Read(KEYn_e KEYno)
*  功能说明：读取按键状态
*  参数说明：KEYn_e KEYno:按键编号
*  函数返回：按键状态，0/1
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
unsigned char KEY_Read(KEYn_e KEYno)
{
    switch(KEYno)
    {
      case KEY0:
        return LQ_Pin_Read_In(KEY0p);//按键0
      break;

      case KEY1:
        return LQ_Pin_Read_In(KEY1p);//按键1
      break;

      case KEY2:
        return LQ_Pin_Read_In(KEY2p);//按键2
      break;

      case DSW0:
        return LQ_Pin_Read_In(DSW0p);//拨码开关0
      break;

      case DSW1:
        return LQ_Pin_Read_In(DSW1p);//拨码开关1
      break;
      default:
        return 0XFF;
    }
}


/*************************************************************************
*  函数名称：unsigned char KEY_Read_All(void)
*  功能说明：读取全部按键
*  参数说明：无
*  函数返回：全部按键状态
*  修改时间：2024年11月22日
*  备    注：读取全部按键状态，方便集中使用
*************************************************************************/
unsigned char KEY_Read_All(void)
{
   unsigned char tm=0;

   tm = (LQ_Pin_Read_In(KEY0p)|(LQ_Pin_Read_In(KEY1p)<<1)|(LQ_Pin_Read_In(KEY2p)<<2));
   if(tm==0x07)
	{
	   return 0;
	}

   return  (~tm)&0X07;
}

/*************************************************************************
*  函数名称：void Test_GPIO_KEY(void)
*  功能说明：按键测试
*  参数说明：无
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void Test_GPIO_KEY(void)
 {
  unsigned char k=0xFF;
  GPIO_LED_Init();
  GPIO_KEY_Init();
  while(1)
  {
    k= KEY_Read(KEY0)& KEY_Read(KEY1)& KEY_Read(KEY2);

    if(k==0) LED_Ctrl(LEDALL, ON); //按键0/1/2按下，LED灯亮
    else     LED_Ctrl(LEDALL, OFF);//按键0/1/2松开，LED灯灭

    Cy_SysTick_DelayInUs(100000);                //延时
  }
 }
/*************************************************************************
*  函数名称：void Reed_Init(void)
*  功能说明：中断初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void Reed_Init(void)
{
  LQ_GPIO_Exit_InitConfig(REEDp, LQ_GPIO_INTR_FALLING);
}

/*************************************************************************
*  函数名称：void Test_reed(void)
*  功能说明：外部中断测试
*  参数说明：无
*  函数返回：无
*  修改时间：2024年11月22日
*  备    注：
*************************************************************************/
void Test_reed(void)
{
  Reed_Init();
  GPIO_LED_Init();
  while(1)
  {
    Cy_SysTick_DelayInUs(100000);
  }
}
void Target(void)
{
      if(KEY_Read(KEY0) == 0)
    {
      Cy_SysTick_DelayInUs(10000);
      if(KEY_Read(KEY0) == 0)
      {
       KP+=0.03;
      }
    }
    if(KEY_Read(KEY1) == 0)
    {
      Cy_SysTick_DelayInUs(10000);
      if(KEY_Read(KEY1) == 0)
      {
         GKD+=0.003;
      }
    }
    if(KEY_Read(KEY2) == 0)
    {
      Cy_SysTick_DelayInUs(10000);
      if(KEY_Read(KEY2) == 0)
      {
         KP2+=0.0002;
      }
    }
     if(KEY_Read(DSW0) == 0)
    {
      Cy_SysTick_DelayInUs(10000);
      if(KEY_Read(DSW0) == 0)
      {
       motor_start=1;
       
      }
    }
         if(KEY_Read(DSW0) == 1)
    {
      Cy_SysTick_DelayInUs(10000);
      if(KEY_Read(DSW0) == 1)
      {
        motor_start=0;
      }
    }
}