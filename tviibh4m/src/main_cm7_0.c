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
=================================================================
程序配套视频地址：(待更新)
=================================================================
*******************************************************************************/
#include "include.h"

int main(void)
{
      LQ_motor_init();                                              // 电机初始化
    //系统
    SystemInit();
    // 系统初始化
    __enable_irq();
    Cy_Cpu_CorePeriDividerSet(0); // 设置外设时钟分频
    Cy_Cpu_CoreSlowClkDividerSet(0);
    

    for (;;)
    {

    }
//move
}



/* [] END OF FILE */
