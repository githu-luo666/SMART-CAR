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
#include "LQ_SBUS.h"

/*以下注释可能不准确，原因是通道位置可以用遥控器进行设置，不同设置对应的不一样*/
unsigned short R_H        = 0;    //右手左右
unsigned short R_V        = 0;    //右手上下
unsigned short L_V        = 0;    //左手上下
unsigned short L_H        = 0;    //左手左右
unsigned short SWA        = 0;    //A开关
unsigned short SWB        = 0;    //B开关
unsigned short SWC        = 0;    //C开关
unsigned short SWD        = 0;    //D开关
unsigned short SWE        = 0;    //E开关
unsigned short VRF        = 0;    //F开关
unsigned short VRG        = 0;    //G开关
unsigned short SWH        = 0;    //H开关

unsigned short VrA        = 0;    //左旋钮
unsigned short VrB        = 0;    //右旋钮
unsigned short VrC        = 0;    //左旋钮-后
unsigned short VrD        = 0;    //右旋钮-后
unsigned short CH_Flg     = 0;    //信号中断标志位
unsigned short CHDATA[18];


/*************************************************************************
*  函数名称：void update_sbus(void)
*  功能说明：遥控器解析函数
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：十通道解析，不同遥控器可能数据组成不同，需要根据遥控器接收机的手册进行修改
*************************************************************************/
void update_sbus(void)
{
    /*10通道解析*/
//    CHDATA[0]= ReadBuff8[0];                                                        //帧头 15=0x0F       100%               140%
    R_H     =CHDATA[ 1]=(ReadBuff8[ 2] & 0X07)<<8  | ReadBuff8[ 1]>>0;                      //右手水平左中右      0314-1005-1694； 0039-1005-1981
    R_V     =CHDATA[ 2]=(ReadBuff8[ 3] & 0X3F)<<5  | ReadBuff8[ 2]>>3;                   //右手上下上中下      0306-0994-1693； 0008-0975-1949
    L_V     =CHDATA[ 3]=(ReadBuff8[ 5] & 0X01)<<10 | ReadBuff8[ 4]<<2 | ReadBuff8[3]>>6; //左手上下上中下      0306-0999-1693；
    L_H     =CHDATA[ 4]=(ReadBuff8[ 6] & 0X0F)<<7  | ReadBuff8[ 5]>>1;                   //左手水平左中右      0306-1002-1690；
    SWC     =CHDATA[ 5]=(ReadBuff8[ 7] & 0X7F)<<4  | ReadBuff8[ 6]>>4;                   //SWC左旋钮顺时针左右  0074-1925；      0000-2000
    VrA     =CHDATA[ 6]=(ReadBuff8[ 9] & 0X03)<<9  | ReadBuff8[ 8]<<1 | ReadBuff8[7]>>7; //SWD右旋钮顺时针左右  0074-1925；      0000-2000
    SWH     =CHDATA[ 7]=(ReadBuff8[10] & 0X1F)<<5  | ReadBuff8[ 9]>>2;                   //C键上中下          0042-0488-0965； 0000-0488-1008
    VrB     =CHDATA[ 8]= ReadBuff8[11] <<3         | ReadBuff8[10]>>5;                         //D键上下            0074-1925；      0000-2000
    SWB     =CHDATA[ 9]=(ReadBuff8[13] & 0X07)<<8  | ReadBuff8[12]>>0;                   //SWA左拉杆顺时针左右   0074-1925；      0000-2000
    SWA     =CHDATA[10]=(ReadBuff8[14] & 0X3F)<<5  | ReadBuff8[13]>>3;                //SWB左拉杆顺时针左右   0085-1931；      0000-2000

}

/*************************************************************************
*  函数名称：void Test_sbus(void)
*  功能说明：遥控器测试函数
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：采用乐迪R9DS接收机，不用设计转换电路，母版上自带，
  通信方式采用SBUS（波特率100000）接收机为蓝灯模式。
*************************************************************************/
void Test_sbus(void)
{
  char txt[64];
  
  GPIO_LED_Init();
  TFTSPI_Init(0);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_Show_Logo(0,37);//显示龙邱LOGO
  TFTSPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLUE);      //字符串显示
  TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);     //字符串显示
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
//  LQ_UART_Init_Config(UART7_RX_P23_0, UART7_TX_P23_1, 115200);
  LQ_UART_Init_Config(UART8_RX_P12_0, UART8_TX_P12_1, 100000);          //SBUS串口初始化
  
  TFTSPI_P8X16Str(1,0,"R9DS TEST", u16RED,u16BLACK);
  while(1)
  {
    //串口7打印数据信息
//    LQ_Printf("RH:%04d LV:%04d RV:%04d LH:%04d VA:%04d VB:%04d SC:%04d SD:%04d SA:%04d SB:%04d\n"\
//           ,R_H,L_V,R_V,L_H,SWH,VRF,SWC,SWD,SWA,SWB);
    Usart8_Rec_Fini_Flag = 1;   //开始接收


    sprintf(txt, "D1:%04d D2:%04d",R_H,R_V);
    TFTSPI_P8X16Str(0,2,txt, u16RED,u16BLACK);
    sprintf(txt, "D3:%04d D4:%04d",L_V,L_H);
    TFTSPI_P8X16Str(0,3,txt, u16RED,u16BLACK);
    sprintf(txt, "D5:%04d D6:%04d",SWC,VrA);
    TFTSPI_P8X16Str(0,4,txt, u16RED,u16BLACK);
    sprintf(txt, "D7:%04d D8:%04d",VrB,SWA);
    TFTSPI_P8X16Str(0,5,txt, u16RED,u16BLACK);
    sprintf(txt, "D9:%04d 10:%04d",SWB,SWH);
    TFTSPI_P8X16Str(0,6,txt, u16RED,u16BLACK);
    Cy_SysTick_DelayInUs(100000);
  }
}



