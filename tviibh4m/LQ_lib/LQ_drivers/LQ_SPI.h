/*
@ LQ_SPI.H 文件  date 2023-11-22
*/

#ifndef _LQ_SPI_H
#define _LQ_SPI_H

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


#define SCB_SPI_MODE   CY_SCB_SPI_MASTER
/*
SCB0管交设置说明
 管脚   
 MISO -->  P01_0
 MOSI -->  P01_1
 CLK  -->  P01_2
 SE   -->  P01_3
 CLOCK和中断均在SCB0下
*/

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

void LQ_SPI_InitConfig(void);

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
