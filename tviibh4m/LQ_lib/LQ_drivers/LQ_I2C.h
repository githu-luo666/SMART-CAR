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
#ifndef _LQ_I2C_H
#define _LQ_I2C_H

#include "LQ_GPIO.h"
#include "cy_device_headers.h"
#include "cy_project.h"
#include "scb/cy_scb_i2c.h"
#include "syslib/cy_syslib.h"
#include "cyip_coredefines.h"
#include "cyip_scb_v2.h"
#include "cyt4bb8cee.h"
#include "gpio/cy_gpio.h"
#include "scb/cy_scb_uart.h"
#include "sysclk/cy_sysclk.h"
#include "sysint/cy_sysint.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

/* Select Frequency */
#if (CY_USE_PSVP == 1)
#define E_SOURCE_CLK_FREQ (24000000ul) // 固定的
#else
#define E_SOURCE_CLK_FREQ (80000000u)   // 固定的  默认的
#endif

#define E_I2C_INCLK_TARGET_FREQ (2000000ul) // 连接到SCB的时钟的频率，单位为Hz。
#define E_I2C_DATARATE (100000ul)           // I2C数据速率设置，单位为Hz。

//I2C从机地址设置

#define Slave_Addr 0x00   //在这修改要初始化的I2C地址



typedef enum
{
    /*
    自编码：  根据SDA引脚编号确定 SCB hsiom port pin，
    SCB hsiom port pin
    0   00    00   0
    */
    I2C0_SDA_P00_3 = 0x00E003, //  引脚SCB0   hsiom 14   P00_3
    I2C0_SDA_P01_1 = 0x00E011, //  引脚SCB0   hsiom 14   P01_1

    I2C1_SDA_P18_1 = 0x112121, //  引脚SCB1   hsiom 18   P18_1
    I2C1_SDA_P20_4 = 0x112144,
  
    I2C2_SDA_P14_1 = 0x2120E1,
    I2C2_SDA_P19_1 = 0x212131,
  
    I2C3_SDA_P13_1 = 0x3120D1,
    I2C3_SDA_P17_2 = 0x312112,
  
    I2C4_SDA_P06_1 = 0x412061,
    I2C4_SDA_P10_1 = 0x4120A1,
  
    I2C5_SDA_P04_1 = 0x512041,
    I2C5_SDA_P07_1 = 0x512071,
  
    I2C6_SDA_P03_1 = 0x612031,
    I2C6_SDA_P22_1 = 0x612161,
  
    I2C7_SDA_P00_0 = 0x712000,
    I2C7_SDA_P02_1 = 0x712021,
    I2C7_SDA_P23_1 = 0x712171,
  
    I2C8_SDA_P12_1 = 0x8120C1,
  
    I2C9_SDA_P15_1 = 0x9120F1,

} I2C_SDA_Pin;

typedef enum
{
    I2C0_SCL_P00_2 = 0x00E002,
    I2C0_SCL_P01_0 = 0x00E010,

    I2C1_SCL_P18_2 = 0x112122,
    I2C1_SCL_P20_5 = 0x112145,
  
    I2C2_SCL_P14_2 = 0x2120E2,
    I2C2_SCL_P19_2 = 0x212132,
  
    I2C3_SCL_P13_2 = 0x3120D2,
    I2C3_SCL_P17_3 = 0x312113,
  
    I2C4_SCL_P06_2 = 0x412062,
    I2C4_SCL_P10_2 = 0x4120A2,
  
    I2C5_SCL_P04_2 = 0x512042,
    I2C5_SCL_P07_2 = 0x512072,
  
    I2C6_SCL_P03_2 = 0x612032,
    I2C6_SCL_P22_2 = 0x612162,
  
    I2C7_SCL_P00_1 = 0x712001,
    I2C7_SCL_P02_2 = 0x712022,
    I2C7_SCL_P23_2 = 0x712172,
  
    I2C8_SCL_P12_2 = 0x8120C2,
  
    I2C9_SCL_P15_2 = 0x9120F2,

} I2C_SCL_Pin;



//typedef enum
//{
//    /*
//    自编码：  根据SDA引脚编号确定 SCB hsiom port pin，SDA可用端口少
//    SCB hsiom port pin
//    0   00    00   0
//    */
//    I2C0_SDA_P00_3 = 0x00E003, //  引脚SCB0   hsiom 14   P00_3
//    I2C0_SDA_P01_1 = 0x00E011, //  引脚SCB0   hsiom 14   P01_1
//
//    I2C1_SDA_P18_1 = 0x112121, //  引脚SCB1   hsiom 18   P18_1
//
//    I2C2_SDA_P14_1 = 0x2120E1,
//    I2C2_SDA_P19_1 = 0x212131,
//
//    I2C3_SDA_P13_1 = 0x3120D1,
//    I2C3_SDA_P17_2 = 0x312112,
//
//    I2C4_SDA_P06_1 = 0x412061,
//    I2C4_SDA_P10_1 = 0x4120A1,
//
//    I2C5_SDA_P04_1 = 0x512041,
//    I2C5_SDA_P07_1 = 0x512071,
//
//    I2C6_SDA_P03_1 = 0x612031,
//    I2C6_SDA_P22_1 = 0x612161,
//
//    I2C7_SDA_P00_0 = 0x712000,
//    I2C7_SDA_P02_1 = 0x712021,
//    I2C7_SDA_P23_1 = 0x712171,
//
//    I2C8_SDA_P12_1 = 0x8120C1,
//
//    I2C9_SDA_P15_1 = 0x9120F1,
//
//} I2C_SDA_Pin;
//
//typedef enum
//{
//    I2C0_SCL_P00_2 = 0x00E002,
//    I2C0_SCL_P01_0 = 0x00E010,
//
//    I2C1_SCL_P18_2 = 0x112122,
//
//    I2C2_SCL_P19_2 = 0x212132,
//
//    I2C3_SCL_P13_2 = 0x3120D2,
//
//    I2C4_SCL_P06_2 = 0x412062,
//
//    I2C5_SCL_P07_2 = 0x512072,
//
//    I2C6_SCL_P22_2 = 0x612162,
//
//    I2C7_SCL_P00_1 = 0x712001,
//    I2C7_SCL_P02_2 = 0x712022,
//
//    I2C8_SCL_P12_2 = 0x8120C2,
//
//} I2C_SCL_Pin;



/* 获取I2C对应GPIO 端口 */
#define I2C_GetPort(GPIO_NAME) ((GPIO_NAME >> 4) & 0xFF)
/* 获取IC2C对应GPIO 引脚 */
#define I2C_GetIndex(GPIO_NAME) ((GPIO_NAME >> 0) & 0xF)
/* 获取C2C对应 复用 */
#define I2C_GetHsiom(GPIO_NAME) ((GPIO_NAME >> 12) & 0xFF)
/* 获取I2C对应SCB号 */
#define I2C_GetSCB(GPIO_NAME) ((GPIO_NAME >> 20) & 0xF)

extern cy_stc_scb_i2c_context_t g_stc_i2c_context;

void LQ_I2C_Init_Config(I2C_SDA_Pin SDA_Pin, I2C_SCL_Pin SCL_Pin);
void Scb_I2C_Master_Test(volatile stc_SCB_t *SCB_n);

#endif


// *\ section group_scb_i2c_configuration配置注意事项
// *\ref cy_stc_scb_i2c_config_t结构。对于主配置，提供i2cMode、useRxFifo和useTxFifo。对于从属模式，
// 还提供*slaveAddress和slaveAddressMask。其他参数是可选的*操作。
// *要初始化驱动程序，请调用\ref Cy_SCB_I2C_Iit函数，
// 该函数提供填充\ref cy_stc_scb_i2c_config_t结构并分配\ref cy_stc_scb_i2c_context_t。
// 调用\ref Cy_SCB_I2C_启用启动之前

// *I2C操作，必须调用\ref Cy_SCB_I2C_SetDataRate来设置*数据速率。
// **调用\ref Cy_SCB_I2C_MasterRead或\ref Cy_StCB_I2C_MasterWrite
// *与从属设备通信。这些函数不阻塞并且仅启动事务。传输开始后
// *\ref Cy_SCB_I2C_Interrupt处理进一步的数据传输，直到完成。
//   因此，\ref Cy_SCB_I2C_Interrupt必须在用户中断处理程序，使API以上工作。为了监视转移，
// *使用\ref Cy_SCB_I2C_MasterGetStatus或\ref Cy_StCB_I2C_RegisterEventCallback
// *注册要通知的回调函数
// *\ref group_scb_i2c_macro_callback_events。

// *调用\ref Cy_SCB_I2C_MasterSendStart生成开始，发送地址并接收确认。
// 在地址被从设备确认，事务可以通过调用*\ref Cy_SCB_I2C_MasterReadByte或\ref Cy_StCB_I2C-MasterWriteByte取决于在其方向上。这些函数每次调用处理一个字节，
// 因此应该为事务中的每个字节调用它们。请注意，对于读取事务—最后一个字节必须是NAKed。完成当前
// 事务，调用\ref Cy_SCB_I2C_MasterSendStop或调用*\ref Cy_SCB_I2C_MasterSendReStart以完成当前事务并
// 开始一个新的。通常，重新启动以更改事务方向，而不从主控制器释放总线。
// *请注意，这些函数是阻塞的，不需要调用*\ref Cy_SCB_I2C_Interrupt在用户中断处理程序中。使用这些
// 函数需要广泛的知识。

// 从模式操作：\n
// *从属操作基于\ref Cy_SCB_I2C_Interrupt，该中断必须在用户中断处理程序内部调用。读取和写入缓冲区必须
// 以便能够与主设备进行通信。使用*\ref Cy_SCB_I2C_SlaveConfigReadBuf和\ref Cy_StCB_I2C_StlaveConfigWriteBuf为此目的。请注意，事务完成后，缓冲区必须*再次配置。否则，从该点开始使用相同的缓冲区，其中主机停止了先前的事务。
// *例如：读取缓冲区配置为10字节，主读取是8字节。如果未再次配置读取缓冲区，则下一个主读取将从第9个字节开始。
// *要监视传输状态，请使用\ref Cy_SCB_I2C_SlaveGetStatus或使用\ref Cy_SCB_I2C_RegisterEventCallback注册回调函数将收到关于\ref group_scb_i2c_macro_callback_events的通知。
