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
#include "LQ_counter.h"

/*************************************************************************
*  函数名称：void LQ_Counter_Init_SDZ(GPIO_TCP_t pinA, GPIO_Name_t dir)
*  功能说明：编码器初始化
*  参数说明：pinA：编码器A相引脚
             dir：编码器B相引脚（SDZ方向引脚）
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
void LQ_Counter_Init_SDZ(GPIO_TCP_t pinA, GPIO_Name_t dir)
{
  LQ_GPIO_Init_Config(dir, GPIO_IN_HIGHZ);
  /*获取GPIO端口*/
  uint16_t port = (uint16_t)TCP_GetModule(pinA);
  /*获取GPIO引脚*/
  uint32_t pin_num = (uint32_t)TCP_GetIndex(pinA);
  /*获取PWM分组*/
  uint8_t group = (uint8_t)TCP_GetGroup(pinA);
  /*获取PWM通道*/
  uint16_t chl = TCP_GetChl(pinA);
  /*互补输出*/
  uint8_t N = TCP_GetN(pinA);
 
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
  
  /*开启时钟*/
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

  cy_stc_gpio_pin_config_t tcpwm_trig_in_pin_cfg =
  {
      .outVal    = 0x0,
      .driveMode = CY_GPIO_DM_STRONG,
      .hsiom     = (en_hsiom_sel_t)10 ,
      .intEdge   = 0,
      .intMask   = 0,
      .vtrip     = 0,
      .slewRate  = 0,
      .driveSel  = 0,
      .vregEn    = 0,
      .ibufMode  = 0,
      .vtripSel  = 0,
      .vrefSel   = 0,
      .vohSel    = 0,
  };

  if(N == 1)
  {
    tcpwm_trig_in_pin_cfg.hsiom = (en_hsiom_sel_t)11;
  }
  Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[port]), pin_num, &tcpwm_trig_in_pin_cfg);
  
//  cy_stc_sysint_irq_t irq_cfg =
//  {
//      .sysIntSrc  = tcpwm_0_interrupts_0_IRQn,
//      .intIdx     = CPUIntIdx3_IRQn,
//      .isEnabled  = true,
//  };
//  /* Configure the Interrut ISR */
//  Cy_SysInt_InitIRQ(&irq_cfg);    
//  Cy_SysInt_SetSystemIrqVector(irq_cfg.sysIntSrc, Counter_Handler);
//  /* Set the Interrupt Priority & Enable the Interrupt */
//  NVIC_SetPriority(CPUIntIdx3_IRQn, 3u);
//  NVIC_ClearPendingIRQ(CPUIntIdx3_IRQn);
//  
//  NVIC_EnableIRQ(CPUIntIdx3_IRQn);
  cy_stc_tcpwm_counter_config_t const MyCounter_config =
  {
    .period             = 0xFFFF,
    .clockPrescaler     = CY_TCPWM_PRESCALER_DIVBY_4,
    .runMode            = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection     = CY_TCPWM_COUNTER_COUNT_UP,
    .debug_pause        = 0uL,
    .compareOrCapture   = CY_TCPWM_COUNTER_MODE_COMPARE,
    .compare0           = 500u,                       // Set the comp value
    .compare0_buff      = 32768uL,
    .compare1           = 16384uL,
    .compare1_buff      = 32768uL,
    .enableCompare0Swap = false,
    .enableCompare1Swap = false,
    .interruptSources   = CY_TCPWM_INT_NONE,
    .capture0InputMode  = CY_TCPWM_INPUT_LEVEL ,
    .capture0Input      = 0uL,
    .reloadInputMode    = CY_TCPWM_INPUT_LEVEL ,
    .reloadInput        = 0uL,
    .startInputMode     = CY_TCPWM_INPUT_LEVEL ,
    .startInput         = 0uL,
    .stopInputMode      = CY_TCPWM_INPUT_LEVEL ,
    .stopInput          = 0uL,
    .capture1InputMode  = CY_TCPWM_INPUT_LEVEL ,
    .capture1Input      = 0uL,
    .countInputMode     = CY_TCPWM_INPUT_RISING_EDGE,		// detect rising edge
    .countInput         = 2uL,
    .trigger0EventCfg   = CY_TCPWM_COUNTER_OVERFLOW,
    .trigger1EventCfg   = CY_TCPWM_COUNTER_OVERFLOW,
  };

  /* Initialize TCPWM0_GRPx_CNTx_COUNTER as Counter & Enable */
  Cy_Tcpwm_Counter_Init(TCPWMx_GRPx_CNTx_PWM, &MyCounter_config);
  Cy_Tcpwm_Counter_Enable(TCPWMx_GRPx_CNTx_PWM);
  Cy_Tcpwm_TriggerStart(TCPWMx_GRPx_CNTx_PWM);  
  /* Enable Interrupt */
  Cy_Tcpwm_Counter_SetCC0_IntrMask(TCPWMx_GRPx_CNTx_PWM);
  
  
}

/*************************************************************************
*  函数名称：int32_t Read_Encoder_SDZ(GPIO_TCP_t pinA, GPIO_Name_t dir)
*  功能说明：编码器读取
*  参数说明：pinA：编码器A相脉冲引脚
             dir：编码器B相方向引脚
*  函数返回：编码器读数（带符号）
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
int32_t Read_Encoder_SDZ(GPIO_TCP_t pinA, GPIO_Name_t dir)
{
  int32_t g_CntValResult;
  
  /*获取PWM分组*/
  uint8_t group = (uint8_t)TCP_GetGroup(pinA);
  /*获取PWM通道*/
  uint16_t chl = TCP_GetChl(pinA);
  
  volatile stc_TCPWM_GRP_CNT_t* TCPWMx_GRPx_CNTx_PWM = ((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[group].CNT[chl]);
  
  g_CntValResult = Cy_Tcpwm_Counter_GetCounter(TCPWMx_GRPx_CNTx_PWM);
  Cy_Tcpwm_Counter_SetCounter(TCPWMx_GRPx_CNTx_PWM, 0);
  if(LQ_Pin_Read_In(dir) == 0)
  {
    g_CntValResult = -g_CntValResult;
  }
  return g_CntValResult;
}




















