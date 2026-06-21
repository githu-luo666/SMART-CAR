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
#include "LQ_Delay.h"


/*************************************************************************
*  函数名称：delay_us(uint32_t us)
*  功能说明：μ秒延时
*  参数说明：us:延时时间  单位：μ秒
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
void delay_us(uint32_t us)
{
  Cy_SysTick_DelayInUs(us);
}

/*************************************************************************
*  函数名称：delay_ms(uint16_t ms)
*  功能说明：毫秒延时
*  参数说明：ms:延时时间  单位：毫秒
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
void delay_ms(uint16_t ms)
{
  while(ms--)
  {
     delay_us(998);
  }
}
