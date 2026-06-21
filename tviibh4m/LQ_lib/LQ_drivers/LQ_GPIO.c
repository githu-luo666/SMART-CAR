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
#include "cyt4bb8cee.h"


volatile stc_GPIO_PRT_t* PORTX[33] = 
  {     
    GPIO_PRT0,  GPIO_PRT1,  GPIO_PRT2,  GPIO_PRT3,  GPIO_PRT4,  GPIO_PRT5,  GPIO_PRT6,  GPIO_PRT7,
    GPIO_PRT8,  GPIO_PRT9,  GPIO_PRT10, GPIO_PRT11, GPIO_PRT12, GPIO_PRT13, GPIO_PRT14, GPIO_PRT15,
    GPIO_PRT16, GPIO_PRT17, GPIO_PRT18, GPIO_PRT19, GPIO_PRT20, GPIO_PRT21, GPIO_PRT22, GPIO_PRT23,
    GPIO_PRT24, GPIO_PRT25, GPIO_PRT26, GPIO_PRT27, GPIO_PRT28, GPIO_PRT29, GPIO_PRT30, GPIO_PRT31, GPIO_PRT32,
  };

/*外部中断服务函数*/
void PortHandler(void)
{
    /* If falling edge detected */
    if (LQ_GPIO_GetInterruptStatusMasked(P21_5) != 0ul)
    {
        LQ_GPIO_ClearInterrupt(P21_5);

        /* Toggle LED */
        LED_Ctrl(LED0, RVS);
    }
    /* If falling edge detected */
    if (LQ_GPIO_GetInterruptStatusMasked(P17_1) != 0ul)//摄像头场中断
    {      
        // task_heartbeat();
        LQ_GPIO_ClearInterrupt(P17_1);    
    }
    /* If falling edge detected */
    if (LQ_GPIO_GetInterruptStatusMasked(P17_3) != 0ul)
    {
        LQ_GPIO_ClearInterrupt(P17_3);

    }
}

/****************************************************************
函数：LQ_Pin_InitConfig
作用：初始化GPIO引脚
参数：GPIO_Name_t：GPIO引脚枚举量
      GPIO_MODE_t：GPIO引脚模式
备注
****************************************************************/
void LQ_GPIO_Init_Config(GPIO_Name_t pin, GPIO_MODE_t dir)
{
  /*获取GPIO端口*/
  uint16_t port = (uint16_t)PIN_GetModule(pin);
  /*获取GPIO引脚*/
  uint32_t pin_num = (uint32_t)PIN_GetIndex(pin);
  /*配置结构体*/
  cy_stc_gpio_pin_config_t config=             
  {                                                  
      .outVal = 0x00,                                   //引脚输出状态
      .driveMode = CY_GPIO_DM_STRONG_IN_OFF,            //驱动模式
      .hsiom = (en_hsiom_sel_t)0,                       //高速IO矩阵      P10_1_GPIO
      .intEdge = 0,                                     //中断边缘类型
      .intMask = 0,                                     //中断使能掩码
      .vtrip = 0,                                       //输入缓冲电压跳闸类型
      .slewRate = 0,                                    //输出缓冲区转换率
      .driveSel = 1,                                    //驱动能力
      .vregEn = 0,                                      //SIO对输出缓冲模式
      .ibufMode = 0,                                    //SIO对输入缓冲模式
      .vtripSel = 0,                                    //SIO对输入缓冲跳闸点
      .vrefSel = 0,                                     //输入缓冲跳闸点的SIO对参考电压
      .vohSel = 0,                                      //SIO对稳压输出电平
  };
  
  switch(dir)
  {
    case GPIO_ANALOG:config.driveMode = CY_GPIO_DM_ANALOG;break;
    case GPIO_OUT_PP_UP:config.driveMode = CY_GPIO_DM_PULLUP_IN_OFF;break;
    case GPIO_OUT_PP_DOWN:config.driveMode = CY_GPIO_DM_PULLDOWN_IN_OFF;break;
    case GPIO_OUT_OD_LOW:config.driveMode = CY_GPIO_DM_OD_DRIVESLOW_IN_OFF;break;
    case GPIO_OUT_OD_HIGH:config.driveMode = CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF;break;
    case GPIO_OUT_STRONG:config.driveMode = CY_GPIO_DM_STRONG_IN_OFF;break;
    case GPIO_OUT_UP_DOWN:config.driveMode = CY_GPIO_DM_PULLUP_DOWN_IN_OFF;break;
    case GPIO_IN_HIGHZ:config.driveMode = CY_GPIO_DM_HIGHZ;break;
    case GPIO_IN_UP:config.driveMode = CY_GPIO_DM_PULLUP;break;
    case GPIO_IN_DOWN:config.driveMode = CY_GPIO_DM_PULLDOWN;break;
    case GPIO_IN_OD_LOW:config.driveMode = CY_GPIO_DM_OD_DRIVESLOW;break;
    case GPIO_IN_OD_HIGH:config.driveMode = CY_GPIO_DM_OD_DRIVESHIGH;break;
    case GPIO_IN_STRONG:config.driveMode = CY_GPIO_DM_STRONG;break;
    case GPIO_IN_UP_DOWN:config.driveMode = CY_GPIO_DM_PULLUP_DOWN;break;
  }
  Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[port]), pin_num, &config);
}

/****************************************************************
函数：LQ_GPIO_Exit_InitConfig
作用：初始化GPIO为外部中断模式
参数：GPIO_Name_t：GPIO引脚枚举量
      mode：中断触发模式
备注
****************************************************************/
void LQ_GPIO_Exit_InitConfig(GPIO_Name_t pin, GPIO_ExitMode_t mode)
{
  /*获取GPIO端口*/
  uint16_t port = (uint16_t)PIN_GetModule(pin);
  /*获取GPIO引脚*/
  uint32_t pin_num = (uint32_t)PIN_GetIndex(pin);
  
  uint16_t irqn = ioss_interrupts_gpio_dpslp_0_IRQn + port;
  cy_stc_gpio_pin_config_t user_button_port_pin_cfg = 
  {
      .outVal    = 0ul,
      .driveMode = CY_GPIO_DM_HIGHZ,
      .hsiom     = (en_hsiom_sel_t)0,
      .intEdge   = mode,
      .intMask   = 1ul,
      .vtrip     = 0ul,
      .slewRate  = 0ul,
      .driveSel  = 0ul,
  };
  Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[port]), pin_num, &user_button_port_pin_cfg);
  
  /* Setup GPIO for interrupt */
  cy_stc_sysint_irq_t irq_cfg =
  {
      .sysIntSrc  = (cy_en_intr_t)irqn,
      .intIdx     = CPUIntIdx3_IRQn,
      .isEnabled  = true,
  };
  Cy_SysInt_InitIRQ(&irq_cfg);
  Cy_SysInt_SetSystemIrqVector(irq_cfg.sysIntSrc, PortHandler);

  NVIC_SetPriority(irq_cfg.intIdx, 3ul);
  NVIC_EnableIRQ(irq_cfg.intIdx);
}

/****************************************************************
函数：LQ_Pin_Write
作用：配置引脚电平状态
参数：     pin：GPIO引脚枚举量
          dir：高低电平（1或0）
返回：无
备注
****************************************************************/
void LQ_Pin_Write(GPIO_Name_t pin, uint8_t dir)
{
  /*获取GPIO端口*/
  volatile stc_GPIO_PRT_t* PORT_num = PORTX[(uint16_t)PIN_GetModule(pin)];
  /*获取GPIO引脚*/
  uint32_t pin_num = (uint32_t)PIN_GetIndex(pin);
  
  if(dir == 0)
  {
    Cy_GPIO_Clr(PORT_num, pin_num);
  }else{
    Cy_GPIO_Set(PORT_num, pin_num);
  }
}

/****************************************************************
函数：LQ_Pin_Inv
作用：引脚电平翻转
参数：     pin：GPIO引脚枚举量
返回：无
备注
****************************************************************/
void LQ_Pin_Inv(GPIO_Name_t pin)
{
  /*获取GPIO端口*/
  volatile stc_GPIO_PRT_t* PORT_num = PORTX[(uint16_t)PIN_GetModule(pin)];
  /*获取GPIO引脚*/
  uint32_t pin_num = (uint32_t)PIN_GetIndex(pin);
  
  Cy_GPIO_Inv(PORT_num, pin_num);
}

/****************************************************************
函数：LQ_Pin_Read_In
作用：读取输入引脚的电平状态
参数：     pin：GPIO引脚枚举量
返回：引脚电平状态
备注
****************************************************************/
uint32_t LQ_Pin_Read_In(GPIO_Name_t pin)
{
  /*获取GPIO端口*/
  volatile stc_GPIO_PRT_t* PORT_num = PORTX[(uint16_t)PIN_GetModule(pin)];
  /*获取GPIO引脚*/
  uint32_t pin_num = (uint32_t)PIN_GetIndex(pin);
  
  return Cy_GPIO_Read(PORT_num, pin_num);
}

/****************************************************************
函数：LQ_Pin_Read_Out
作用：读取输出引脚的电平状态
参数：     pin：GPIO引脚枚举量
返回：引脚电平状态
备注
****************************************************************/
uint32_t LQ_Pin_Read_Out(GPIO_Name_t pin)
{
  /*获取GPIO端口*/
  volatile stc_GPIO_PRT_t* PORT_num = PORTX[(uint16_t)PIN_GetModule(pin)];
  /*获取GPIO引脚*/
  uint32_t pin_num = (uint32_t)PIN_GetIndex(pin);
  
  return Cy_GPIO_ReadOut(PORT_num, pin_num);
}

/****************************************************************
函数：LQ_GPIO_ClearInterrupt
作用：清除外部中断标志位
参数：     pin：GPIO引脚枚举量
返回：无
备注
****************************************************************/
void LQ_GPIO_ClearInterrupt(GPIO_Name_t pin)
{
  /*获取GPIO端口*/
  uint16_t port = (uint16_t)PIN_GetModule(pin);
  /*获取GPIO引脚*/
  uint32_t pin_num = (uint32_t)PIN_GetIndex(pin);
  Cy_GPIO_ClearInterrupt(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[port]), pin_num);
}

/****************************************************************
函数：LQ_GPIO_GetInterruptStatusMasked
作用：返回引脚的中断状态
参数：     pin：GPIO引脚枚举量
返回：状态   0 =未检测到引脚中断或未转发到cPU中断控制器        1 =检测到引脚中断并转发到CPU中断控制器
备注
****************************************************************/
uint32_t LQ_GPIO_GetInterruptStatusMasked(GPIO_Name_t pin)
{
  /*获取GPIO端口*/
  uint16_t port = (uint16_t)PIN_GetModule(pin);
  /*获取GPIO引脚*/
  uint32_t pin_num = (uint32_t)PIN_GetIndex(pin);
  return Cy_GPIO_GetInterruptStatusMasked(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[port]), pin_num);
}

















