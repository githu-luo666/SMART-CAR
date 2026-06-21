#include "LQ_SPI.h"
#include "LQ_UART.h"

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

void LQ_SPI_InitConfig(void)
{

    static const cy_stc_scb_spi_config_t SCB_SPI_cfg =
        {
            .spiMode = CY_SCB_SPI_MASTER,       /*** 指定操作模式   ***/
            .subMode = CY_SCB_SPI_MOTOROLA,     /*** 指定SPI操作的子模式    ***/
            .sclkMode = CY_SCB_SPI_CPHA0_CPOL0, /*** SPI SCLk的模式选择，时钟为低电平有效，数据在第一个边沿改变 ***/
            .oversample = 16ul,                 /*** SPI_CLOCK除以SCB_SPI_OVERSAMPLATION应为波特率  ***/
            .rxDataWidth = 16ul,                /*** RX数据的宽度（有效范围为4—16）。它必须与\ref txDataWidth相同，除了在National 模式中。 ***/
            .txDataWidth = 16ul,                /*** TX数据的宽度（有效范围为4—16）。它必须与\ref rxDataWidth相同，但在National子模式中除外. ***/
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
            .rxFifoTriggerLevel = 1ul,          /*** 当RX FIFO中有2个以上的条目时，发生中断 */
            .rxFifoIntEnableMask = 1ul,         /*** 此掩码中设置的位将允许事件导致中断 */
            .txFifoTriggerLevel = 0ul,          /*** 当TX FIFO中的条目较少时，则在该级别，TX触发器输出变高。该输出可以通过触发多路复用器连接到DMA通道。此外，它还控制\ref CY_SCB_SPI_TX_TRIGGER中断源。 */
            .txFifoIntEnableMask = 0ul,         /*** 此掩码中设置的位允许事件导致中断  */
            .masterSlaveIntEnableMask = 0ul,    /*** 此掩码中设置的位允许事件导致中断  */
            .enableSpiDoneInterrupt = false,
            .enableSpiBusErrorInterrupt = false,
        };

    /******* Calculate divider setting for the SCB ********/
    Cy_SysClk_PeriphAssignDivider(CY_SPI_SCB_PCLK, CY_SYSCLK_DIV_24_5_BIT, 1u);
    // SetPeripheFracDiv24_5(SCB_SPI_CLOCK_FREQ, SOURCE_CLOCK_FRQ, DIVIDER_NO_1);
    {
        uint32_t divSetting;
        uint64_t temp = ((uint64_t)80000000ul << 5ull);
        divSetting = (uint32_t)(temp / 2000000ul);
        Cy_SysClk_PeriphSetFracDivider(Cy_SysClk_GetClockGroup(CY_SPI_SCB_PCLK),
                                       CY_SYSCLK_DIV_24_5_BIT, 1ul,
                                       (((divSetting >> 5ul) & 0x00000FFFul) - 1ul),
                                       (divSetting & 0x0000001Ful));
    }
    Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup(CY_SPI_SCB_PCLK), CY_SYSCLK_DIV_24_5_BIT, 1u);

    __enable_irq(); /* 使能全局中断. */

    /*   SCB外围器件的去初始化    */
    Cy_SCB_SPI_DeInit(CY_SPI_SCB_TYPE);

    /* SPI通信中断设置 */
    static cy_stc_sysint_irq_t irq_cfg =
        {
            .sysIntSrc = CY_SPI_SCB_IRQN,
            .intIdx = CPUIntIdx3_IRQn,
            .isEnabled = true,
        };
    Cy_SysInt_InitIRQ(&irq_cfg); // cy_systemIntr_Handler
    Cy_SysInt_SetSystemIrqVector(irq_cfg.sysIntSrc, irqSCB);
    NVIC_EnableIRQ(irq_cfg.intIdx);

    /* SPI通信的端口设置 */
    /* 根据硬件环境更改SCB CH*/
    static cy_stc_gpio_pin_config_t SPI_port_pin_cfg ={0ul};
    SPI_port_pin_cfg.driveMode = SCB_MISO_DRIVE_MODE;
    SPI_port_pin_cfg.hsiom = CY_SPI_SCB_MISO_MUX;
    Cy_GPIO_Pin_Init(CY_SPI_SCB_MISO_PORT, CY_SPI_SCB_MISO_PIN, &SPI_port_pin_cfg);

    SPI_port_pin_cfg.driveMode = SCB_MOSI_DRIVE_MODE;
    SPI_port_pin_cfg.hsiom = CY_SPI_SCB_MOSI_MUX;
    Cy_GPIO_Pin_Init(CY_SPI_SCB_MOSI_PORT, CY_SPI_SCB_MOSI_PIN, &SPI_port_pin_cfg);

    SPI_port_pin_cfg.driveMode = SCB_CLK_DRIVE_MODE;
    SPI_port_pin_cfg.hsiom = CY_SPI_SCB_CLK_MUX;
    Cy_GPIO_Pin_Init(CY_SPI_SCB_CLK_PORT, CY_SPI_SCB_CLK_PIN, &SPI_port_pin_cfg);

    SPI_port_pin_cfg.driveMode = SCB_SEL0_DRIVE_MODE;
    SPI_port_pin_cfg.hsiom = CY_SPI_SCB_SEL0_MUX;
    Cy_GPIO_Pin_Init(CY_SPI_SCB_SEL0_PORT, CY_SPI_SCB_SEL0_PIN, &SPI_port_pin_cfg);

    /* SPI通信的SCB初始化 */
    Cy_SCB_SPI_Init(CY_SPI_SCB_TYPE, &SCB_SPI_cfg, NULL);
    Cy_SCB_SPI_SetActiveSlaveSelect(CY_SPI_SCB_TYPE, 0ul);
    Cy_SCB_SPI_Enable(CY_SPI_SCB_TYPE);

}


/*******************************************************************************/

void Test_SPI(void)
{
    /********************************************/
    /*      将初始值写入缓冲区     */
    /********************************************/
    readData[0] = 0xAAAA;
    readData[1] = 0xBBBB;
  
  LQ_SPI_InitConfig();
  
  Cy_SCB_SPI_WriteArray(CY_SPI_SCB_TYPE,(void*)readData, 2);
  
//  
//  LQ_Printf("SCB_SPI_Test ing...\n");
}

/*******************************************************************************/












