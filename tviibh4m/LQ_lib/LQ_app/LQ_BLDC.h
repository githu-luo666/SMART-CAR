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
#ifndef __LQ_BLDC_H__
#define __LQ_BLDC_H__

#include "stdint.h"

typedef struct _BLDC_MANAGER
{
  uint8_t step;
  uint16_t counts;
  int16_t duty;
  uint8_t Electricity_flag;//上电检测电流标志
}BLDC_MANAGER, *PBLDC_MANAGER;

extern BLDC_MANAGER g_sBLDCMag;
extern short BLDCduty;

#define P1_PWM_P      P19_2_TCP_27
#define P2_PWM_P      P19_3
#define P3_PWM_P      P20_0_TCP_30
#define P4_PWM_P      P20_1
#define P5_PWM_P      P20_2_TCP_48
#define P6_PWM_P      P20_3

#define HALL_H1_P     P15_0     //蓝色
#define HALL_H2_P     P15_1     //绿色
#define HALL_H3_P     P15_2     //黄色

void BLDC_MotorInit(uint16_t freq);
void BLDC_HallInit(void);
uint8_t Get_Hall_State(void);
void BLDC_Motor_Hall_Run(int16_t motor_duty);
void LQ_BLDCCorotation(void);
void LQ_BLDCReversal(void);
void LQ_BLDCStop(void);
void LQ_BLDCCurrent_detection(void);
void TestMotorBLDC (void);

#endif /* __LQ_BLDC_H__ */
