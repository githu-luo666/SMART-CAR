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
#ifndef __LQ_VL53_H_
#define __LQ_VL53_H_

#include <stdio.h>
#include <stdint.h>


 #define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
 #define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
 #define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
 #define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
 #define VL53L0X_REG_SYSRANGE_START                  0x00 // 开始测量
 #define VL53L0X_Mode1                               0x01 // 0x01 快速，单次测量模式
 #define VL53L0X_Mode2                               0x02 // 0x02 快速，连续测量模式
 #define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
 #define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
 #define VL53_REG_DIS                                0x1e // 测量数据寄存器
 #define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS        0x8a // VL53设备地址所在寄存器，可修改
 #define VL53ADDR                                    0x52 // 0x52 VL53L0X设备默认I2C地址
 #define VL53NEWADDR                                 0x30



void Test_Vl53(void);

void VL53_Write_Byte(unsigned char dev, unsigned char reg, unsigned char data);

void VL53_Read_nByte(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data);


#endif











