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
*  函数名称：void LQ_Init_ADC(GPIO_ADC_t pinADC)
*  功能说明：ADC初始化
*  参数说明：pinADC：ADC引脚
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
void LQ_Init_ADC(GPIO_ADC_t pinADC)
{
  uint8_t port, pin_num, group, chl;
  port = ADC_GetModule(pinADC);
  pin_num = ADC_GetIndex(pinADC);
  group = ADC_GetGroup(pinADC);
  chl = ADC_GetChl(pinADC);
  cy_stc_gpio_pin_config_t adcPinConfig =
  {
      .outVal    = 0ul,
      .driveMode = CY_GPIO_DM_ANALOG,
      .hsiom     = (en_hsiom_sel_t)0,
      .intEdge   = 0ul,
      .intMask   = 0ul,
      .vtrip     = 0ul,
      .slewRate  = 0ul,
      .driveSel  = 0ul,
  };
  Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[port]), pin_num, &adcPinConfig);
  
  /* ADC clock divider setting */
  uint16_t ADC_PCLK = (uint16_t)PCLK_PASS0_CLOCK_SAR0+group;
  uint32_t actualAdcOperationFreq;
  
  uint32_t periFreq = 80000000ul;

  uint32_t divNum = DIV_ROUND_UP(periFreq, ADC_OPERATION_FREQUENCY_MAX_IN_HZ);
  actualAdcOperationFreq = periFreq / divNum;
  Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)ADC_PCLK, CY_SYSCLK_DIV_16_BIT, 0u);
  Cy_SysClk_PeriphSetDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)ADC_PCLK), CY_SYSCLK_DIV_16_BIT, 0u, (divNum - 1ul));
  Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)ADC_PCLK), CY_SYSCLK_DIV_16_BIT, 0u);
  
  uint32_t samplingCycle = (uint32_t)DIV_ROUND_UP((ANALOG_IN_SAMPLING_TIME_MIN_IN_NS * (uint64_t)actualAdcOperationFreq), 1000000000ull);

  cy_stc_adc_config_t adcConfig =
  {
      .preconditionTime    = 0u,
      .powerupTime         = 0u,
      .enableIdlePowerDown = false,
      .msbStretchMode      = CY_ADC_MSB_STRETCH_MODE_1CYCLE,
      .enableHalfLsbConv   = 0u,
      .sarMuxEnable        = true,
      .adcEnable           = true,
      .sarIpEnable         = true,
  };
  cy_stc_adc_channel_config_t adcChannelConfig =
  {
      .triggerSelection          = CY_ADC_TRIGGER_OFF,
      .channelPriority           = 0u,
      .preenptionType            = CY_ADC_PREEMPTION_FINISH_RESUME,
      .isGroupEnd                = true,
      .doneLevel                 = CY_ADC_DONE_LEVEL_PULSE,
      .pinAddress                = (cy_en_adc_pin_address_t)chl,
      .portAddress               = (cy_en_adc_port_address_t)group,
      .extMuxSelect              = 0u,
      .extMuxEnable              = true,
      .preconditionMode          = CY_ADC_PRECONDITION_MODE_OFF,
      .overlapDiagMode           = CY_ADC_OVERLAP_DIAG_MODE_OFF,
      .sampleTime                = samplingCycle,
      .calibrationValueSelect    = CY_ADC_CALIBRATION_VALUE_REGULAR,
      .postProcessingMode        = CY_ADC_POST_PROCESSING_MODE_NONE,
      .resultAlignment           = CY_ADC_RESULT_ALIGNMENT_RIGHT,
      .signExtention             = CY_ADC_SIGN_EXTENTION_UNSIGNED,
      .averageCount              = 0u,
      .rightShift                = 0u,
      .rangeDetectionMode        = CY_ADC_RANGE_DETECTION_MODE_INSIDE_RANGE,
      .rangeDetectionLoThreshold = 0x0000u,
      .rangeDetectionHiThreshold = 0x0FFFu,
      .mask.grpDone              = true,
      .mask.grpCancelled         = false,
      .mask.grpOverflow          = false,
      .mask.chRange              = false,
      .mask.chPulse              = false,
      .mask.chOverflow           = false,
  };

  Cy_Adc_Init(((volatile stc_PASS_SAR_t*) &PASS0->SAR[group]), &adcConfig);
  Cy_Adc_Channel_Init(((volatile stc_PASS_SAR_CH_t*) &PASS0->SAR[group].CH[chl]), &adcChannelConfig);
    
  
//   /* Register ADC interrupt handler and enable interrupt */
//    {
//        cy_stc_sysint_irq_t irq_cfg;
//        irq_cfg = (cy_stc_sysint_irq_t){
//            .sysIntSrc  = (cy_en_intr_t)((uint32_t)CY_ADC_POT_IRQN + ADC_LOGICAL_CHANNEL),
//            .intIdx     = CPUIntIdx3_IRQn,
//            .isEnabled  = true,
//        };
//        Cy_SysInt_InitIRQ(&irq_cfg);
//        Cy_SysInt_SetSystemIrqVector(irq_cfg.sysIntSrc, AdcIntHandler);
//        NVIC_SetPriority(irq_cfg.intIdx, 0ul);
//        NVIC_EnableIRQ(irq_cfg.intIdx);
//    }

    /* Enable ADC ch. */
    Cy_Adc_Channel_Enable(((volatile stc_PASS_SAR_CH_t*) &PASS0->SAR[group].CH[chl]));
    /* Issue SW trigger */
    Cy_Adc_Channel_SoftwareTrigger(((volatile stc_PASS_SAR_CH_t*) &PASS0->SAR[group].CH[chl]));

}

/*************************************************************************
*  函数名称：uint16_t Read_ADC(GPIO_ADC_t pinADC)
*  功能说明：ADC读取
*  参数说明：pinADC：ADC引脚
*  函数返回：ADC读取值
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
uint16_t Read_ADC(GPIO_ADC_t pinADC)
{
  uint8_t group, chl;
  uint16_t value1;
  cy_stc_adc_ch_status_t status;
  
  group = ADC_GetGroup(pinADC);
  chl = ADC_GetChl(pinADC);
  
  /* Get the result(s) */
  Cy_Adc_Channel_GetResult(((volatile stc_PASS_SAR_CH_t*) &PASS0->SAR[group].CH[chl]), &value1, &status);

  /* Trigger next conversion */
  Cy_Adc_Channel_SoftwareTrigger(((volatile stc_PASS_SAR_CH_t*) &PASS0->SAR[group].CH[chl]));
  
  return value1;
  
}

/*************************************************************************
*  函数名称：void Test_ADC(void)
*  功能说明：ADC测试函数
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：测试母板ADC接口
*************************************************************************/
void Test_ADC(void)
{
  char txt[64];
  
  uint16_t ADC60, ADC61, ADC62, ADC63, ADC64, ADC65, ADC66, ADC67;
  
  GPIO_LED_Init();
  LQ_UART_Init_Config(UART7_RX_P23_0, UART7_TX_P23_1, 115200);

  LQ_Printf("\nUART TEST (driver ver=%d.%d)\n\r", CY_SCB_DRV_VERSION_MAJOR, CY_SCB_DRV_VERSION_MINOR);

  /**选择屏幕类型**/
#if show_type==0
  OLED_Init();                          //OLED初始化
  OLED_Show_LQLogo();                   //显示LOGO
  OLED_CLS();
  OLED_P8x16Str(5, 0,"LQ ADC Test");       //显示字符串
#elif show_type==1
  TFTSPI_Init(1);        //LCD初始化 
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_Show_Logo(0,37);//显示龙邱LOGO
  TFTSPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLUE);      //字符串显示
  TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);     //字符串显示
  TFTSPI_CLS(u16BLACK);   //蓝色屏幕
  TFTSPI_P8X16Str(3, 0, "LQ ADC Test", u16RED, u16BLUE);
#elif show_type==2
  TFT2SPI_Init(0);        //LCD初始化
  TFT2SPI_CLS(u16BLUE);   //蓝色屏幕
  TFT2SPI_Show_Logo(0,37);//显示龙邱LOGO
  TFT2SPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLUE);      //字符串显示
  TFT2SPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);     //字符串显示
  TFT2SPI_CLS(u16BLACK);   //蓝色屏幕 
  TFT2SPI_P8X16Str(3, 0, "LQ ADC Test", u16RED, u16BLUE);
#endif
  
  LQ_Init_ADC(ADC0_CH0_P06_0);
  LQ_Init_ADC(ADC0_CH1_P06_1);
  LQ_Init_ADC(ADC0_CH2_P06_2);
  LQ_Init_ADC(ADC0_CH3_P06_3);
  
  LQ_Init_ADC(ADC0_CH4_P06_4);
  LQ_Init_ADC(ADC0_CH5_P06_5);
  LQ_Init_ADC(ADC0_CH6_P06_6);
  LQ_Init_ADC(ADC0_CH7_P06_7);
  
  while(1)
  {
    ADC60 = Read_ADC(ADC0_CH0_P06_0);
    ADC61 = Read_ADC(ADC0_CH1_P06_1);
    ADC62 = Read_ADC(ADC0_CH2_P06_2);
    ADC63 = Read_ADC(ADC0_CH3_P06_3);
    ADC64 = Read_ADC(ADC0_CH4_P06_4);
    ADC65 = Read_ADC(ADC0_CH5_P06_5);
    ADC66 = Read_ADC(ADC0_CH6_P06_6);
    ADC67 = Read_ADC(ADC0_CH7_P06_7);
    
    LQ_Printf("ADC1 60:%04d  61:%04d 62:%04d  63:%04d \n",ADC60, ADC61, ADC62, ADC63);
    LQ_Printf("ADC2 64:%04d  65:%04d  \n",ADC64, ADC65);
    LQ_Printf("ADC3 66:%04d  67:%04d \n",ADC66, ADC67);
    
#if show_type==0
    sprintf(txt,"ADC1 60:%04d 61:%04d",ADC60, ADC61);
    OLED_P6x8Str(0, 3,txt);       //显示字符串
    sprintf(txt,"     62:%04d 63:%04d",ADC62, ADC63);
    OLED_P6x8Str(0, 4,txt);       //显示字符串
    sprintf(txt,"ADC2 64:%04d 65:%04d",ADC64, ADC65);
    OLED_P6x8Str(0, 5,txt);       //显示字符串
    sprintf(txt,"ADC3 66:%04d 67:%04d",ADC66, ADC67);
    OLED_P6x8Str(0, 6,txt);       //显示字符串
#elif show_type==1
    sprintf(txt,"ADC1:");
    TFTSPI_P8X16Str(0, 2, txt, u16RED, u16BLACK);
    sprintf(txt,"60:%04d 61:%04d",ADC60, ADC61);
    TFTSPI_P8X16Str(1, 3, txt, u16RED, u16BLACK);
    sprintf(txt,"62:%04d 63:%04d",ADC62, ADC63);
    TFTSPI_P8X16Str(1, 4, txt, u16RED, u16BLACK);
    sprintf(txt,"ADC2:");
    TFTSPI_P8X16Str(0, 5, txt, u16YELLOW, u16BLACK);
    sprintf(txt,"64:%04d 65:%04d",ADC64, ADC65);
    TFTSPI_P8X16Str(1, 6, txt, u16YELLOW, u16BLACK);
    sprintf(txt,"ADC3:");
    TFTSPI_P8X16Str(0, 7, txt, u16GREEN, u16BLACK);
    sprintf(txt,"66:%04d 67:%04d",ADC66, ADC67);
    TFTSPI_P8X16Str(1, 8, txt, u16GREEN, u16BLACK);
#elif show_type==2
    sprintf(txt,"ADC1:");
    TFT2SPI_P8X16Str(0, 2, txt, u16RED, u16BLACK);
    sprintf(txt,"60:%04d 61:%04d",ADC60, ADC61);
    TFT2SPI_P8X16Str(1, 3, txt, u16RED, u16BLACK);
    sprintf(txt,"62:%04d 63:%04d",ADC62, ADC63);
    TFT2SPI_P8X16Str(1, 4, txt, u16RED, u16BLACK);
    sprintf(txt,"ADC2:");
    TFT2SPI_P8X16Str(0, 5, txt, u16YELLOW, u16BLACK);
    sprintf(txt,"64:%04d 65:%04d",ADC64, ADC65);
    TFT2SPI_P8X16Str(1, 6, txt, u16YELLOW, u16BLACK);
    sprintf(txt,"ADC3:");
    TFT2SPI_P8X16Str(0, 7, txt, u16GREEN, u16BLACK);
    sprintf(txt,"66:%04d 67:%04d",ADC66, ADC67);
    TFT2SPI_P8X16Str(1, 8, txt, u16GREEN, u16BLACK);
#endif
    LED_Ctrl(LED0, RVS);
    Cy_SysTick_DelayInUs(50000);
  }
}







