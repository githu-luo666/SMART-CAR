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
#ifndef _LQ_ADC_H_
#define _LQ_ADC_H_
#include "LQ_GPIO.h"
#include "sysclk/cy_sysclk.h"
#include "adc/cy_adc.h"
#include "LQ_led.h"

typedef enum
{
  /*
    ADCnum chl port pin
    0      00  00   0
  */
 
  ADC0_CH0_P06_0   = 0x000060,      ADC1_CH0_P10_4   = 0x1000A4,        ADC2_CH0_P18_0   = 0x200120,
  ADC0_CH1_P06_1   = 0x001061,                                          ADC2_CH1_P18_1   = 0x201121,
  ADC0_CH2_P06_2   = 0x002062,                                          ADC2_CH2_P18_2   = 0x202122,
  ADC0_CH3_P06_3   = 0x003063,                                          ADC2_CH3_P18_3   = 0x203123,
  ADC0_CH4_P06_4   = 0x004064,      ADC1_CH4_P12_0   = 0x1040C0,        ADC2_CH4_P18_4   = 0x204124,
  ADC0_CH5_P06_5   = 0x005065,      ADC1_CH5_P12_1   = 0x1050C1,        ADC2_CH5_P18_5   = 0x205125,
  ADC0_CH6_P06_6   = 0x006066,      ADC1_CH6_P12_2   = 0x1060C2,        ADC2_CH6_P18_6   = 0x206126,
  ADC0_CH7_P06_7   = 0x007067,      ADC1_CH7_P12_3   = 0x1070C3,        ADC2_CH7_P18_7   = 0x207127,
                                    ADC1_CH8_P12_4   = 0x1080C4,
                                    ADC1_CH9_P12_5   = 0x1090C5,
                                    
                                    
                                    ADC1_CH12_P13_0  = 0x10C0D0,
                                    ADC1_CH13_P13_1  = 0x10D0D1,
                                    ADC1_CH14_P13_2  = 0x10E0D2,
                                    ADC1_CH15_P13_3  = 0x10F0D3,
  ADC0_CH16_P07_0  = 0x010070,      ADC1_CH16_P13_4  = 0x1100D4,
  ADC0_CH17_P07_1  = 0x011071,      ADC1_CH17_P13_5  = 0x1110D5,
  ADC0_CH18_P07_2  = 0x012072,      ADC1_CH18_P13_6  = 0x1120D6,
  ADC0_CH19_P07_3  = 0x013073,      ADC1_CH19_P13_7  = 0x1130D7,
  ADC0_CH20_P07_4  = 0x014074,      ADC1_CH20_P14_0  = 0x1140E0,
  ADC0_CH21_P07_5  = 0x015075,      ADC1_CH21_P14_1  = 0x1150E1,
  ADC0_CH22_P07_6  = 0x016076,      
  ADC0_CH23_P07_7  = 0x017077,      
  ADC0_CH24_P08_1  = 0x018081,      ADC1_CH24_P14_4  = 0x1180E4,
  ADC0_CH25_P08_2  = 0x019082,      ADC1_CH25_P14_5  = 0x1190E5,
  ADC0_CH26_P08_3  = 0x01A083,      

  ADC0_CH28_P09_0  = 0x01C090,      ADC1_CH28_P15_0  = 0x11C0F0,
  ADC0_CH29_P09_1  = 0x01D091,      ADC1_CH29_P15_1  = 0x11D0F1,
                                    ADC1_CH30_P15_2  = 0x11E0F2,
                                    ADC1_CH31_P15_3  = 0x11F0F3,
                                    
  ADC0_CH_M_P11_0  = 0x0FF0B0,      ADC1_CH_M_P11_1  = 0x1FF0B1,       ADC2_CH_M_P11_2  = 0x2FF0B2,                           
                                
}GPIO_ADC_t;

/* 获取ADC对应GPIO 端口 */
#define ADC_GetModule(GPIO_NAME) ((GPIO_NAME >>4)& 0xFF)

/* 获取ADC对应GPIO 引脚 */
#define ADC_GetIndex(GPIO_NAME)   ((GPIO_NAME>>0) & 0xF)

/* 获取ADC对应 分组 */
#define ADC_GetGroup(GPIO_NAME)   ((GPIO_NAME>>20) & 0xF)

/* 获取ADC对应 通道 */
#define ADC_GetChl(GPIO_NAME)   ((GPIO_NAME>>12) & 0xFF)


// VDDA Voltage
#define VDDA_VOLTAGE_4_5V_TO_5_5V   0
#define VDDA_VOLTAGE_2_7V_TO_4_5V   1
#define VDDA_VOLTAGE                VDDA_VOLTAGE_2_7V_TO_4_5V

// Definition of SAR ADC AC Specified Parameters
#if VDDA_VOLTAGE == VDDA_VOLTAGE_4_5V_TO_5_5V
  #define ADC_OPERATION_FREQUENCY_MAX_IN_HZ (26670000ul)
  #define ANALOG_IN_SAMPLING_TIME_MIN_IN_NS (412ull)
#else // VDDA_VOLTAGE == VDDA_VOLTAGE_2_7V_TO_4_5V
  #define ADC_OPERATION_FREQUENCY_MAX_IN_HZ (13340000ul)
  #define ANALOG_IN_SAMPLING_TIME_MIN_IN_NS (600ull)
#endif


#define DIV_ROUND_UP(a,b) (((a) + (b)/2) / (b))

void LQ_Init_ADC(GPIO_ADC_t pinADC);
uint16_t Read_ADC(GPIO_ADC_t pinADC);
void Test_ADC(void);

#endif



