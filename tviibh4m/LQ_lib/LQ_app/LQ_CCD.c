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

#define CCD_AO_INIT()           LQ_Init_ADC(ADC0_CH5_P06_5)
#define CCD_AO_GET(value)       value=Read_ADC(ADC0_CH5_P06_5);
#define CCD_CLK_INIT()          LQ_GPIO_Init_Config(P17_1, GPIO_OUT_STRONG);
#define CCD_SI_INIT()           LQ_GPIO_Init_Config(P17_2, GPIO_OUT_STRONG);

#define CCD_CLK_H()             LQ_Pin_Write(P17_1,1)
#define CCD_CLK_L()             LQ_Pin_Write(P17_1,0)

#define CCD_SI_H()              LQ_Pin_Write(P17_2,1)
#define CCD_SI_L()              LQ_Pin_Write(P17_2,0)

#define CCD_DELAY(us)           Cy_SysTick_DelayInUs(us*10)

void ccd_init()
{
  CCD_AO_INIT();
  CCD_CLK_INIT();
  CCD_SI_INIT();
  return;
}

void ccd_collect(uint16_t *p)
{	
    unsigned char i;
    unsigned int  temp = 0;

    CCD_SI_H();              //SI  = 1 
    CCD_DELAY(1);
    CCD_CLK_H();             // CLK = 1 
    CCD_DELAY(1);
    CCD_SI_L();              // SI  = 0 
    CCD_DELAY(1);

    for(i=0; i<128; i++) 
    {
        CCD_DELAY(1);
        CCD_CLK_H();         // CLK = 1 
        CCD_DELAY(1);
        CCD_AO_GET(temp);       
        *p++ = temp;     
        CCD_CLK_L();        // CLK = 0 
    }
    
    CCD_DELAY(1);
    CCD_CLK_H();            // CLK = 1 
    CCD_DELAY(1);
    CCD_CLK_L();            // CLK = 0 
  return;
}
void test_ccd()
{
    TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏，2，3
    TFTSPI_CLS(u16BLUE);   //蓝色屏幕
    uint16_t ccd_buf[128];      //存储128位ccd图像
    ccd_init();
    char txt[32];
    for(;;)
    {  
      ccd_collect(ccd_buf);  
      for(int i=0;i<128;i++)
      {       
          if(ccd_buf[i]>2000)
          {
            TFTSPI_Draw_Dot(i,8,u16WHITE);
          }else{
            TFTSPI_Draw_Dot(i,8,u16BLACK);
          }
      }                            //ccd扫描
      
      sprintf(txt,"ccd_buf[64] %04d",ccd_buf[64]);      //显示图像中间像素值
      TFTSPI_P8X16Str(0,1,txt,u16WHITE,u16BLACK);     
    
    }
    
}
