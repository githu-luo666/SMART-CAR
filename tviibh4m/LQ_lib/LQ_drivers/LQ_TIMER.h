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
#ifndef _LQ_TIMER_H_
#define _LQ_TIMER_H_
#include "sysclk/cy_sysclk.h"
#include "sysint/cy_sysint.h"
#include "tcpwm/cy_tcpwm_counter.h"
#include "LQ_UART.h"
#include "LQ_GPIO.h"

/*
包含75个16位和8个32位定时器，
其中12个16位计数器用于电机控制，最多63个16位计数器和8个32位计数器用于常规操作
Timer/counter/PWM block (TCPWM)
*/
/*
TCPWM0 Group #0         常规16位 63个               PWM PWMx_y PWMx_y_N
TCPWM0 Group #1         PWM用于电机控制的12个       PWMx_M_y PWMx_M_y_N
TCPWM0 Group #3         32位 8个                    PWM_H_x  PWM_H_x_n

*/

typedef enum
{
    P00_0_TCP_18  = 0x0000120,       P00_0_TCP_22_N  = 0x0000161,
    P00_1_TCP_17  = 0x0010110,       P00_1_TCP_18_N  = 0x0010121,
    P00_2_TCP_14  = 0x00200E0,       P00_2_TCP_17_N  = 0x0020111,
    P00_3_TCP_13  = 0x00300D0,       P00_3_TCP_14_N  = 0x00300E1,
    
    P01_0_TCP_12  = 0x01000C0,       P01_0_TCP_13_N  = 0x01000D1,
    P01_1_TCP_11  = 0x01100B0,       P01_1_TCP_12_N  = 0x01100C1,
    
    P02_0_TCP_7   = 0x0200070,       P02_0_TCP_8_N   = 0x0200081,
    P02_1_TCP_6   = 0x0210060,       P02_1_TCP_7_N   = 0x0210071,
    P02_2_TCP_5   = 0x0220050,       P02_2_TCP_6_N   = 0x0220061,
    P02_3_TCP_4   = 0x0230040,       P02_3_TCP_5_N   = 0x0230051,
    P02_4_TCP_3   = 0x0240030,       P02_4_TCP_4_N   = 0x0240041,
    
    P03_0_TCP_1   = 0x0300010,       P03_0_TCP_2_N   = 0x0300021,
    P03_1_TCP_0   = 0x0310000,       P03_1_TCP_1_N   = 0x0310011,
    P03_2_TCP_M3  = 0x0321030,       P03_2_TCP_0_N   = 0x0320001,
    P03_3_TCP_M2  = 0x0331020,       P03_3_TCP_M3_N  = 0x0331031,
    P03_4_TCP_M1  = 0x0341010,       P03_4_TCP_M3_N  = 0x0341021,
    
    P04_0_TCP_4   = 0x0400040,       P04_0_TCP_M0_N  = 0x0401001,
    P04_1_TCP_5   = 0x0410050,       P04_1_TCP_4_N   = 0x0410041,
    
    P05_0_TCP_9   = 0x0500090,       P05_0_TCP_8_N   = 0x0500081,
    P05_1_TCP_10  = 0x05100A0,       P05_1_TCP_9_N   = 0x0510091,
    P05_2_TCP_11  = 0x05200B0,       P05_2_TCP_10_N  = 0x05200A1,
    P05_3_TCP_12  = 0x05300C0,       P05_3_TCP_11_N  = 0x05300B1,
    P05_4_TCP_13  = 0x05400D0,       P05_4_TCP_12_N  = 0x05400C1,
    
    P06_0_TCP_M0  = 0x0601000,       P06_0_TCP_14_N  = 0x06000E1,
    P06_1_TCP_0   = 0x0610000,       P06_1_TCP_M0_N  = 0x0611001,
    P06_2_TCP_M1  = 0x0621010,       P06_2_TCP_0_N   = 0x0620001,
    P06_3_TCP_1   = 0x0630010,       P06_3_TCP_M1_N  = 0x0631011,
    P06_4_TCP_M2  = 0x0641020,       P06_4_TCP_1_N   = 0x0640011,
    P06_5_TCP_2   = 0x0650020,       P06_5_TCP_M2_N  = 0x0651021,
    P06_6_TCP_M3  = 0x0661030,       P06_6_TCP_2_N   = 0x0660021,
    P06_7_TCP_3   = 0x0670030,       P06_7_TCP_M3_N  = 0x0671031,
    
    P07_0_TCP_M4  = 0x0701040,       P07_0_TCP_M3_N  = 0x0701031,
    P07_1_TCP_15  = 0x07100F0,       P07_1_TCP_M4_N  = 0x0711041,
    P07_2_TCP_M5  = 0x0721050,       P07_2_TCP_15_N  = 0x07200F1,
    P07_3_TCP_16  = 0x0730100,       P07_3_TCP_M5_N  = 0x0731051,
    P07_4_TCP_M6  = 0x0741060,       P07_4_TCP_16_N  = 0x0740101,
    P07_5_TCP_17  = 0x0750110,       P07_5_TCP_M6_N  = 0x0751061,
    P07_6_TCP_M7  = 0x0761070,       P07_6_TCP_17_N  = 0x0760111,
    P07_7_TCP_18  = 0x0770120,       P07_7_TCP_M7_N  = 0x0771071,
    
    P08_0_TCP_19  = 0x0800130,       P08_0_TCP_18_N  = 0x0800121,
    P08_1_TCP_20  = 0x0810140,       P08_1_TCP_19_N  = 0x0810131,
    P08_2_TCP_21  = 0x0820150,       P08_2_TCP_20_N  = 0x0820141,
    P08_3_TCP_22  = 0x0830160,       P08_3_TCP_21_N  = 0x0830151,
    
    P09_0_TCP_24  = 0x0900180,       P09_0_TCP_23_N  = 0x0900171,
    P09_1_TCP_25  = 0x0910190,       P09_1_TCP_24_N  = 0x0910181,
    
    P10_0_TCP_28  = 0x0A001C0,       P10_0_TCP_27_N  = 0x0A001B1,
    P10_1_TCP_29  = 0x0A101D0,       P10_1_TCP_28_N  = 0x0A101C1,
    P10_2_TCP_30  = 0x0A201E0,       P10_2_TCP_29_N  = 0x0A201D1,
    P10_3_TCP_31  = 0x0A301F0,       P10_3_TCP_30_N  = 0x0A301E1,
    P10_4_TCP_32  = 0x0A40200,       P10_4_TCP_31_N  = 0x0A401F1,
    
    P11_0_TCP_61  = 0x0B003D0,       P11_0_TCP_62_N  = 0x0B003E1,
    P11_1_TCP_60  = 0x0B103C0,       P11_1_TCP_61_N  = 0x0B103D1,
    P11_2_TCP_59  = 0x0B203B0,       P11_2_TCP_60_N  = 0x0B203C1,
    
    P12_0_TCP_36  = 0x0C00240,       P12_0_TCP_35_N  = 0x0C00231,
    P12_1_TCP_37  = 0x0C10250,       P12_1_TCP_36_N  = 0x0C10241,
    P12_2_TCP_38  = 0x0C20260,       P12_2_TCP_37_N  = 0x0C20251,
    P12_3_TCP_39  = 0x0C30270,       P12_3_TCP_38_N  = 0x0C30261,
    P12_4_TCP_40  = 0x0C40280,       P12_4_TCP_39_N  = 0x0C40271,
    P12_5_TCP_41  = 0x0C50290,       P12_5_TCP_40_N  = 0x0C50281,
    
    P13_0_TCP_M8  = 0x0D01080,       P13_0_TCP_43_N  = 0x0D002B1,
    P13_1_TCP_44  = 0x0D102C0,       P13_1_TCP_M8_N  = 0x0D11081,
    P13_2_TCP_M9  = 0x0D21090,       P13_2_TCP_44_N  = 0x0D202C1,
    P13_3_TCP_45  = 0x0D302D0,       P13_3_TCP_M9_N  = 0x0D31091,
    P13_4_TCP_M10 = 0x0D410A0,       P13_4_TCP_45_N  = 0x0D402D1,
    P13_5_TCP_46  = 0x0D502E0,       P13_5_TCP_M10_N = 0x0D510A1,
    P13_6_TCP_M11 = 0x0D610B0,       P13_6_TCP_46_N  = 0x0D602E1,
    P13_7_TCP_47  = 0x0D702F0,       P13_7_TCP_M11_N = 0x0D710B1,
    
    P14_0_TCP_48  = 0x0E00300,       P14_0_TCP_47_N  = 0x0E002F1,
    P14_1_TCP_49  = 0x0E10310,       P14_1_TCP_48_N  = 0x0E10301,
    P14_4_TCP_52  = 0x0E40340,       P14_4_TCP_51_N  = 0x0E40331,
    P14_5_TCP_53  = 0x0E50350,       P14_5_TCP_52_N  = 0x0E50341,
    
    P15_0_TCP_56  = 0x0F00380,       P15_0_TCP_55_N  = 0x0F00371,
    P15_1_TCP_57  = 0x0F10390,       P15_1_TCP_56_N  = 0x0F10381,
    P15_2_TCP_58  = 0x0F203A0,       P15_2_TCP_57_N  = 0x0F20391,
    P15_3_TCP_59  = 0x0F303B0,       P15_3_TCP_58_N  = 0x0F303A1,
    
    
    P17_0_TCP_61  = 0x11003D0,       P17_0_TCP_62_N  = 0x11003E1,
    P17_1_TCP_60  = 0x11103C0,       P17_1_TCP_61_N  = 0x11103D1,
    P17_2_TCP_59  = 0x11203B0,       P17_2_TCP_60_N  = 0x11203C1,
    P17_3_TCP_58  = 0x11303A0,       P17_3_TCP_59_N  = 0x11303B1,
    P17_4_TCP_57  = 0x1140390,       P17_4_TCP_58_N  = 0x11403A1,
    
    P18_0_TCP_M6  = 0x1201060,       P18_0_TCP_M5_N  = 0x1201051,
    P18_1_TCP_M7  = 0x1211070,       P18_1_TCP_M6_N  = 0x1211061,
    P18_2_TCP_55  = 0x1220370,       P18_2_TCP_M7_N  = 0x1221071,
    P18_3_TCP_54  = 0x1230360,       P18_3_TCP_55_N  = 0x1230371,
    P18_4_TCP_53  = 0x1240350,       P18_4_TCP_54_N  = 0x1240361,
    P18_5_TCP_52  = 0x1250340,       P18_5_TCP_53_N  = 0x1250351,
    P18_6_TCP_51  = 0x1260330,       P18_6_TCP_52_N  = 0x1260341,
    P18_7_TCP_50  = 0x1270320,       P18_7_TCP_51_N  = 0x1270331,
    
    P19_0_TCP_M3  = 0x1301030,       P19_0_TCP_50_N  = 0x1300321,
    P19_1_TCP_26  = 0x13101A0,       P19_1_TCP_M3_N  = 0x1311031,
    P19_2_TCP_27  = 0x13201B0,       P19_2_TCP_26_N  = 0x13201A1,
    P19_3_TCP_28  = 0x13301C0,       P19_3_TCP_27_N  = 0x13301B1,
    P19_4_TCP_29  = 0x13401D0,       P19_4_TCP_28_N  = 0x13401C1,
    
    P20_0_TCP_30  = 0x14001E0,       P20_0_TCP_29_N  = 0x14001D1,
    P20_1_TCP_49  = 0x1410310,       P20_1_TCP_30_N  = 0x14101E1,
    P20_2_TCP_48  = 0x1420300,       P20_2_TCP_49_N  = 0x1420311,
    P20_3_TCP_47  = 0x14302F0,       P20_3_TCP_48_N  = 0x1430301,
    
    P21_0_TCP_42  = 0x15002A0,       P21_0_TCP_43_N  = 0x15002B1,
    P21_1_TCP_41  = 0x1510290,       P21_1_TCP_42_N  = 0x15102A1,
    P21_2_TCP_40  = 0x1520280,       P21_2_TCP_41_N  = 0x1520291,
    P21_3_TCP_39  = 0x1530270,       P21_3_TCP_40_N  = 0x1530281,
    P21_5_TCP_37  = 0x1550250,       P21_5_TCP_38_N  = 0x1550261,
    P21_6_TCP_36  = 0x1560240,       P21_6_TCP_37_N  = 0x1560251,
    
    P22_1_TCP_33  = 0x1610210,       P22_1_TCP_34_N  = 0x1610221,
    P22_2_TCP_32  = 0x1620200,       P22_2_TCP_33_N  = 0x1620211,
    P22_3_TCP_31  = 0x16301F0,       P22_3_TCP_32_N  = 0x1630201,
    P22_4_TCP_30  = 0x16401E0,       P22_4_TCP_31_N  = 0x16401F1,
    P22_5_TCP_29  = 0x16501D0,       P22_5_TCP_30_N  = 0x16501E1,
    P22_6_TCP_28  = 0x16601C0,       P22_6_TCP_29_N  = 0x16601D1,
    
    P23_0_TCP_M8  = 0x1701080,       P23_0_TCP_27_N  = 0x17001B1,
    P23_1_TCP_M9  = 0x1711090,       P23_1_TCP_M8_N  = 0x1711081,                    
    P23_3_TCP_M11 = 0x17310B0,       P23_3_TCP_M10_N = 0x17310A1,
    
//    P23_4_TCP_25  = 0x1740190,                           //TDO   不可用于配置TCP_
//    P23_5_TCP_24  = 0x1750180,                           //TCK   不可用于配置TCP_
//    P23_6_TCP_23  = 0x1760170,                           //TMS   不可用于配置TCP_
//    P23_7_TCP_22  = 0x1770160,                           //TDI   不可用于配置TCP_
    
}GPIO_TCP_t;    //Timer、Counter、PWM

/* 获取TCP对应GPIO 端口 */
#define TCP_GetModule(GPIO_NAME) ((GPIO_NAME >>20)& 0xFF)

/* 获取TCP对应GPIO 引脚 */
#define TCP_GetIndex(GPIO_NAME)   ((GPIO_NAME>>16) & 0xF)

/* 获取TCP对应 分组 */
#define TCP_GetGroup(GPIO_NAME)   ((GPIO_NAME>>12) & 0xF)

/* 获取TCP对应 通道 */
#define TCP_GetChl(GPIO_NAME)   ((GPIO_NAME>>4) & 0xFF)

/* 获取TCP对应 互补通道 */
#define TCP_GetN(GPIO_NAME)   ((GPIO_NAME) & 0xF)

void LQ_Init_Timer(GPIO_TCP_t pin, uint16_t tim, cy_en_tcpwm_clk_prescalers_t pre);
void Test_timer(void);




#endif


