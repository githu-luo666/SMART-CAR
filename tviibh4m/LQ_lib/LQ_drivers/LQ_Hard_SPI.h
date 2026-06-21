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

#ifndef _LQ_HARD_SPI_H_
#define _LQ_HARD_SPI_H_

#include "gpio/cy_gpio.h"
#include "prot/cy_prot.h"
#include "scb/cy_scb_common.h"
#include "scb/cy_scb_ezi2c.h"
#include "scb/cy_scb_i2c.h"
#include "scb/cy_scb_spi.h"
#include "scb/cy_scb_uart.h"
#include "sysclk/cy_sysclk.h"
#include "sysint/cy_sysint.h"
#include "syslib/cy_syslib.h"
#include "bb_bsp_tviibh4m_revb.h"
#include "LQ_UART.h"

/*
SCB0管交设置说明
 管脚   
 MISO -->  P01_0
 MOSI -->  P01_1
 CLK  -->  P01_2
 SE   -->  P01_3
 CLOCK和中断均在SCB0下
*/

typedef enum
{
  SCB0_MISO_P00_0 = 0x01E000,

  SCB1_MISO_P18_0 = 0x113120,

  SCB2_MISO_P14_0 = 0x2110E0,
  SCB2_MISO_P19_0 = 0x211130,

  SCB3_MISO_P13_0 = 0x3150D0,
  SCB3_MISO_P18_1 = 0x315121,

  SCB4_MISO_P00_2 = 0x413002,
  SCB4_MISO_P06_0 = 0x413060,
  SCB4_MISO_P10_0 = 0x4130A0,

  SCB5_MISO_P04_0 = 0x513040,
  SCB5_MISO_P07_0 = 0x513070,

  SCB6_MISO_P03_0 = 0x613030,

  SCB7_MISO_P02_0 = 0x713020,

  SCB8_MISO_P12_0 = 0x8130C0,

  SCB9_MISO_P15_0 = 0x9130F0,
  
  
}GPIO_SPI_MISO_t;

typedef enum
{
  SCB0_MOSI_P00_1 = 0x01E001,

  SCB1_MOSI_P18_1 = 0x113121,

  SCB2_MOSI_P14_1 = 0x2110E1,
  SCB2_MOSI_P19_1 = 0x211131,

  SCB3_MOSI_P13_1 = 0x3150D1,
  SCB3_MOSI_P18_2 = 0x315122,

  SCB4_MOSI_P00_3 = 0x413003,
  SCB4_MOSI_P06_1 = 0x413061,
  SCB4_MOSI_P10_1 = 0x4130A1,

  SCB5_MOSI_P04_1 = 0x513041,
  SCB5_MOSI_P07_1 = 0x513071,

  SCB6_MOSI_P03_1 = 0x613031,

  SCB7_MOSI_P02_1 = 0x713021,

  SCB8_MOSI_P12_1 = 0x8130C1,

  SCB9_MOSI_P15_1 = 0x9130F1,
  
}GPIO_SPI_MOSI_t;

typedef enum
{
  SCB0_CLK_P00_2 = 0x01E002,

  SCB1_CLK_P18_2 = 0x113122,

  SCB2_CLK_P19_2 = 0x211132,

  SCB3_CLK_P13_2 = 0x3150D2,
  SCB3_CLK_P17_3 = 0x315113,
  SCB3_CLK_P18_3 = 0x315123,

  SCB4_CLK_P01_0 = 0x413010,
  SCB4_CLK_P06_2 = 0x413062,
  SCB4_CLK_P10_2 = 0x4130A2,

  SCB5_CLK_P07_2 = 0x513072,

  SCB6_CLK_P03_2 = 0x613032,

  SCB7_CLK_P02_2 = 0x713022,

  SCB8_CLK_P12_2 = 0x8130C2,

  SCB9_CLK_P15_2 = 0x9130F2,
  
}GPIO_SPI_CLK_t;

typedef enum
{
  SCB0_SEL0_P00_3 = 0x001E003,
  SCB0_SEL1_P02_0 = 0x101E020,
  SCB0_SEL2_P02_1 = 0x201E021,
  SCB0_SEL3_P02_2 = 0x301E022,

  SCB1_SEL0_P18_3 = 0x0113123,
  SCB1_SEL1_P18_4 = 0x1113124,
  SCB1_SEL2_P18_5 = 0x2113125,
  SCB1_SEL3_P18_6 = 0x3113126,
  SCB1_SEL2_P21_5 = 0x2113155,

  SCB2_SEL0_P18_4 = 0x0211124,
  SCB2_SEL1_P14_4 = 0x12110E4,
  SCB2_SEL2_P14_5 = 0x22110E5,
  SCB2_SEL0_P19_3 = 0x0211133,
  SCB2_SEL1_P19_4 = 0x1211134,
  SCB2_SEL2_P20_0 = 0x2211140,
                      
  SCB3_SEL0_P13_3 = 0x03150D3,
  SCB3_SEL0_P17_4 = 0x0315114,
  SCB3_SEL1_P13_4 = 0x13150D4,
  SCB3_SEL2_P13_5 = 0x23150D5,
  SCB3_SEL3_P13_6 = 0x33150D6,
                      
  SCB4_SEL0_P01_1 = 0x0413011,
  SCB4_SEL0_P06_3 = 0x0413063,
  SCB4_SEL1_P06_4 = 0x1413064,
  SCB4_SEL0_P10_3 = 0x04130A3,
  SCB4_SEL1_P10_4 = 0x14130A4,
  SCB4_SEL2_P06_5 = 0x2413065,
  SCB4_SEL3_P06_6 = 0x3413066,
                      
  SCB5_SEL0_P07_3 = 0x0513073,
  SCB5_SEL1_P07_4 = 0x1513074,
  SCB5_SEL2_P07_5 = 0x2513075,
  SCB5_SEL2_P05_0 = 0x2513050,
  SCB5_SEL3_P05_1 = 0x3513051,
                      
  SCB6_SEL0_P03_3 = 0x0613033,
  SCB6_SEL1_P03_4 = 0x1613034,
                      
  SCB7_SEL0_P02_3 = 0x0713023,
  SCB7_SEL1_P02_4 = 0x1713024,
                      
  SCB8_SEL0_P12_3 = 0x08130C3,
  SCB8_SEL1_P12_4 = 0x18130C4,
                      
  SCB9_SEL0_P15_3 = 0x09130F3,
  
}GPIO_SPI_SEL_t;

/* 获取SPI对应GPIO 端口 */
#define SPI_GetModule(GPIO_NAME) ((GPIO_NAME >>4)& 0xFF)

/* 获取SPI对应GPIO 引脚 */
#define SPI_GetIndex(GPIO_NAME)   ((GPIO_NAME>>0) & 0xF)

/* 获取SPI对应 复用 */
#define SPI_GetHsiom(GPIO_NAME)   ((GPIO_NAME>>12) & 0xFF)

/* 获取SPI对应 通道 */
#define SPI_GetChl(GPIO_NAME)   ((GPIO_NAME>>20) & 0xF)

/* 获取SPI使能脚 */
#define SPI_GetSEL(GPIO_NAME)   ((GPIO_NAME>>24) & 0xF)

#define SCB_SPI_MODE   CY_SCB_SPI_MASTER

#if SCB_SPI_MODE == CY_SCB_SPI_SLAVE
  /* Slave Settings */
  #define SCB_MISO_DRIVE_MODE CY_GPIO_DM_STRONG_IN_OFF
  #define SCB_MOSI_DRIVE_MODE CY_GPIO_DM_HIGHZ
  #define SCB_CLK_DRIVE_MODE  CY_GPIO_DM_HIGHZ
  #define SCB_SEL0_DRIVE_MODE CY_GPIO_DM_HIGHZ
#else // SCB_SPI_MASTER
  /* Master Settings */
  #define SCB_MISO_DRIVE_MODE CY_GPIO_DM_HIGHZ
  #define SCB_MOSI_DRIVE_MODE CY_GPIO_DM_STRONG_IN_OFF
  #define SCB_CLK_DRIVE_MODE  CY_GPIO_DM_STRONG_IN_OFF
  #define SCB_SEL0_DRIVE_MODE CY_GPIO_DM_STRONG_IN_OFF
#endif

#define SCB_SPI_BAUDRATE     125000ul /* Please set baudrate value of SPI you want */
#define SCB_SPI_OVERSAMPLING 16ul     /* Please set oversampling of SPI you want */
#define SCB_SPI_CLOCK_FREQ   (SCB_SPI_BAUDRATE * SCB_SPI_OVERSAMPLING)

extern cy_stc_scb_spi_context_t spix_context[10];


void LQ_SPI_InitConfig(GPIO_SPI_CLK_t CLK, GPIO_SPI_MISO_t MISO, GPIO_SPI_MOSI_t MOSI, GPIO_SPI_SEL_t SEL, uint32_t BAUDRATE);
uint32_t LQ_SPI_WriteArray(volatile stc_SCB_t *base, void *txBuf, uint32_t len);
uint32_t LQ_SPI_ReadArray(volatile stc_SCB_t const *base, void *rxBuf, uint32_t len);


/**********************************************************************
函数名称：Cy_SCB_SPI_WriteArray
extern __STATIC_INLINE uint32_t Cy_SCB_SPI_WriteArray(volatile stc_SCB_t *base, void *txBuf, uint32_t size);
*将数据数组放入SPI TX FIFO。此函数返回在TX FIFO中放置了多少数据元素。
* base  ：  指向SPI SCB实例的指针。
* txBuf ：  指向要放入TX FIFO的数据的指针。大小由数据类型定义，数据类型取决于配置的TX数据宽度。
* size  ：  要传输的数据元素的数量。
*return：放置在TX FIFO中的数据元素的数量。
*注意：在主模式下，将数据写入TX FIFO会启动SPI转让。当处于从属模式时，不会开始将数据写入TX FIFO SPI传输。
*     数据加载在TX FIFO中，并将发送至主机应其要求。
**SPI接口是全双工的，因此会发生读写操作同时。对于每个数据元素，对于从 TX FIFO，一个传输到RX FIFO。
 *eg:  Cy_SCB_SPI_WriteArray(CY_SPI_SCB_TYPE,(void*)readData, 2);
*******************************************************************/

void Test_SPI(void);


#endif
