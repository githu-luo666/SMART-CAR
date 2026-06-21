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
#include "LQ_PWM.h"


/*************************************************************************
*  函数名称：uint16_t LQ_PWM_Init(GPIO_TCP_t pin, uint32_t fre)
*  功能说明：初始化PWM
*  参数说明：pin：PWM引脚
             fre：PWM频率
*  函数返回：返回最大PWM占空比
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
uint16_t LQ_PWM_Init(GPIO_TCP_t pin, uint32_t fre)
{
  /*获取GPIO端口*/
  uint16_t port = (uint16_t)TCP_GetModule(pin);
  volatile stc_GPIO_PRT_t* PORT_num = PORTX[port];
  /*获取GPIO引脚*/
  uint32_t pin_num = (uint32_t)TCP_GetIndex(pin);
  /*获取PWM分组*/
  uint8_t group = (uint8_t)TCP_GetGroup(pin);
  /*获取PWM通道*/
  uint16_t chl = TCP_GetChl(pin);
  /*互补输出*/
  uint8_t N = TCP_GetN(pin);
  volatile stc_TCPWM_GRP_CNT_t* TCPWMx_GRPx_CNTx_PWM = ((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[group].CNT[chl]);
  /*获取PWM时钟 与分组和通道有关*/
  uint32_t PCLK_TCPWM_Num;
  if(group == 0)
  {
    PCLK_TCPWM_Num = 0x0126u + chl;
  }else if(group == 1)
  {
    PCLK_TCPWM_Num = 0x0165u + chl;
  }else if(group == 3)
  {
    PCLK_TCPWM_Num = 0x0171u + chl;
  }
  
  Cy_SysClk_HfClkEnable(CY_SYSCLK_HFCLK_2); //开启时钟 CLK_HF2
  
  /*--------------------------------*/
  /* Clock Configuration for TCPWMs */
  /*--------------------------------*/
  /* Assign a programmable divider  for TCPWMx_GRPx_CNTx_PWM 为选定的IP块(如TCPWM或SCB)分配一个可编程分配器。*/
  Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)PCLK_TCPWM_Num, (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0u);
  /*PWM 分频设置*/
  Cy_SysClk_PeriphSetDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)PCLK_TCPWM_Num), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0u, 39u); // Divider 39 --> 80MHz / (39+1) = 2MHz
  /*启用分频器*/
  Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)PCLK_TCPWM_Num), (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0u);

  /*配置结构体*/
  cy_stc_gpio_pin_config_t config=             
  {                                                  
      .outVal = 0x01u,                                  //引脚输出状态
      .driveMode = CY_GPIO_DM_STRONG,                   //驱动模式
      .hsiom = (en_hsiom_sel_t)8,                       //高速IO矩阵      P10_1_GPIO
      .intEdge = 0,                                     //中断边缘类型
      .intMask = 0,                                     //中断使能掩码
      .vtrip = 0,                                       //输入缓冲电压跳闸类型
      .slewRate = 0,                                    //输出缓冲区转换率
      .driveSel = 0,                                    //驱动能力
      .vregEn = 0,                                      //SIO对输出缓冲模式
      .ibufMode = 0,                                    //SIO对输入缓冲模式
      .vtripSel = 0,                                    //SIO对输入缓冲跳闸点
      .vrefSel = 0,                                     //输入缓冲跳闸点的SIO对参考电压
      .vohSel = 0,                                      //SIO对稳压输出电平
  };
  if(N == 1)
  {
    config.hsiom = (en_hsiom_sel_t)9;
  }
  Cy_GPIO_Pin_Init(PORT_num, pin_num, &config);
 
  
  cy_stc_tcpwm_pwm_config_t  MyPWM_config =
  {
      .pwmMode            = CY_TCPWM_PWM_MODE_PWM,
      .clockPrescaler     = CY_TCPWM_PRESCALER_DIVBY_1, //分频值1 2 4 8 16 32 64 128
      .debug_pause        = false,
      .cc0MatchMode       = CY_TCPWM_PWM_TR_CTRL2_CLEAR,
      .overflowMode       = CY_TCPWM_PWM_TR_CTRL2_SET,
      .underflowMode      = CY_TCPWM_PWM_TR_CTRL2_NO_CHANGE,
      .cc1MatchMode       = CY_TCPWM_PWM_TR_CTRL2_NO_CHANGE,
      .deadTime           = 0uL,
      .deadTimeComp       = 0uL,
      .runMode            = CY_TCPWM_PWM_CONTINUOUS,
      .period             = 200 - 1,          //重装载值
      .period_buff        = 0,
      .enablePeriodSwap   = false,
      .compare0           = 0,            //占空比
      .compare1           = 0,
      .enableCompare0Swap = false,
      .enableCompare1Swap = false,
      .interruptSources   = CY_TCPWM_INT_NONE,
      .invertPWMOut       = 0uL,
      .invertPWMOutN      = 0uL,
      .killMode           = CY_TCPWM_PWM_STOP_ON_KILL,
      .switchInputMode    = CY_TCPWM_INPUT_LEVEL,
      .switchInput        = 0uL,
      .reloadInputMode    = CY_TCPWM_INPUT_LEVEL,
      .reloadInput        = 0uL,
      .startInputMode     = CY_TCPWM_INPUT_LEVEL,
      .startInput         = 0uL,
      .kill0InputMode     = CY_TCPWM_INPUT_LEVEL,
      .kill0Input         = 0uL,
      .kill1InputMode     = CY_TCPWM_INPUT_LEVEL,
      .kill1Input         = 0uL,
      .countInputMode     = CY_TCPWM_INPUT_LEVEL,
      .countInput         = 1uL,
  };
   
  uint32_t fen, cpl, per;
  fen = 2000000/fre;    //fen = 2000000/10000=200
  cpl = fen/10000;
  if(cpl < 1)
  {
    MyPWM_config.clockPrescaler = CY_TCPWM_PRESCALER_DIVBY_1;
    per = 2000000/1/fre;
    MyPWM_config.period = per;
    MyPWM_config.compare0 = per/2;
  }else if(cpl<2)
  {
    MyPWM_config.clockPrescaler = CY_TCPWM_PRESCALER_DIVBY_2;
    per = 2000000/2/fre;
    MyPWM_config.period = per;
    MyPWM_config.compare0 = per/2;
  }else if(cpl<4)
  {
    MyPWM_config.clockPrescaler = CY_TCPWM_PRESCALER_DIVBY_4;
    per = 2000000/4/fre;
    MyPWM_config.period = per;
    MyPWM_config.compare0 = per/2;
  }else if(cpl<8)
  {
    MyPWM_config.clockPrescaler = CY_TCPWM_PRESCALER_DIVBY_8;
    per = 2000000/8/fre;
    MyPWM_config.period = per;
    MyPWM_config.compare0 = per/2;
  }else if(cpl<16)
  {
    MyPWM_config.clockPrescaler = CY_TCPWM_PRESCALER_DIVBY_16;
    per = 2000000/16/fre;
    MyPWM_config.period = per;
    MyPWM_config.compare0 = per/2;
  }else if(cpl<32)
  {
    MyPWM_config.clockPrescaler = CY_TCPWM_PRESCALER_DIVBY_32;
    per = 2000000/32/fre;
    MyPWM_config.period = per;
    MyPWM_config.compare0 = per/2;
  }else if(cpl<64)
  {
    MyPWM_config.clockPrescaler = CY_TCPWM_PRESCALER_DIVBY_64;
    per = 2000000/64/fre;
    MyPWM_config.period = per;
    MyPWM_config.compare0 = per/2;
  }else if(cpl<128)
  {
    MyPWM_config.clockPrescaler = CY_TCPWM_PRESCALER_DIVBY_128;
    per = 2000000/128/fre;
    MyPWM_config.period = per;
    MyPWM_config.compare0 = per/2;
  }
  
  /* Initialize TCPWM0_GRPx_CNTx_PWM_PR as PWM Mode & Enable */
  Cy_Tcpwm_Pwm_Init(TCPWMx_GRPx_CNTx_PWM, &MyPWM_config);
  Cy_Tcpwm_Pwm_Enable(TCPWMx_GRPx_CNTx_PWM);
  Cy_Tcpwm_TriggerStart(TCPWMx_GRPx_CNTx_PWM);
  return per;   //返回最大占空比值
}

/*************************************************************************
*  函数名称：void LQ_Set_Pwm(GPIO_TCP_t pin, uint32_t duty)
*  功能说明：设置PWM占空比
*  参数说明：pin：PWM引脚
             duty：高电平宽度
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
void LQ_Set_Pwm(GPIO_TCP_t pin, uint32_t duty)
{
  /*获取PWM分组*/
  uint8_t group = (uint8_t)TCP_GetGroup(pin);
  /*获取PWM通道*/
  uint16_t chl = TCP_GetChl(pin);
  volatile stc_TCPWM_GRP_CNT_t* TCPWMx_GRPx_CNTx_PWM = ((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[group].CNT[chl]);
  Cy_Tcpwm_Pwm_SetCompare0(TCPWMx_GRPx_CNTx_PWM, duty);
}



