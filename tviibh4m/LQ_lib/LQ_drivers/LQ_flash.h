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
code flash : 4160KB  0x10000000-0x103F0000 32KB*126(LS)+8KB*16(SS) LS:大扇区 SS:小扇区
work flash : 256KB   0x14000000-0x14030000 2KB*96(LS)+128B*512(SS) LS:大扇区 SS:小扇区
      SRAM : 768KB
       ROM : 64KB
P22
*******************************************************************************/
#ifndef _LQ_FLASH_H_
#define _LQ_FLASH_H_

#include "flash/cy_flash.h"
#include "flash/cy_mw_flash.h"


#define TEST_W_LS_ADDR    (CY_WFLASH_LG_SBM_TOP)        //0x14000000UL
#define TEST_W_SS_ADDR    (CY_WFLASH_SM_SBM_TOP)        //0x14030000UL


void Test_EEPROM(void);












#endif



