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
#ifndef _LQ_KEY_H_
#define _LQ_KEY_H_

#include "LQ_GPIO.h"
#include "LQ_led.h"

//定义模块号
typedef enum
{
    KEY0 = 0,  //母板按键0
    KEY1 = 1,
    KEY2 = 2,
    DSW0 = 3,    //母板拨码开关0
    DSW1 = 4,
} KEYn_e;

typedef enum
{
    LOW = 0,  //按下
    HIGH = 1, //松开
    FAIL = 0xff, //错误
} KEYs_e;

typedef enum //
{
    NOKEYDOWN = 0, 
    KEY0DOWN = 0x01,  
    KEY1DOWN = 0x02,
    KEY2DOWN = 0x04,
    KEY01DOWN = 0x03,  
    KEY02DOWN = 0x05,
    KEY12DOWN = 0x06,
    KEY012DOWN = 0x07,
    KEYError = 0xFF,
} KEYdown_e;
//定义的管脚对应实际按键
#define KEY0p      P11_0  //母板上按键0
#define KEY1p      P11_1  //母板上按键1
#define KEY2p      P11_2  //母板上按键2
#define DSW0p      P13_5  //母板上拨码开关0
#define DSW1p      P13_6  //母板上拨码开关1
#define REEDp      P21_5  //干簧管

void GPIO_KEY_Init (void);
unsigned char KEY_Read (KEYn_e KEYno);
unsigned char KEY_Read_All (void);
void Test_GPIO_KEY (void);
void Test_ComKEY_Tft (void);
void Test_reed(void);
void Target(void);


#endif




