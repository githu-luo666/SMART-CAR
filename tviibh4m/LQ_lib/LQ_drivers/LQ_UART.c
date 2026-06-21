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
#include "LQ_UART.h"
#include "LQ_SBUS.h"
#include "LQ_GPS_BD.h"

cy_stc_scb_uart_context_t uartx_context[10];

/*中断接收缓存*/
uint8_t g_uart_interrupt_buf[size];

unsigned char ReadBuff8[512];  //SBUS
char Usart8_Rec_Fini_Flag;

uint8_t g_uart_interrupt_buf0[1];
char ReadBuff0[128];
char bufbd;   //北斗


uint8_t Usart7_Rec_Fini_Flag;
uint8_t ReadBuff7[512];
static int num7;
unsigned char buf7;//遥控
//---------------------------------------------------------------


void Scb0_UART_IntrISR(void);   void Scb0_UART_Event(uint32_t locEvents);
void Scb1_UART_IntrISR(void);   void Scb1_UART_Event(uint32_t locEvents);
void Scb2_UART_IntrISR(void);   void Scb2_UART_Event(uint32_t locEvents);
void Scb3_UART_IntrISR(void);   void Scb3_UART_Event(uint32_t locEvents);
void Scb4_UART_IntrISR(void);   void Scb4_UART_Event(uint32_t locEvents);
void Scb5_UART_IntrISR(void);   void Scb5_UART_Event(uint32_t locEvents);
void Scb6_UART_IntrISR(void);   void Scb6_UART_Event(uint32_t locEvents);
void Scb7_UART_IntrISR(void);   void Scb7_UART_Event(uint32_t locEvents);
void Scb8_UART_IntrISR(void);   void Scb8_UART_Event(uint32_t locEvents);
void Scb9_UART_IntrISR(void);   void Scb9_UART_Event(uint32_t locEvents);



volatile stc_SCB_t* SCB_x[10] = 
  {     
    SCB0, SCB1, SCB2, SCB3, SCB4, SCB5, SCB6, SCB7, SCB8, SCB9
  };
const uint8_t SCBx_Clk[10] = {0ul, 1ul, 2ul, 3ul, 4ul, 5ul, 6ul, 7ul, 8ul, 9ul};

/*以下为中断服务函数，注意，不管是接收完成还是发送完成都会触发中断，并执行中断中的程序
  若使用回调函数，则取消注释-串口初始化函数中的 创建回调函数的程序
*/
void Scb0_UART_IntrISR(void)
{
    /* UART interrupt handler */
    Cy_SCB_UART_Interrupt(SCB0, &uartx_context[0]);
}
void Scb1_UART_IntrISR(void)
{
    /* UART interrupt handler */
    Cy_SCB_UART_Interrupt(SCB1, &uartx_context[1]);
}
void Scb2_UART_IntrISR(void)
{
    /* UART interrupt handler */
    Cy_SCB_UART_Interrupt(SCB2, &uartx_context[2]);
}
void Scb3_UART_IntrISR(void)
{
    /* UART interrupt handler */
    Cy_SCB_UART_Interrupt(SCB3, &uartx_context[3]);
}
void Scb4_UART_IntrISR(void)
{
    /* UART interrupt handler */
    Cy_SCB_UART_Interrupt(SCB4, &uartx_context[4]);
}
void Scb5_UART_IntrISR(void)
{
    /* UART interrupt handler */
    Cy_SCB_UART_Interrupt(SCB5, &uartx_context[5]);
}
void Scb6_UART_IntrISR(void)
{
    /* UART interrupt handler */
    Cy_SCB_UART_Interrupt(SCB6, &uartx_context[6]);
}

void Scb7_UART_IntrISR(void)
{
    /* UART interrupt handler */
    Cy_SCB_UART_Interrupt(SCB7, &uartx_context[7]);
    
    /*若开启回调函数则注释掉此部分*/
//    {
//      Cy_SCB_UART_Receive(SCB7, &g_uart_interrupt_buf[0], size, &uartx_context[7]);
//      for(int i = 0; i < size; i++)
//      {
//        LQ_Printf("%c", g_uart_interrupt_buf[i]);
//      }
//    }
}
void Scb8_UART_IntrISR(void)
{
    /* UART interrupt handler */
    Cy_SCB_UART_Interrupt(SCB8, &uartx_context[8]);
    
    
}
void Scb9_UART_IntrISR(void)
{
    /* UART interrupt handler */
    Cy_SCB_UART_Interrupt(SCB9, &uartx_context[9]);
}
/*********************************以下为回调函数**********************************************/

/*此处仅展示串口7的回调服务函数*/

void Scb7_UART_Event(uint32_t locEvents)
{
    switch (locEvents)
    {
      case CY_SCB_UART_TRANSMIT_IN_FIFO_EVENT:
          break;

      case CY_SCB_UART_TRANSMIT_DONE_EVENT:
          break;

      case CY_SCB_UART_RECEIVE_DONE_EVENT:
          /* Transmit received data */
          Cy_SCB_UART_Receive(SCB7, &g_uart_interrupt_buf[0], size, &uartx_context[7]);
          buf7 = g_uart_interrupt_buf[0];
          if(num7 ==0 && buf7 == 'A')
          {
              LQ_Printf("%d\r\n",num7);
              ReadBuff0[num7] = buf7;
              num7++;
          }
          else if(num7 == 1 && buf7 == '6')
          {
              ReadBuff0[num7] = buf7;
              num7++;
          }
          else if(num7 == 2 && buf7 == 'A')
          {
              ReadBuff0[num7] = buf7;
              num7++;
          }
          else if(num7 == 3 && buf7 == '8')
          {
              ReadBuff0[num7] = buf7;
              num7++;
          }
           else if(num7 > 3 && ReadBuff0[0] == 'A' && ReadBuff0[1] == '6' && ReadBuff0[2] == 'A' && ReadBuff0[3] == '8' && num7 < 80)
          {
              ReadBuff0[num7] = buf7;
              num7++;
              if(ReadBuff0[num7 -1] == '9' && ReadBuff0[num7-2] == 'C' && ReadBuff0[num7-3] == '9' && ReadBuff0[num7-4] == 'B')
              {              
                  num7 = 0;
                  Usart7_Rec_Fini_Flag = 1;
              }
          }
          else    //不满足，舍弃一帧数据
          {
              num7 = 0;
          }
          
         

//          LQ_Printf("%c\n", g_uart_interrupt_buf[0]);

          break;

      case CY_SCB_UART_RB_FULL_EVENT:
          break;

      case CY_SCB_UART_RECEIVE_ERR_EVENT:
          break;

      case CY_SCB_UART_TRANSMIT_ERR_EVENT:
          break;

      default:
          break;
    }
}

//-----------------------------------------------------------------------//

//测试北斗 串口0回调
void Scb0_UART_Event(uint32_t locEvents)
{
  static int numbd;
  switch (locEvents)
  {
     case CY_SCB_UART_RECEIVE_DONE_EVENT:
       /* 用户代码 */
        Cy_SCB_UART_Receive(SCB0, &g_uart_interrupt_buf0[0], size, &uartx_context[0]);
	bufbd = g_uart_interrupt_buf0[0]; 
//        LQ_Printf0("%c",bufbd);
//        Cy_SCB_UART_PutArray(SCB0, (void*)bufbd, strlen((const char*)bufbd));
        if(bufbd == '\n'||bufbd == '\r')  //一帧数据接收完成
	{

//	    strcpy((char*)Save_Data.GPS_Buffer,(const char*)ReadBuff0);
            sprintf((char*)Save_Data.GPS_Buffer,"%s", ((const char*)ReadBuff0));
	    Save_Data.isGetData=1;  //接收完成标志位
	    numbd = 0;//            
//            LQ_Printf0("ReadBuff0:%s;\n",ReadBuff0);
//            LQ_Printf0("Save_Data.GPS_Buffer:%s;\n",Save_Data.GPS_Buffer);
	}
	else
	{
	    ReadBuff0[numbd++] = bufbd;   //一个一个存进去
	}   
        
   break;
   
   
  }
}
//-----------------------------------------------------------------------//
void Scb1_UART_Event(uint32_t locEvents){}
//-----------------------------------------------------------------------//
void Scb2_UART_Event(uint32_t locEvents){}
//-----------------------------------------------------------------------//
void Scb3_UART_Event(uint32_t locEvents){}
//-----------------------------------------------------------------------//
void Scb4_UART_Event(uint32_t locEvents){}
//-----------------------------------------------------------------------//
void Scb5_UART_Event(uint32_t locEvents){}
//-----------------------------------------------------------------------//
void Scb6_UART_Event(uint32_t locEvents){}
//-----------------------------------------------------------------------//
/*遥控器 串口8回调*/
void Scb8_UART_Event(uint32_t locEvents)
{
  /*    SBUS数据解析  */
  static int Snum=0;
  unsigned char buf;
  switch (locEvents)
  {
     case CY_SCB_UART_RECEIVE_DONE_EVENT:
        /* 用户代码 */
        Cy_SCB_UART_Receive(SCB8, &g_uart_interrupt_buf[0], size, &uartx_context[8]);
        buf = g_uart_interrupt_buf[0];
        
    
        if(Usart8_Rec_Fini_Flag == 1)       //可以接收
        {
          if (Snum == 0 && buf == 0x0F)   //帧头
          {
            ReadBuff8[Snum] = buf;
            Snum++;
            LED_Ctrl(LED1, RVS);        // LED闪烁 指示程序运行状态
          }
          else if(Snum != 0)
          {
            LED_Ctrl(LED0, RVS);        // LED闪烁 指示程序运行状态
            ReadBuff8[Snum] = buf;
            Snum++;
            if (Snum > 24)              //接收数据超过24，表示一帧接收完成
            {
              Usart8_Rec_Fini_Flag = 0;   //停止接收

              if (ReadBuff8[0] == 0x0F && ReadBuff8[24] == 0x00 && Snum == 25)    //接受完一帧数据
              {
                  update_sbus();      //数据解析
                  LED_Ctrl(LED0, RVS);
              }
              Snum = 0;               //清空
            }

          }
        }
      break;
  }
}

/*串口9回调*/
void Scb9_UART_Event(uint32_t locEvents){}


/*******************************以下为初始化等函数********************************************/

/*************************************************************************
*  函数名称：LQ_UART_Init_Config(GPIO_UART_RX_t RX_Pin, GPIO_UART_TX_t TX_Pin, uint32_t boadrate)
*  功能说明：初始化串口
*  参数说明：RX_Pin：串口RX引脚
            TX_Pin：串口TX引脚
            boadrate：波特率
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
void LQ_UART_Init_Config(GPIO_UART_RX_t RX_Pin, GPIO_UART_TX_t TX_Pin, uint32_t boadrate)
{
  
    /*获取GPIO端口以及引脚*/
    uint8_t Rx_port, Rx_pin, Tx_port, Tx_pin;
    
    Rx_port = (uint8_t)UART_GetModule(RX_Pin);
    Rx_pin  = (uint8_t)UART_GetIndex(RX_Pin);
    Tx_port = (uint8_t)UART_GetModule(TX_Pin);
    Tx_pin  = (uint8_t)UART_GetIndex(TX_Pin);
    
    /*获取引脚复用信息*/
    uint8_t act;
    act = (uint8_t)UART_GetHsiom(TX_Pin);
    
    /*获取串口号*/
    uint32_t chl = (uint32_t)UART_GetChl(TX_Pin);
    volatile stc_SCB_t* UART_TYPE = SCB_x[chl];
    
    /*获取串口时钟*/
    uint16_t pclk = (uint16_t)PCLK_SCB0_CLOCK+(uint16_t)chl;
    uint16_t irqn;
    /*获取串口中断号*/
    if(chl == 0)
    {
      irqn = (uint16_t)scb_0_interrupt_IRQn;
    }else{
      irqn = (uint16_t)scb_1_interrupt_IRQn+UART_GetChl(TX_Pin)-1;
    }
    /* Port Configuration for UART */
    cy_stc_gpio_pin_config_t    stc_port_pin_cfg_uart = {0ul};
    stc_port_pin_cfg_uart.driveMode = CY_GPIO_DM_HIGHZ;
    stc_port_pin_cfg_uart.hsiom     = (en_hsiom_sel_t)act;
    Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[Rx_port]), Rx_pin, &stc_port_pin_cfg_uart);

    stc_port_pin_cfg_uart.driveMode = CY_GPIO_DM_STRONG_IN_OFF;
    stc_port_pin_cfg_uart.hsiom     = (en_hsiom_sel_t)act;
    Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[Tx_port]), Tx_pin, &stc_port_pin_cfg_uart);

    cy_stc_scb_uart_config_t g_stc_uart_config =
    {
        .uartMode                   = CY_SCB_UART_STANDARD,
        .oversample                 = UART_OVERSAMPLING,
        .dataWidth                  = 8ul,                              //8位数据位
        .enableMsbFirst             = false,
        .stopBits                   = CY_SCB_UART_STOP_BITS_1,          //1位停止位
        .parity                     = CY_SCB_UART_PARITY_NONE,          //无校验位
        .enableInputFilter          = false,
        .dropOnParityError          = false,
        .dropOnFrameError           = false,
        .enableMutliProcessorMode   = false,
        .receiverAddress            = 0ul,
        .receiverAddressMask        = 0ul,
        .acceptAddrInFifo           = false,
        .irdaInvertRx               = false,
        .irdaEnableLowPowerReceiver = false,
        .smartCardRetryOnNack       = false,
        .enableCts                  = false,
        .ctsPolarity                = CY_SCB_UART_ACTIVE_LOW,
        .rtsRxFifoLevel             = 0ul,
        .rtsPolarity                = CY_SCB_UART_ACTIVE_LOW,
        .breakWidth                 = 0ul,
        .rxFifoTriggerLevel         = 0ul,
        .rxFifoIntEnableMask        = 0ul,
        .txFifoTriggerLevel         = 0ul,
        .txFifoIntEnableMask        = 0ul
    };

    /* SCB-UART Initialization */
    Cy_SCB_UART_DeInit((volatile stc_SCB_t*)UART_TYPE);
    Cy_SCB_UART_Init((volatile stc_SCB_t*)UART_TYPE, &g_stc_uart_config, &uartx_context[chl]);
    switch(chl)
    {
      case 0:Cy_SCB_UART_RegisterCallback((volatile stc_SCB_t*)UART_TYPE, Scb0_UART_Event, &uartx_context[chl]);break;
      case 1:Cy_SCB_UART_RegisterCallback((volatile stc_SCB_t*)UART_TYPE, Scb1_UART_Event, &uartx_context[chl]);break;
      case 2:Cy_SCB_UART_RegisterCallback((volatile stc_SCB_t*)UART_TYPE, Scb2_UART_Event, &uartx_context[chl]);break;
      case 3:Cy_SCB_UART_RegisterCallback((volatile stc_SCB_t*)UART_TYPE, Scb3_UART_Event, &uartx_context[chl]);break;
      case 4:Cy_SCB_UART_RegisterCallback((volatile stc_SCB_t*)UART_TYPE, Scb4_UART_Event, &uartx_context[chl]);break;
      case 5:Cy_SCB_UART_RegisterCallback((volatile stc_SCB_t*)UART_TYPE, Scb5_UART_Event, &uartx_context[chl]);break;
      case 6:Cy_SCB_UART_RegisterCallback((volatile stc_SCB_t*)UART_TYPE, Scb6_UART_Event, &uartx_context[chl]);break;
      case 7:Cy_SCB_UART_RegisterCallback((volatile stc_SCB_t*)UART_TYPE, Scb7_UART_Event, &uartx_context[chl]);break;
      case 8:Cy_SCB_UART_RegisterCallback((volatile stc_SCB_t*)UART_TYPE, Scb8_UART_Event, &uartx_context[chl]);break;
      case 9:Cy_SCB_UART_RegisterCallback((volatile stc_SCB_t*)UART_TYPE, Scb9_UART_Event, &uartx_context[chl]);break;
    }
    
    Cy_SCB_UART_Enable((volatile stc_SCB_t*)UART_TYPE);

    /* Clock Configuration for UART */
    // Assign a programmable divider
    Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)pclk, CY_SYSCLK_DIV_24_5_BIT, SCBx_Clk[chl]);
    // Set divider value
    {
        uint64_t targetFreq     = UART_OVERSAMPLING * boadrate;
        uint64_t sourceFreq_fp5 = ((uint64_t)80000000ul << 5ull);
        uint32_t divSetting_fp5 = (uint32_t)(sourceFreq_fp5 / targetFreq);
        Cy_SysClk_PeriphSetFracDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)pclk),
                                       CY_SYSCLK_DIV_24_5_BIT,
                                       SCBx_Clk[chl],
                                       ((divSetting_fp5 & 0x1FFFFFE0ul) >> 5ul),
                                       (divSetting_fp5 & 0x0000001Ful));
    }
    // Enable peripheral divider
    Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)pclk), CY_SYSCLK_DIV_24_5_BIT, SCBx_Clk[chl]);

    /* 串口中断配置 */
    cy_stc_sysint_irq_t stc_sysint_irq_cfg_uart;
    stc_sysint_irq_cfg_uart.sysIntSrc = (cy_en_intr_t)irqn;
    stc_sysint_irq_cfg_uart.intIdx    = CPUIntIdx2_IRQn;
    stc_sysint_irq_cfg_uart.isEnabled = true;
    Cy_SysInt_InitIRQ(&stc_sysint_irq_cfg_uart);
    switch(chl)
    {
      case 0:Cy_SysInt_SetSystemIrqVector(stc_sysint_irq_cfg_uart.sysIntSrc, Scb0_UART_IntrISR);break;
      case 1:Cy_SysInt_SetSystemIrqVector(stc_sysint_irq_cfg_uart.sysIntSrc, Scb1_UART_IntrISR);break;
      case 2:Cy_SysInt_SetSystemIrqVector(stc_sysint_irq_cfg_uart.sysIntSrc, Scb2_UART_IntrISR);break;
      case 3:Cy_SysInt_SetSystemIrqVector(stc_sysint_irq_cfg_uart.sysIntSrc, Scb3_UART_IntrISR);break;
      case 4:Cy_SysInt_SetSystemIrqVector(stc_sysint_irq_cfg_uart.sysIntSrc, Scb4_UART_IntrISR);break;
      case 5:Cy_SysInt_SetSystemIrqVector(stc_sysint_irq_cfg_uart.sysIntSrc, Scb5_UART_IntrISR);break;
      case 6:Cy_SysInt_SetSystemIrqVector(stc_sysint_irq_cfg_uart.sysIntSrc, Scb6_UART_IntrISR);break;
      case 7:Cy_SysInt_SetSystemIrqVector(stc_sysint_irq_cfg_uart.sysIntSrc, Scb7_UART_IntrISR);break;
      case 8:Cy_SysInt_SetSystemIrqVector(stc_sysint_irq_cfg_uart.sysIntSrc, Scb8_UART_IntrISR);break;
      case 9:Cy_SysInt_SetSystemIrqVector(stc_sysint_irq_cfg_uart.sysIntSrc, Scb9_UART_IntrISR);break;
    }
    
    NVIC_EnableIRQ(stc_sysint_irq_cfg_uart.intIdx);
    Cy_SCB_UART_Receive((volatile stc_SCB_t*)UART_TYPE, &g_uart_interrupt_buf[0], size, &uartx_context[chl]);
}

/*************************************************************************
*  函数名称：void UART_PutChar(UART_t  uratn, char ch)
*  功能说明：UART发送字节函数,使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ ch    ： 要打印的字符
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutChar(UART0, 'a');  //打印字符a
*************************************************************************/
void UART_PutChar(volatile stc_SCB_t *base, char ch)
{
  Cy_SCB_UART_Put(base, ch);
}
/*************************************************************************
*  函数名称：void UART_PutBuff(UART_t  uratn, unsigned char *buff, unsigned long len)
*  功能说明：UART发送字节函数,使用前请先初始化对应串口
*  参数说明：
* @param    uratn ： UART0 - UART3
* @param    buff  ： 要打印的字符串地址
* @param    len   ： 要打印的长度
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutBuff(UART4, "123456789",5);//实际发送5个字节‘1’‘2’‘3’‘4’‘5’
*************************************************************************/
void UART_PutBuff(volatile stc_SCB_t *base, unsigned char *buff, unsigned long len)
{
    while(len--)
    {
        UART_PutChar(base, *buff);
        buff++;
    }
}

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】vvoid ANO_DT_send_int16(short data1,..., short data8 )
【功  能】匿名上位机用户协议，使用时占用MCU资源较大，跑车时不使用
【参数值】short data1,..., short data8, 待发送的数据
【返回值】无 
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ANO_DT_send_int16byte16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 )
{
  unsigned char data_to_send[50];
  unsigned char  _cnt=0;
  unsigned char  sum = 0, i = 0;
  
  data_to_send[_cnt++] = 0xAA;      //匿名协议帧头  0xAAAA
  data_to_send[_cnt++] = 0xAA;
  data_to_send[_cnt++] = 0xF1;      //使用用户协议帧0xF1
  data_to_send[_cnt++] = 16;        //8个int16_t 长度 16个字节
  
  data_to_send[_cnt++]=(unsigned short)(data1>>8);
  data_to_send[_cnt++]=(unsigned char)(data1);
  
  data_to_send[_cnt++]=(unsigned short)(data2>>8);
  data_to_send[_cnt++]=(unsigned char)(data2);
  
  data_to_send[_cnt++]=(unsigned short)(data3>>8);
  data_to_send[_cnt++]=(unsigned char)(data3);
  
  data_to_send[_cnt++]=(unsigned short)(data4>>8);
  data_to_send[_cnt++]=(unsigned char)(data4);
  
  data_to_send[_cnt++]=(unsigned short)(data5>>8);
  data_to_send[_cnt++]=(unsigned char)(data5);
  
  data_to_send[_cnt++]=(unsigned short)(data6>>8);
  data_to_send[_cnt++]=(unsigned char)(data6);
  
  data_to_send[_cnt++]=(unsigned short)(data7>>8);
  data_to_send[_cnt++]=(unsigned char)(data7);
  
  data_to_send[_cnt++]=(unsigned short)(data8>>8);
  data_to_send[_cnt++]=(unsigned char)(data8);
  
  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++]=sum;
  
  UART_PutBuff(Printf_Uartx, data_to_send, _cnt);     //可以修改不同的串口发送数据;
}

/*************************************************************************
*  函数名称：LQ_Printf(void *fmt, ...)
*  功能说明：串口输出函数
*  参数说明：与printf（）函数一样的用法
            
            
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：LQ_Printf("data:%d\n",data);若data=2，则打印：data:2
  修改宏定义“Printf_Uartx”配置不同的串口输出
*************************************************************************/
void LQ_Printf(void *fmt, ...)
{
    uint8_t uart_out_data[128];
    va_list arg;

    /* UART Print */
    va_start(arg, fmt);
    vsprintf((char*)&uart_out_data[0], (char*)fmt, arg);
    while (Cy_SCB_UART_IsTxComplete(Printf_Uartx) != true) {};
    Cy_SCB_UART_PutArray(Printf_Uartx, uart_out_data, strlen((char *)uart_out_data));
    va_end(arg);//修改宏定义“Printf_Uartx0
}
/*************************************************************************
*  函数名称：LQ_Printf0(void *fmt, ...)
*  功能说明：串口输出函数
*  参数说明：与printf（）函数一样的用法
            
            
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：LQ_Printf("data:%d\n",data);若data=2，则打印：data:2
  修改宏定义“Printf_Uartx0”配置不同的串口输出
  与上面功能一样
*************************************************************************/
void LQ_Printf0(void *fmt, ...)
{
    uint8_t uart_out_data[128];
    va_list arg;

    /* UART Print */
    va_start(arg, fmt);
    vsprintf((char*)&uart_out_data[0], (char*)fmt, arg);
    while (Cy_SCB_UART_IsTxComplete(Printf_Uartx0) != true) {};
    Cy_SCB_UART_PutArray(Printf_Uartx0, uart_out_data, strlen((char *)uart_out_data));
    va_end(arg);
}
/*************************************************************************
*  函数名称：Test_Bluetooth(void)
*  功能说明：蓝牙串口接口测试函数
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：测试母板蓝牙接口，同下载器接口处的串口（串口7）

*************************************************************************/
void Test_Bluetooth(void)
{
  uint8_t cnt=0;
  GPIO_LED_Init();
  LQ_UART_Init_Config(UART7_RX_P23_0, UART7_TX_P23_1, 115200);

  LQ_Printf("\nUART TEST (driver ver=%d.%d)\n\r", CY_SCB_DRV_VERSION_MAJOR, CY_SCB_DRV_VERSION_MINOR);
  while(1)
  {
    LQ_Printf("Long Qiu Ke Ji %d\n", cnt++);

//    ANO_DT_send_int16byte16(cnt, cnt*2, cnt*3, cnt*4, cnt*5, cnt*6, cnt*7, cnt*8);
    cnt++;
    LED_Ctrl(LED0, RVS);
    Cy_SysTick_DelayInUs(50000);
  }
}








