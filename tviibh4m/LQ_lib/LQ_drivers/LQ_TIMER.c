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
#include "LQ_TIMER.h"
#include "LQ_BLDC.h"


/*************************************************************************
*  函数名称：void Timer_Handler(void)
*  功能说明：定时器回调
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：定时器通道很多，这里仅例举了几个，有需要可以自行添加
*************************************************************************/

void Timer_Handler(void)
{
  /*通道2*/
  if(Cy_Tcpwm_Counter_GetTC_IntrMasked(TCPWM0_GRP0_CNT2))
  {
      Cy_Tcpwm_Counter_ClearTC_Intr(TCPWM0_GRP0_CNT2);

      timer_handler_10ms();
  }
          if(Cy_Tcpwm_Counter_GetTC_IntrMasked(TCPWM0_GRP0_CNT0))
    {
        Cy_Tcpwm_Counter_ClearTC_Intr(TCPWM0_GRP0_CNT0);
        
      timer_handler_05ms();
    }
       if(Cy_Tcpwm_Counter_GetTC_IntrMasked(TCPWM0_GRP0_CNT4))
    {
        Cy_Tcpwm_Counter_ClearTC_Intr(TCPWM0_GRP0_CNT4 );
        
      timer_handler_5ms();
    }
}

/*************************************************************************
*  函数名称：void LQ_Init_Timer(GPIO_TCP_t pin, uint16_t tim, cy_en_tcpwm_clk_prescalers_t pre)
*  功能说明：初始化定时器
*  参数说明：pin：定时器通道
             tim：计数周期
             pre：分频值
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
  这里用到的是定时器模块，对应引脚并未使用。（与PWM、编码器公用）
  tim: 计数周期（2-65535）
  pre：分频值
  定时时间 = tim/(2000 000/pre)
  例如：
    LQ_Init_Timer(P06_5_TCP_2, 10000, CY_TCPWM_PRESCALER_DIVBY_4);  
    定时时间 = 10000/(2000000/4) = 0.02s = 20ms 
*************************************************************************/
void LQ_Init_Timer(GPIO_TCP_t pin, uint16_t tim, cy_en_tcpwm_clk_prescalers_t pre)
{
  
  /*获取PWM分组*/
  uint8_t group = (uint8_t)TCP_GetGroup(pin);
  /*获取PWM通道*/
  uint16_t chl = TCP_GetChl(pin);
  
  /*获取PWM时钟和中断号 与分组和通道有关*/
  uint16_t PCLK_TCPWM_Num;
  uint16_t tcpwm_x_interrupts_x_IRQn;
  if(group == 0)
  {
    PCLK_TCPWM_Num = (uint16_t)PCLK_TCPWM0_CLOCKS0 + chl;
    tcpwm_x_interrupts_x_IRQn = (uint16_t)tcpwm_0_interrupts_0_IRQn+chl;
  }else if(group == 1)
  {
    PCLK_TCPWM_Num = (uint16_t)PCLK_TCPWM0_CLOCKS256 + chl;
    tcpwm_x_interrupts_x_IRQn = (uint16_t)tcpwm_0_interrupts_256_IRQn+chl;
  }else if(group == 3)
  {
    PCLK_TCPWM_Num = (uint16_t)PCLK_TCPWM0_CLOCKS512 + chl;
    tcpwm_x_interrupts_x_IRQn = (uint16_t)tcpwm_0_interrupts_512_IRQn+chl;
  }
  volatile stc_TCPWM_GRP_CNT_t* TCPWMx_GRPx_CNTx_PWM = ((volatile stc_TCPWM_GRP_CNT_t*) &TCPWM0->GRP[group].CNT[chl]);
  
  Cy_SysClk_HfClkEnable(CY_SYSCLK_HFCLK_2); //CLK_HF2 enable
  /* Assign a programmable divider for TCPWM1_GRP0_CNT0 */
  Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)PCLK_TCPWM_Num, (cy_en_divider_types_t)CY_SYSCLK_DIV_16_BIT, 0u);
  Cy_SysClk_PeriphSetDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)PCLK_TCPWM_Num), CY_SYSCLK_DIV_16_BIT, 0u, 39u); 
  Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)PCLK_TCPWM_Num), CY_SYSCLK_DIV_16_BIT, 0u);
  
  cy_stc_tcpwm_counter_config_t const MyCounter_config =
  {
      .period             = tim - 1,                      		// 1000 / 1000000 = 0.001s = 1ms = 1000us
      .clockPrescaler     = pre,  		                        // 2,000,000Hz / 16 = 125 000Hz 
      .runMode            = CY_TCPWM_COUNTER_CONTINUOUS,
      .countDirection     = CY_TCPWM_COUNTER_COUNT_UP,
      .debug_pause        = 0uL,
      .compareOrCapture   = CY_TCPWM_COUNTER_MODE_COMPARE,
      .compare0           = 0,
      .compare0_buff      = 0,
      .compare1           = 0,
      .compare1_buff      = 0,
      .enableCompare0Swap = false,
      .enableCompare1Swap = false,
      .interruptSources   = CY_TCPWM_INT_NONE,
      .capture0InputMode  = CY_TCPWM_INPUT_LEVEL,
      .capture0Input      = 0uL,
      .reloadInputMode    = CY_TCPWM_INPUT_LEVEL,
      .reloadInput        = 0uL,
      .startInputMode     = CY_TCPWM_INPUT_LEVEL,
      .startInput         = 0uL,
      .stopInputMode      = CY_TCPWM_INPUT_LEVEL,
      .stopInput          = 0uL,
      .capture1InputMode  = CY_TCPWM_INPUT_LEVEL,
      .capture1Input      = 0uL,
      .countInputMode     = CY_TCPWM_INPUT_LEVEL,
      .countInput         = 1uL,
      .trigger0EventCfg   = CY_TCPWM_COUNTER_OVERFLOW,
      .trigger1EventCfg   = CY_TCPWM_COUNTER_OVERFLOW,
  };

  cy_stc_sysint_irq_t irq_cfg = 
  {
      .sysIntSrc  = (cy_en_intr_t)tcpwm_x_interrupts_x_IRQn,
      .intIdx     = CPUIntIdx3_IRQn,
      .isEnabled  = true,
  };
    
  Cy_SysInt_InitIRQ(&irq_cfg);
  Cy_SysInt_SetSystemIrqVector(irq_cfg.sysIntSrc, Timer_Handler);
  /* Set the Interrupt Priority & Enable the Interrupt */
  NVIC_SetPriority(CPUIntIdx3_IRQn, 3u);
  NVIC_ClearPendingIRQ(CPUIntIdx3_IRQn);
  NVIC_EnableIRQ(CPUIntIdx3_IRQn);
  
  /* Initialize TCPWM0_GPR0_CNT0 as Timer/Counter & Enable */
  Cy_Tcpwm_Counter_Init(TCPWMx_GRPx_CNTx_PWM, &MyCounter_config);
  Cy_Tcpwm_Counter_Enable(TCPWMx_GRPx_CNTx_PWM);
  Cy_Tcpwm_TriggerStart(TCPWMx_GRPx_CNTx_PWM);  
  /* Enable Interrupt */
  Cy_Tcpwm_Counter_SetTC_IntrMask(TCPWMx_GRPx_CNTx_PWM);

}


/*************************************************************************
*  函数名称：void Test_timer(void)
*  功能说明：定时器测试
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
  采用定时器通道2 定时20ms，定时时间到后LED0闪烁 P05.4
*************************************************************************/
void Test_timer(void)
{
  
  LQ_GPIO_Init_Config(P05_4, GPIO_OUT_STRONG);
  
  LQ_UART_Init_Config(UART7_RX_P23_0, UART7_TX_P23_1, 115200);
  LQ_Printf0("\nUART TEST (driver ver=%d.%d)\n\r", CY_SCB_DRV_VERSION_MAJOR, CY_SCB_DRV_VERSION_MINOR);
  
  /*
    定时时间 = 10000/(2000000/4) = 0.02s = 20ms
  */
  LQ_Init_Timer(P06_5_TCP_2, 10000, CY_TCPWM_PRESCALER_DIVBY_4);  
  
  while(1)
  {
    Cy_SysTick_DelayInUs(10000);
    LQ_Printf("\nUART TEST (driver ver=%d.%d)\n\r CY_SCB_DRV_VERSION_MAJOR, CY_SCB_DRV_VERSION_MINOR");
  }
}














