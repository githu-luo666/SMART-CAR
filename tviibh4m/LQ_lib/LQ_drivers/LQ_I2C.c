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

#include "LQ_I2C.h"


/* SCB - I2C Configuration */

cy_stc_scb_i2c_context_t Scb_I2C_Context[10];
uint32_t I2c_Scb;
volatile stc_SCB_t *SCB_n[10] = {SCB0, SCB1, SCB2, SCB3, SCB4, SCB5, SCB6, SCB7, SCB8, SCB9};



/* Scb_I2C_Master 高级 API 会用到这个结构体，使用时去掉注释，在头文件中自行配置从机地址  */
 cy_stc_scb_i2c_master_xfer_config_t g_stc_i2c_master_config =
{
     .slaveAddress = Slave_Addr,
     .buffer       = 0,
     .bufferSize   = 0,
     .xferPending  = false
};

void LQ_I2C_Init_Config(I2C_SDA_Pin SDA_Pin, I2C_SCL_Pin SCL_Pin)
{
    /*获取GPIO端口以及引脚*/
    uint8_t Sda_port, Sda_pin, Scl_port, Scl_pin;

    Sda_port = (uint8_t)I2C_GetPort(SDA_Pin);
    Sda_pin = (uint8_t)I2C_GetIndex(SDA_Pin);
    Scl_port = (uint8_t)I2C_GetPort(SCL_Pin);
    Scl_pin = (uint8_t)I2C_GetIndex(SCL_Pin);

    /*获取引脚复用信息*/
    // uint8_t SCL_hsiom = (uint8_t)I2C_GetHsiom(SCL_Pin);
    // uint8_t SDA_hsiom = (uint8_t)I2C_GetHsiom(SDA_Pin);
    uint8_t i2c_SCB_hsiom = (uint8_t)I2C_GetHsiom(SDA_Pin); // 同一组SCB，I2C管脚的scb_hsiom 相同,下同

    /*获取I2C_scb号*/
    uint32_t I2c_SCB = (uint32_t)I2C_GetSCB(SDA_Pin);
//    volatile stc_SCB_t *I2C_SCB_TYPE = SCB_n[I2c_SCB];//和数组里的一样，任用一个

    /*获取I2C_SCB时钟*/
    uint16_t I2C_pclk = (uint16_t)PCLK_SCB0_CLOCK + I2C_GetSCB(SCL_Pin);
    /*获取I2C_SCB中断号*/
    uint16_t SCB_IrQn;
    if(I2c_SCB==0)
    {  
      SCB_IrQn=(uint16_t)scb_0_interrupt_IRQn;
    }else
    {
      SCB_IrQn = (uint16_t)scb_1_interrupt_IRQn + I2C_GetSCB(SCL_Pin)-1;
    }
    /* 1 Clock Configuration */
    Cy_SysClk_PeriphAssignDivider((en_clk_dst_t)I2C_pclk, CY_SYSCLK_DIV_24_5_BIT, 0ul);
//    SetPeripheFracDiv24_5(E_I2C_INCLK_TARGET_FREQ, E_SOURCE_CLK_FREQ, 0ul, (en_clk_dst_t)I2C_pclk);
   {
      uint64_t temp = ((uint64_t)E_SOURCE_CLK_FREQ << 5ull);
      uint32_t divSetting = (uint32_t)(temp / E_I2C_INCLK_TARGET_FREQ);
      Cy_SysClk_PeriphSetFracDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)I2C_pclk),
                                   CY_SYSCLK_DIV_24_5_BIT, 0ul,
                                   (((divSetting >> 5u) & 0x00000FFFul) - 1ul),
                                   (divSetting & 0x0000001Ful));
    }    
    Cy_SysClk_PeriphEnableDivider(Cy_SysClk_GetClockGroup((en_clk_dst_t)I2C_pclk), CY_SYSCLK_DIV_24_5_BIT, 0ul);
     
    /* 2  Port Configuration */
    cy_stc_gpio_pin_config_t stc_port_pin_cfg_i2c = {0ul};
    stc_port_pin_cfg_i2c.driveMode = CY_GPIO_DM_OD_DRIVESLOW;
    stc_port_pin_cfg_i2c.hsiom = (en_hsiom_sel_t)i2c_SCB_hsiom;
    Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t *)&GPIO->PRT[Sda_port]), Sda_pin, &stc_port_pin_cfg_i2c);

    stc_port_pin_cfg_i2c.driveMode = CY_GPIO_DM_OD_DRIVESLOW;
    stc_port_pin_cfg_i2c.hsiom = (en_hsiom_sel_t)i2c_SCB_hsiom;
    Cy_GPIO_Pin_Init(((volatile stc_GPIO_PRT_t *)&GPIO->PRT[Scl_port]), Scl_pin, &stc_port_pin_cfg_i2c);

    /* 3  Interrupt Configuration */
    /*中断配置，SCB中断号确定*/
    cy_stc_sysint_irq_t irq_cfg =
        {
            .sysIntSrc = (cy_en_intr_t)SCB_IrQn,
            .intIdx = CPUIntIdx3_IRQn,
            .isEnabled = true,
        };
    Cy_SysInt_InitIRQ(&irq_cfg);
    Cy_SysInt_SetSystemIrqVector(irq_cfg.sysIntSrc, (cy_systemIntr_Handler)(cy_en_intr_t)SCB_IrQn);
    NVIC_SetPriority(irq_cfg.intIdx, 3ul);
    NVIC_EnableIRQ(irq_cfg.intIdx);

    /* 4 Initialize & Enable I2C  */
    /* SCB初始化配置结构体 */
    static const cy_stc_scb_i2c_config_t g_stc_i2c_config =
        {
            .i2cMode = CY_SCB_I2C_MASTER, // I2C模式
             .useRxFifo = true,
             .useTxFifo = true,
             .slaveAddress = Slave_Addr,
             .slaveAddressMask = Slave_Addr,
             .acceptAddrInFifo = false,
             .ackGeneralAddr = false,
             .enableWakeFromSleep = false,
        };
    Cy_SCB_I2C_DeInit(SCB_n[I2c_SCB]);
    Cy_SCB_I2C_Init(SCB_n[I2c_SCB], &g_stc_i2c_config, &Scb_I2C_Context[I2c_SCB]);
    Cy_SCB_I2C_SetDataRate(SCB_n[I2c_SCB], E_I2C_DATARATE, E_I2C_INCLK_TARGET_FREQ);
    Cy_SCB_I2C_RegisterEventCallback(SCB_n[I2c_SCB], NULL, &Scb_I2C_Context[I2c_SCB]);
    Cy_SCB_I2C_Enable(SCB_n[I2c_SCB]);
}

//
//void Scb_SendByte(uint8_t I2C_x,uint8_t Slave_Addr,uint8_t Send_byte)
//{
//  while(Cy_SCB_GetNumInTxFifo(SCB_n[I2C_x]) != 0ul){}
//    
//   CY_ASSERT(Cy_SCB_I2C_MasterSendStart(SCB_n[I2C_x], Slave_Addr, CY_SCB_I2C_WRITE_XFER, 2000ul, &Scb_I2C_Context[I2c_Scb]) == CY_SCB_I2C_SUCCESS);
//   CY_ASSERT(Cy_SCB_I2C_MasterWriteByte(SCB_n[I2C_x], g_send_byte, 2000ul, &Scb_I2C_Context[I2c_Scb]) == CY_SCB_I2C_SUCCESS);
//   CY_ASSERT(Cy_SCB_I2C_MasterSendWriteStop(SCB_n, 2000ul, &Scb_I2C_Context[I2c_Scb]) == CY_SCB_I2C_SUCCESS);
//}
//
//uint8_t Scb_ReadByte(uint8_t Scb,uint8_t Slave_Addr)
//{
//    static uint8_t g_recv_byte = 0x00u;
//    while(Cy_SCB_GetNumInRxFifo(SCB_n) != 0ul);
//    /* Send START and Receive ACK/NACK */
//    CY_ASSERT(Cy_SCB_I2C_MasterSendStart(SCB_n, Slave_Addr, CY_SCB_I2C_READ_XFER, 2000ul, &Scb_I2C_Context[I2c_Scb]) == CY_SCB_I2C_SUCCESS);
//    /* Receive One Byte Data */    
//    CY_ASSERT(Cy_SCB_I2C_MasterReadByte(SCB_n, CY_SCB_I2C_NAK, &g_recv_byte, 2000ul, &Scb_I2C_Context[I2c_Scb]) == CY_SCB_I2C_SUCCESS);
//    /* Send NACK (and stop) */
//    CY_ASSERT(Cy_SCB_I2C_MasterSendReadStop(SCB_n, 2000ul, &Scb_I2C_Context[I2c_Scb]) == CY_SCB_I2C_SUCCESS);
//    return g_recv_byte;
//}
//
//
//
//
//
///*
//@ 使用I2C几 就是SCB几，注意如果其他，模块使用了某个SCB模块，避开重复
//*/
//void Scb_I2C_Master_Test(volatile stc_SCB_t *SCB_n) // use LowLevel API
//{
//  
//    /* I2C Master Byte Write  */
//    /* Make sure TX FIFO empty */
//    while(Cy_SCB_GetNumInTxFifo(SCB_n) != 0ul);
//    /* Send START and Receive ACK/NACK */
//    CY_ASSERT(Cy_SCB_I2C_MasterSendStart(SCB_n, Slave_Addr, CY_SCB_I2C_WRITE_XFER, 2000ul, &g_stc_i2c_context) == CY_SCB_I2C_SUCCESS);
//    /* Transmit One Byte Data */
//    
//    //
//    /*卡死在发送中*/
//    //
//    static uint8_t g_send_byte = 0xF1u;
//    CY_ASSERT(Cy_SCB_I2C_MasterWriteByte(SCB_n, g_send_byte, 2000ul, &g_stc_i2c_context) == CY_SCB_I2C_SUCCESS);
//    /* Send STOP */
//    CY_ASSERT(Cy_SCB_I2C_MasterSendWriteStop(SCB_n, 2000ul, &g_stc_i2c_context) == CY_SCB_I2C_SUCCESS);
//    /*------------------------*/
//    /* I2C Master Byte Read   */
//    /*------------------------*/
//    /* Make sure RX FIFO empty */
//    while(Cy_SCB_GetNumInRxFifo(SCB_n) != 0ul);
//    /* Send START and Receive ACK/NACK */
//    CY_ASSERT(Cy_SCB_I2C_MasterSendStart(SCB_n, Slave_Addr, CY_SCB_I2C_READ_XFER, 2000ul, &g_stc_i2c_context) == CY_SCB_I2C_SUCCESS);
//    /* Receive One Byte Data */
//    static uint8_t g_recv_byte = 0x00u;
//    CY_ASSERT(Cy_SCB_I2C_MasterReadByte(SCB_n, CY_SCB_I2C_NAK, &g_recv_byte, 2000ul, &g_stc_i2c_context) == CY_SCB_I2C_SUCCESS);
//    /* Send NACK (and stop) */
//    CY_ASSERT(Cy_SCB_I2C_MasterSendReadStop(SCB_n, 2000ul, &g_stc_i2c_context) == CY_SCB_I2C_SUCCESS);
//}
//
//void Scb_I2C_Master_HighLevelAPI_Test(volatile stc_SCB_t *SCB_n)
//{
//    /* I2C Master Write */
//    static uint8_t g_i2c_tx_buf[32] = "ABCDEFG012345!";
//    g_stc_i2c_master_config.buffer     = &g_i2c_tx_buf[0];
//    g_stc_i2c_master_config.bufferSize = strlen((char *)g_i2c_tx_buf);
//    CY_ASSERT(Cy_SCB_I2C_MasterWrite(SCB_n, &g_stc_i2c_master_config, &g_stc_i2c_context) == CY_SCB_I2C_SUCCESS);
//    Cy_SysTick_DelayInUs(100000ul);
//    /* I2C Master Read */
//    static uint8_t g_i2c_rx_buf[32];
//    g_stc_i2c_master_config.buffer     = &g_i2c_rx_buf[0];
//    g_stc_i2c_master_config.bufferSize = 32;
//    CY_ASSERT(Cy_SCB_I2C_MasterRead(SCB_n, &g_stc_i2c_master_config, &g_stc_i2c_context) == CY_SCB_I2C_SUCCESS);
//}
