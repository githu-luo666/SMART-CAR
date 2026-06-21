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
#include "include.h"

/*************************************************************************
*  函数名称：void Test_counter(void)
*  功能说明：测试编码器（暂时仅支持带方向的编码器（SDZxxx））
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月22日
*  备    注：
*************************************************************************/
void Test_counter(void)
{
  int32_t ENCODER1, ENCODER2;
  char txt[32];
  
  LQ_UART_Init_Config(UART7_RX_P23_0, UART7_TX_P23_1, 115200ul);       //初始化串口7
  GPIO_LED_Init();							//初始化LED	

  LQ_Printf("\nEncoder TEST\n");
  
  /*初始化编码器接口*/
  LQ_Counter_Init_SDZ(P05_0_TCP_9,  P05_1);
  LQ_Counter_Init_SDZ(P05_2_TCP_11, P05_3);

  LCD_Init();        //LCD初始化
  LCD_CLS(BLACK);
  LCD_ShowString(2,0,"LQ Encoder Test",WHITE,BLACK,32,0);
  
  while(1)
  {
    ENCODER1 = Read_Encoder_SDZ(P05_0_TCP_9,  P05_1);
    ENCODER2 = Read_Encoder_SDZ(P05_2_TCP_11, P05_3);
    
    /*发送至串口*/
    LQ_Printf("E1:%05d E2:%05d\n", ENCODER1, ENCODER2);

    /*屏幕显示*/
    sprintf(txt,"E1:%05d",ENCODER1);
    LCD_ShowStr_24(0,2,txt,RED_IPS,LIGHTBLUE);        //显示字符串
    sprintf(txt,"E2:%05d",ENCODER2);
    LCD_ShowStr_24(0,3,txt,RED_IPS,LIGHTBLUE); 

    LED_Ctrl(LED0, RVS);
    Cy_SysTick_DelayInUs(50000);
  }    
}