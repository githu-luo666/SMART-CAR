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

/************************待调****************************/
/************************待调****************************/
/************************待调****************************/

#include "LQ_Hard_SPI.h"
#include "LQ_UART.h"

#define OVERSAMPLING  16ul

cy_stc_scb_spi_context_t spix_context[10];

uint16_t readData[2];      //数据缓存区

void irqSCB(void)
{
    uint32_t status;
    status = Cy_SCB_SPI_GetRxFifoStatus(CY_SPI_SCB_TYPE);
    if (status & CY_SCB_SPI_RX_TRIGGER)
    {
        Cy_SCB_SPI_ReadArray(CY_SPI_SCB_TYPE, (void *)readData, 2ul);
        Cy_SCB_SPI_ClearRxFifoStatus(CY_SPI_SCB_TYPE, CY_SCB_SPI_RX_TRIGGER);
    }
}


void SetPeripheFracDiv24_5(uint64_t targetFreq, uint64_t sourceFreq, uint8_t divNum)
{
    uint64_t temp = ((uint64_t)sourceFreq << 5ull);
    uint32_t divSetting;

    divSetting = (uint32_t)(temp / targetFreq);
    Cy_SysClk_PeriphSetFracDivider(Cy_SysClk_GetClockGroup(CY_SPI_SCB_PCLK), 
                                   CY_SYSCLK_DIV_24_5_BIT, divNum, 
                                   (((divSetting >> 5ul) & 0x00000FFFul) - 1ul), 
                                   (divSetting & 0x0000001Ful));
}

void LQ_SPI_InitConfig(GPIO_SPI_CLK_t CLK, GPIO_SPI_MISO_t MISO, GPIO_SPI_MOSI_t MOSI, GPIO_SPI_SEL_t SEL, uint32_t BAUDRATE)
{
#if (CY_USE_PSVP == 1)  
    uint32_t SOURCE_CLOCK_FRQ = 24000000ul;
#else                                     
//    uint32_t SOURCE_CLOCK_FRQ = 80000000ul;
      uint32_t SOURCE_CLOCK_FRQ = 24000000ul;
#endif
    
    uint32_t SPI_CLOCK_FREQ = OVERSAMPLING * BAUDRATE;
    /*获取GPIO端口以及引脚*/
    uint8_t MOSI_port, MOSI_pin, MISO_port, MISO_pin, CLK_port, CLK_pin, SEL_port, SEL_pin;
    uint8_t Slave_Select;
    uint8_t act;
    
    MOSI_port           = (uint8_t)SPI_GetModule(MOSI);
    MOSI_pin            = (uint8_t)SPI_GetIndex(MOSI);
    MISO_port           = (uint8_t)SPI_GetModule(MISO);
    MISO_pin            = (uint8_t)SPI_GetIndex(MISO);
    CLK_port            = (uint8_t)SPI_GetModule(CLK);
    CLK_pin             = (uint8_t)SPI_GetIndex(CLK);
    SEL_port            = (uint8_t)SPI_GetModule(SEL);
    SEL_pin             = (uint8_t)SPI_GetIndex(SEL);
    act                 = (uint8_t)SPI_GetHsiom(CLK);
    Slave_Select        = (uint8_t)SPI_GetSEL(SEL);
      
    uint8_t chl;
    chl = (uint8_t)SPI_GetChl(CLK);
    
    /*获取SCBx时钟*/
    uint16_t pclk = (uint16_t)PCLK_SCB0_CLOCK+(uint16_t)chl;
    
    /******* Calculate divider setting for the SCB ********/
    Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)pclk, CY_SYSCLK_DIV_24_5_BIT, SCBx_Clk[chl]);
    SetPeripheFracDiv24_5(SPI_CLOCK_FREQ, SOURCE_CLOCK_FRQ, SCBx_Clk[chl]);
    Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)pclk), CY_SYSCLK_DIV_24_5_BIT, SCBx_Clk[chl]);

    
    static const cy_stc_scb_spi_config_t SCB_SPI_cfg =
    {
      .spiMode = CY_SCB_SPI_MASTER,       /*** 指定操作模式   ***/
      .subMode = CY_SCB_SPI_MOTOROLA,     /*** 指定SPI操作的子模式    ***/
      .sclkMode = CY_SCB_SPI_CPHA0_CPOL0, /*** SPI SCLk的模式选择，时钟为低电平有效，数据在第一个边沿改变 ***/
      .oversample = OVERSAMPLING,         /*** SPI_CLOCK除以OVERSAMPLATION应为波特率  ***/
      .rxDataWidth = 8ul,                /*** RX数据的宽度（有效范围为4—16）。它必须与\ref txDataWidth相同，除了在National 模式中。 ***/
      .txDataWidth = 8ul,                /*** TX数据的宽度（有效范围为4—16）。它必须与\ref rxDataWidth相同，但在National子模式中除外. ***/
      .enableMsbFirst = true,             /*** 使硬件能够先移出数据元素MSB，否则先移出LSB ***/
      .enableFreeRunSclk = false,         /*** 使硬件能够先移出数据元素MSB，否则先移出LSB  ***/
      .enableInputFilter = false,         /*** 使数字3 中值滤波器应用于RX FIFO的输入，以滤除线路上的毛刺. ***/
      .enableMisoLateSample = true,       /*** 使主机能够在半个时钟后对MISO线进行采样，以实现更好的定时。 ***/
      .enableTransferSeperation = true,   /*** 使主设备能够传输由从设备选择线的取消断言分隔的每个数据元素（仅适用于主设备模式） ***/
      .ssPolarity0 = false,               /*** SS0: active low ***/
      .ssPolarity1 = false,               /*** SS1: active low ***/
      .ssPolarity2 = false,               /*** SS2: active low ***/
      .ssPolarity3 = false,               /*** SS3: active low ***/
      .enableWakeFromSleep = false,       /*** 设置后，从设备将在从设备选择线变为活动时唤醒设备。请注意，并非所有SCB都支持此模式。查阅设备数据表以确定哪些SCB支持从深度睡眠中唤醒. ***/
      .rxFifoTriggerLevel = 0ul,          /*** 当RX FIFO中有2个以上的条目时，发生中断 */
      .rxFifoIntEnableMask = 0ul,         /*** 此掩码中设置的位将允许事件导致中断 */
      .txFifoTriggerLevel = 0ul,          /*** 当TX FIFO中的条目较少时，则在该级别，TX触发器输出变高。该输出可以通过触发多路复用器连接到DMA通道。此外，它还控制\ref CY_SCB_SPI_TX_TRIGGER中断源。 */
      .txFifoIntEnableMask = 0ul,         /*** 此掩码中设置的位允许事件导致中断  */
      .masterSlaveIntEnableMask = 0ul,    /*** 此掩码中设置的位允许事件导致中断  */
      .enableSpiDoneInterrupt = false,
      .enableSpiBusErrorInterrupt = false,
    };


    
    /*   SCB外围器件的默认初始化    */
    Cy_SCB_SPI_DeInit(SCB_x[chl]);

//    /* SPI通信中断设置 */
//    static cy_stc_sysint_irq_t irq_cfg =
//        {
//            .sysIntSrc = CY_SPI_SCB_IRQN,
//            .intIdx = CPUIntIdx3_IRQn,
//            .isEnabled = true,
//        };
//    Cy_SysInt_InitIRQ(&irq_cfg); // cy_systemIntr_Handler
//    Cy_SysInt_SetSystemIrqVector(irq_cfg.sysIntSrc, irqSCB);
//    NVIC_EnableIRQ(irq_cfg.intIdx);

    /* SPI通信的端口设置 */
    /* 根据硬件环境更改SCB CH*/
    static cy_stc_gpio_pin_config_t SPI_port_pin_cfg ={0ul};
    SPI_port_pin_cfg.driveMode = SCB_MISO_DRIVE_MODE;
    SPI_port_pin_cfg.hsiom = (en_hsiom_sel_t)act;
    Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[MISO_port]), MISO_pin, &SPI_port_pin_cfg);

    SPI_port_pin_cfg.driveMode = SCB_MOSI_DRIVE_MODE;
    SPI_port_pin_cfg.hsiom = (en_hsiom_sel_t)act;
    Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[MOSI_port]), MOSI_pin, &SPI_port_pin_cfg);

    SPI_port_pin_cfg.driveMode = SCB_CLK_DRIVE_MODE;
    SPI_port_pin_cfg.hsiom = (en_hsiom_sel_t)act;
    Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[CLK_port]), CLK_pin, &SPI_port_pin_cfg);

    SPI_port_pin_cfg.driveMode = SCB_SEL0_DRIVE_MODE;
    SPI_port_pin_cfg.hsiom = (en_hsiom_sel_t)act;
    Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t*) &GPIO->PRT[SEL_port]), SEL_pin, &SPI_port_pin_cfg);

    /* SPI通信的SCB初始化 */
    Cy_SCB_SPI_Init(SCB_x[chl], &SCB_SPI_cfg, &spix_context[chl]);
    Cy_SCB_SPI_SetActiveSlaveSelect(SCB_x[chl], Slave_Select);
    Cy_SCB_SPI_Enable(SCB_x[chl]);

}
uint32_t LQ_SPI_WriteArray(volatile stc_SCB_t *base, void *txBuf, uint32_t len)
{
    /* Get free entries in TX FIFO */
    uint32_t numToCopy = Cy_SCB_GetFifoSize(base) - Cy_SCB_GetNumInTxFifo(base);

    /* Adjust the data elements to write */
    if (numToCopy > len)
    {
        numToCopy = len;
    }

    Cy_SCB_WriteArrayNoCheck(base, txBuf, numToCopy);

    return (numToCopy);
}

uint32_t LQ_SPI_ReadArray(volatile stc_SCB_t const *base, void *rxBuf, uint32_t len)
{
    /* Get available items in RX FIFO */
    uint32_t numToCopy = Cy_SCB_GetNumInRxFifo(base);

    /* Adjust items which will be read */
    if (numToCopy > len)
    {
        numToCopy = len;
    }

    /* Get data available in RX FIFO */
    Cy_SCB_ReadArrayNoCheck(base, rxBuf, numToCopy);

    return (numToCopy);
}

/*******************************************************************************/

void Test_SPI(void)
{
  
  readData[0] = 0xAAAA;
  readData[1] = 0xBBBB;
  
  LQ_SPI_InitConfig(SCB5_CLK_P07_2, SCB5_MISO_P07_0, SCB5_MOSI_P07_1, SCB5_SEL0_P07_3, 125000);
  
  Cy_SCB_SPI_WriteArray(CY_SPI_SCB_TYPE,(void*)readData, 2);
  
  
//  
//  LQ_Printf("SCB_SPI_Test ing...\n");
}

//void LQ_SCB_SPI_Transfer(GPIO_SPI_MISO_t MOSI, void *txBuf, void *rxBuf, uint32_t size)
//{
//  uint8_t chl;
//  chl = (uint8_t)SPI_GetChl(MOSI);
//  Cy_SCB_SPI_Transfer(SCB_x[chl], txBuf, rxBuf, size, &spix_context[chl]);
//}
/*******************************************************************************/












