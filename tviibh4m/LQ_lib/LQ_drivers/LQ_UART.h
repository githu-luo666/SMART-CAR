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
#ifndef LQ_UART_H_
#define LQ_UART_H_

#include "cyt4bb8cee.h"
#include "gpio/cy_gpio.h"
#include "scb/cy_scb_uart.h"
#include "gpio/cy_gpio.h"
#include "cyip_scb_v2.h"
#include "sysclk/cy_sysclk.h"
#include "sysint/cy_sysint.h"
#include <stdio.h>
#include <stdarg.h>
#include "LQ_led.h"

/*printf 用哪个串口输出选择宏定义*/
#define Printf_Uartx   SCB7
#define Printf_Uartx0  SCB1

/*定义接收几个数据后触发接收中断*/
#define size 1
extern uint8_t g_uart_interrupt_buf[size];

extern unsigned char ReadBuff8[512];
extern char Usart8_Rec_Fini_Flag;

extern uint8_t g_uart_interrupt_buf0[1];
extern  char ReadBuff0[128];

extern uint8_t Usart7_Rec_Fini_Flag;
extern uint8_t ReadBuff7[512];

//----------------------------------------------------------------------
typedef enum
{
    /*
      SCB hsiom port pin 
      0   00    00   0   
    */
    UART0_RX_P00_0  = 0x011000,       
    
    UART1_RX_P18_0  = 0x111120,       
    UART1_RX_P20_3  = 0x111143,       

    UART2_RX_P14_0  = 0x2130E0,       
    UART2_RX_P19_0  = 0x213130,       
    
    UART3_RX_P13_0  = 0x3110D0,       
    UART3_RX_P17_1  = 0x311111,       
    
    UART4_RX_P06_0  = 0x411060,       
    UART4_RX_P10_0  = 0x4110A0,       
    
    UART5_RX_P04_0  = 0x511040,       
    UART5_RX_P07_0  = 0x511070,       
    
    UART6_RX_P03_0  = 0x611030,       
    UART6_RX_P21_7  = 0x611157,       
    
    UART7_RX_P02_0  = 0x711020,       
    UART7_RX_P23_0  = 0x711170,       
    
    UART8_RX_P12_0  = 0x8110C0,       
    
    UART9_RX_P15_0  = 0x9110F0,       
    
}GPIO_UART_RX_t;

typedef enum
{
  UART0_TX_P00_1  = 0x011001,
  
  UART1_TX_P18_1  = 0x111121,
  UART1_TX_P20_4  = 0x111144,
  
  UART2_TX_P14_1  = 0x2130E1,
  UART2_TX_P19_1  = 0x213131,
  
  UART3_TX_P13_1  = 0x3110D1,
  UART3_TX_P17_2  = 0x311112,
  
  UART4_TX_P06_1  = 0x411061,
  UART4_TX_P10_1  = 0x4110A1,
  
  UART5_TX_P04_1  = 0x511041,
  UART5_TX_P07_1  = 0x511071,
  
  UART6_TX_P03_1  = 0x611031,
  UART6_TX_P22_1  = 0x611161,
  
  UART7_TX_P02_1  = 0x711021,
  UART7_TX_P23_1  = 0x711171,
  
  UART8_TX_P12_1  = 0x8110C1,
  
  UART9_TX_P15_1  = 0x9110F1,
  
}GPIO_UART_TX_t;

/* 获取UART对应GPIO 端口 */
#define UART_GetModule(GPIO_NAME) ((GPIO_NAME >>4)& 0xFF)

/* 获取UART对应GPIO 引脚 */
#define UART_GetIndex(GPIO_NAME)   ((GPIO_NAME>>0) & 0xF)

/* 获取UART对应 复用 */
#define UART_GetHsiom(GPIO_NAME)   ((GPIO_NAME>>12) & 0xFF)

/* 获取UART对应 通道 */
#define UART_GetChl(GPIO_NAME)   ((GPIO_NAME>>20) & 0xF)
/*过采样频率*/
#define UART_OVERSAMPLING (8ul)

extern cy_stc_scb_uart_context_t uartx_context[10];
extern const uint8_t SCBx_Clk[10];
extern volatile stc_SCB_t* SCB_x[10];
extern uint8_t g_uart_in_data[128];

void LQ_UART_Init_Config(GPIO_UART_RX_t RX_Pin, GPIO_UART_TX_t TX_Pin, uint32_t boadrate);

void Scb_UART_Event(uint32_t locEvents);
void Test_Bluetooth(void);
void LQ_Printf(void *fmt, ...);
void LQ_Printf0(void *fmt, ...);
void ANO_DT_send_int16byte16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 );

#endif





