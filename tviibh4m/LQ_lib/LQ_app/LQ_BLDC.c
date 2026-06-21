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
#include "LQ_BLDC.h"
#include "include.h"

//#define Use_IR2304        //用2304无刷驱动板 请取消此注释  用2136无刷驱动板 需注释此代码
#define Use_IR2136        //用2136无刷驱动板 请取消此注释  用2304无刷驱动板 需注释此代码

#define BLDC_PWM_FREQUENCY    200 //频率为10KHz, 满占空比为200

BLDC_MANAGER g_sBLDCMag;//无刷结构管理定义
#define MOTOR_FREQUENCY  10000  //电机频率
short BLDCduty=10;//无刷电机pwm 

float y_vel_prev=0;//低通滤波
float IA = 0,IB = 0,IC = 0;//三相电流定义
/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void MotorInit(uint16 freq)
@功能说明：无刷电机引脚，pwm初始化
@参数说明：pwm 频率
@函数返回：无
@修改时间：2023/12/5
@调用方法：BLDC_MotorInit(1000-1);
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void BLDC_MotorInit(uint16_t freq)
{


    LQ_GPIO_Init_Config(P2_PWM_P, GPIO_OUT_PP_DOWN);
    LQ_GPIO_Init_Config(P4_PWM_P, GPIO_OUT_PP_DOWN);
    LQ_GPIO_Init_Config(P6_PWM_P, GPIO_OUT_PP_DOWN);
    
    LQ_PWM_Init(P1_PWM_P,freq);
    //LQ_Pin_Write(P2_PWM_P,freq);
    LQ_PWM_Init(P3_PWM_P,freq);
    //LQ_Pin_Write(P4_PWM_P,freq);
    LQ_PWM_Init(P5_PWM_P,freq);
    //LQ_Pin_Write(P6_PWM_P,freq);
}

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void BLDC_HallInit(void);
@功能说明：初始化霍尔引脚及编码器接口
@参数说明：无
@函数返回：无
@修改时间：2023/12/5
@备    注：编码器接口同时为霍尔引脚状态读取接口
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void BLDC_HallInit(void)
{
   LQ_GPIO_Init_Config(HALL_H1_P, GPIO_IN_HIGHZ);       //蓝色
   LQ_GPIO_Init_Config(HALL_H2_P, GPIO_IN_HIGHZ);       //绿色
   LQ_GPIO_Init_Config(HALL_H3_P, GPIO_IN_HIGHZ);       //黄色
}

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：uint8_t Get_Hall_State(void);
@功能说明：读取霍尔状态
@参数说明：无
@函数返回：hall_state（霍尔状态）
@修改时间：2023/12/5
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
uint8_t Get_Hall_State(void)
{
  uint8_t hall_state = 0;

  if(LQ_Pin_Read_In(HALL_H3_P)) /* 读取霍尔传感器 U 的状态 */
  {
    hall_state |= 0x01U << 0;
  }
  if(LQ_Pin_Read_In(HALL_H2_P)) /* 读取霍尔传感器 V 的状态 */
  {
    hall_state |= 0x01U << 1;
  }
  if(LQ_Pin_Read_In(HALL_H1_P)) /* 读取霍尔传感器 W 的状态 */
  {
    hall_state |= 0x01U << 2;
  }
  return hall_state;
}

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void BLDC_Motor_Hall_Run(int16_t motor_duty);
@功能说明：无刷电机驱动函数，有霍尔
@参数说明：motor_duty（电机占空比）-200 - 200
@函数返回：无
@修改时间：2023/12/5
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void BLDC_Motor_Hall_Run(int16_t motor_duty)
{
  if(motor_duty > 200)  motor_duty = 200;
  else if(motor_duty < -200) motor_duty = -200;
  /* 获取霍尔传感器引脚状态,作为换相的依据 */
  g_sBLDCMag.step = Get_Hall_State();
  g_sBLDCMag.duty = motor_duty;
  if(motor_duty>0)LQ_BLDCCorotation();
  else if(motor_duty==0)LQ_BLDCStop();
  else if(motor_duty<0)LQ_BLDCReversal();
}

#ifdef Use_IR2304
/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCCorotation(void);
@功能说明：控制无刷电机正转
@参数说明：无
@函数返回：无
@修改时间：2023/12/5
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void LQ_BLDCCorotation(void)
{
    switch(g_sBLDCMag.step) //U+ U-  V+ V-  W+ W-
    {
    case 1:
        //A→B=0   B→C=-  C→A=+
        LQ_Set_Pwm(P1_PWM_P, 0);              // P1_PWM_P输出0
        LQ_Set_Pwm(P3_PWM_P, 0);              // P3_PWM_P输出0
        LQ_Set_Pwm(P5_PWM_P,g_sBLDCMag.duty); // P5_PWM_P输出PWM
      break;

    case 2:
        //A→B=-   B→C=+  C→A=0
        LQ_Set_Pwm(P1_PWM_P,0);               // P231输出0
        LQ_Set_Pwm(P3_PWM_P,g_sBLDCMag.duty); // P3_PWM_P输出PWM
        LQ_Set_Pwm(P5_PWM_P,0);               // P5_PWM_P输出0
      break;

    case 3:
        //A→B=-   B→C=+  C→A=0
        LQ_Set_Pwm(P1_PWM_P,0);               // P231输出0
        LQ_Set_Pwm(P3_PWM_P, g_sBLDCMag.duty); // P3_PWM_P输出PWM
        LQ_Set_Pwm(P5_PWM_P,0);                // P5_PWM_P输出0
      break;

    case 4:
        //A→B=+   B→C=0  C→A=-
        LQ_Set_Pwm(P1_PWM_P, g_sBLDCMag.duty); // P1_PWM_P输出PWM
        LQ_Set_Pwm(P3_PWM_P, 0);               // P3_PWM_P输出0
        LQ_Set_Pwm(P5_PWM_P,0);                // P5_PWM_P输出0
      break;

    case 5:
        //A→B=0   B→C=-  C→A=+
        LQ_Set_Pwm(P1_PWM_P, 0);              // P1_PWM_P输出0
        LQ_Set_Pwm(P3_PWM_P, 0);              // P3_PWM_P输出0
        LQ_Set_Pwm(P5_PWM_P,g_sBLDCMag.duty); // P5_PWM_P输出PWM
      break;

    case 6:
        //A→B=+   B→C=0  C→A=-
        LQ_Set_Pwm(P1_PWM_P, g_sBLDCMag.duty); // P1_PWM_P输出PWM
        LQ_Set_Pwm(P3_PWM_P, 0);               // P5_PWM_P输出0
        LQ_Set_Pwm(P5_PWM_P, 0);               // P3_PWM_P输出0
      break;

    default:
      LQ_BLDCStop();
      break;
    }
}

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCReversal(void);
@功能说明：控制无刷电机反转
@参数说明：无
@函数返回：无
@修改时间：2023/12/5
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void LQ_BLDCReversal(void)
{
    switch(g_sBLDCMag.step)
    {
      case 1:
          //A→B=+   B→C=0  C→A=-
          LQ_Set_Pwm(P1_PWM_P, 0-g_sBLDCMag.duty); // P1_PWM_P输出PWM
          LQ_Set_Pwm(P5_PWM_P, 0);                 // P5_PWM_P输出0
          LQ_Set_Pwm(P3_PWM_P, 0);                 // P3_PWM_P输出0
        break;

      case 2:
          //A→B=0  B→C=-  C→A=+
          LQ_Set_Pwm(P1_PWM_P, 0);                 // P1_PWM_P输出0
          LQ_Set_Pwm(P3_PWM_P, 0);                 // P3_PWM_P输出0
          LQ_Set_Pwm(P5_PWM_P, 0-g_sBLDCMag.duty); // P5_PWM_P输出PWM
        break;

      case 3:
          //A→B=+   B→C=0  C→A=-
          LQ_Set_Pwm(P5_PWM_P, 0);                 // P5_PWM_P输出0
          LQ_Set_Pwm(P1_PWM_P, 0-g_sBLDCMag.duty); // P1_PWM_P输出PWM
          LQ_Set_Pwm(P3_PWM_P, 0);                 // P3_PWM_P输出0
        break;

      case 4:
          //A→B=-   B→C=+  C→A=0
          LQ_Set_Pwm(P5_PWM_P, 0);                 // P5_PWM_P输出0
          LQ_Set_Pwm(P3_PWM_P, 0-g_sBLDCMag.duty); // P3_PWM_P输出PWM
          LQ_Set_Pwm(P1_PWM_P, 0);                 // P1_PWM_P输出0
        break;

      case 5:
          //A→B=-   B→C=+  C→A=0
          LQ_Set_Pwm(P5_PWM_P, 0);                 // P5_PWM_P输出0
          LQ_Set_Pwm(P3_PWM_P, 0-g_sBLDCMag.duty); // P3_PWM_P输出PWM
          LQ_Set_Pwm(P1_PWM_P, 0);                 // P1_PWM_P输出0
        break;

      case 6:
          //A→B=0   B→C=-  C→A=+
          LQ_Set_Pwm(P1_PWM_P, 0);                 // P1_PWM_P输出0
          LQ_Set_Pwm(P3_PWM_P, 0);                 // P3_PWM_P输出0
          LQ_Set_Pwm(P5_PWM_P, 0-g_sBLDCMag.duty); // P5_PWM_P输出PWM
        break;

      default:
        LQ_BLDCStop();
        break;
    }
}

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCStop(void);
@功能说明：关闭无刷电机，停止运行
@参数说明：无
@函数返回：无
@修改时间：2023/12/5
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void LQ_BLDCStop(void)
{
    LQ_Pin_Write(P4_PWM_P,0);  //关断C-
    LQ_Pin_Write(P2_PWM_P,0);  //关断B-
    LQ_Set_Pwm(P1_PWM_P, 0);   //关断B+
    LQ_Set_Pwm(P5_PWM_P, 0);   //关断A+
    /**********guanduan********************/
    LQ_Set_Pwm(P3_PWM_P, 0);   //关断C+
    LQ_Pin_Write(P6_PWM_P,0);  //关断A-

}
#else   //USE2136

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCCorotation(void);
@功能说明：控制无刷电机正转
@参数说明：无
@函数返回：无
@修改时间：2023/12/5
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void LQ_BLDCCorotation(void)
{
    switch(g_sBLDCMag.step)
    {
      case 1:
        //C->B
        LQ_Set_Pwm( P1_PWM_P, 0);                    // P1_PWM_P关闭
        LQ_Set_Pwm( P3_PWM_P, 200);                  // P3_PWM_P输出0
        LQ_Set_Pwm( P5_PWM_P, 200-g_sBLDCMag.duty);  // P5_PWM_P输出PWM
        LQ_Pin_Write( P2_PWM_P, 0);                  //关闭
        LQ_Pin_Write( P4_PWM_P, 0);                  //关闭
        LQ_Pin_Write( P6_PWM_P, 1);                  //打开
        break;

      case 2:
        //A->C
        LQ_Set_Pwm( P1_PWM_P, 200-g_sBLDCMag.duty);   // P231输出PWM
        LQ_Set_Pwm( P3_PWM_P, 0);                     // P3_PWM_P关闭
        LQ_Set_Pwm( P5_PWM_P, 200);                   // P5_PWM_P输出0
        LQ_Pin_Write( P2_PWM_P, 1);                   //打开
        LQ_Pin_Write( P4_PWM_P, 0);                   //关闭
        LQ_Pin_Write( P6_PWM_P, 0);                   //关闭
        break;

      case 3:
        //A->B
        LQ_Set_Pwm( P1_PWM_P, 200-g_sBLDCMag.duty);  // P1_PWM_P输出0
        LQ_Set_Pwm( P3_PWM_P, 200);                  // P3_PWM_P输出PWM
        LQ_Set_Pwm( P5_PWM_P, 0);                    // P5_PWM_P输出0
        LQ_Pin_Write( P2_PWM_P, 1);                  //打开
        LQ_Pin_Write( P4_PWM_P, 0);                  //关闭
        LQ_Pin_Write( P6_PWM_P, 0);                  //关闭
        break;

      case 4:
        //B->A
        LQ_Set_Pwm( P1_PWM_P, 200);                   // P1_PWM_P输出0
        LQ_Set_Pwm( P3_PWM_P, 200-g_sBLDCMag.duty);   // P3_PWM_P输出PWM
        LQ_Set_Pwm( P5_PWM_P, 0);                     // P5_PWM_P关闭
        LQ_Pin_Write( P2_PWM_P, 0);                   //关闭
        LQ_Pin_Write( P4_PWM_P, 1);                   //打开
        LQ_Pin_Write( P6_PWM_P, 0);                   //关闭
        break;

      case 5:
        //C->A
        LQ_Set_Pwm( P1_PWM_P, 200);                    // P1_PWM_P输出PWM
        LQ_Set_Pwm( P3_PWM_P, 0);                      // P3_PWM_P输出0
        LQ_Set_Pwm( P5_PWM_P, 200-g_sBLDCMag.duty);    // P5_PWM_P输出0
        LQ_Pin_Write( P2_PWM_P, 0);                    //关闭
        LQ_Pin_Write( P4_PWM_P, 0);                    //关闭
        LQ_Pin_Write( P6_PWM_P, 1);                    //打开
        break;

      case 6:
        //B->C
        LQ_Set_Pwm( P1_PWM_P, 0);                       // P1_PWM_P输出PWM
        LQ_Set_Pwm( P3_PWM_P, 200-g_sBLDCMag.duty);     // P3_PWM_P输出0
        LQ_Set_Pwm( P5_PWM_P, 200);                     // P5_PWM_P输出0
        LQ_Pin_Write( P2_PWM_P, 0);                     //关闭
        LQ_Pin_Write( P4_PWM_P, 1);                     //打开
        LQ_Pin_Write( P6_PWM_P, 0);                     //关闭
        break;
      default:
        LQ_BLDCStop();
        break;
    }
}

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCReversal(void);
@功能说明：控制无刷电机反转
@参数说明：无
@函数返回：无
@修改时间：2023/12/5
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void LQ_BLDCReversal(void)
{
    switch(g_sBLDCMag.step)
    {
      case 1:
        //B->C
        LQ_Set_Pwm(P1_PWM_P, 0);                        // P1_PWM_P输出PWM
        LQ_Set_Pwm(P3_PWM_P, 200+g_sBLDCMag.duty);      // P3_PWM_P输出0
        LQ_Set_Pwm(P5_PWM_P, 200);                      // P5_PWM_P输出0
        LQ_Pin_Write(P2_PWM_P, 0);                      //关闭
        LQ_Pin_Write(P4_PWM_P, 1);                      //打开
        LQ_Pin_Write(P6_PWM_P, 0);                      //关闭
        break;

      case 2:
        //C->A
        LQ_Set_Pwm(P1_PWM_P, 200);                      // P1_PWM_P输出0
        LQ_Set_Pwm(P3_PWM_P, 0);                        // P3_PWM_P输出0
        LQ_Set_Pwm(P5_PWM_P, 200+g_sBLDCMag.duty);      // P5_PWM_P输出PWM
        LQ_Pin_Write(P2_PWM_P,0);                       //关闭
        LQ_Pin_Write(P4_PWM_P,0);                       //关闭
        LQ_Pin_Write(P6_PWM_P,1);                       //打开
        break;
//
      case 3:
        //B->A
        LQ_Set_Pwm(P1_PWM_P, 200);                      // P1_PWM_P输出PWM
        LQ_Set_Pwm(P3_PWM_P, 200+g_sBLDCMag.duty);      // P3_PWM_P输出0
        LQ_Set_Pwm(P5_PWM_P, 0);                        // P5_PWM_P输出0
        LQ_Pin_Write(P2_PWM_P,0);                       //关闭
        LQ_Pin_Write(P4_PWM_P,1);                       //打开
        LQ_Pin_Write(P6_PWM_P,0);                       //关闭
        break;

      case 4:
        //A->B
        LQ_Set_Pwm(P1_PWM_P, 200+g_sBLDCMag.duty);      // P1_PWM_P输出0
        LQ_Set_Pwm(P3_PWM_P, 200);                      // P3_PWM_P输出PWM
        LQ_Set_Pwm(P5_PWM_P, 0);                        // P5_PWM_P输出0
        LQ_Pin_Write(P2_PWM_P,1);                       //打开
        LQ_Pin_Write(P4_PWM_P,0);                       //关闭
        LQ_Pin_Write(P6_PWM_P,0);                       //关闭
        break;

      case 5:
        //A->C
        LQ_Set_Pwm(P1_PWM_P, 200+g_sBLDCMag.duty);      // P1_PWM_P输出0
        LQ_Set_Pwm(P3_PWM_P, 0);                        // P3_PWM_P输出PWM
        LQ_Set_Pwm(P5_PWM_P, 200);                      // P5_PWM_P输出0
        LQ_Pin_Write(P2_PWM_P,1);                       //打开
        LQ_Pin_Write(P4_PWM_P,0);                       //关闭
        LQ_Pin_Write(P6_PWM_P,0);                       //关闭
        break;

      case 6:
        //C->B
        LQ_Set_Pwm(P1_PWM_P, 0);                        // P1_PWM_P输出0
        LQ_Set_Pwm(P3_PWM_P, 200);                      // P3_PWM_P输出0
        LQ_Set_Pwm(P5_PWM_P, 200+g_sBLDCMag.duty);      // P5_PWM_P输出PWM
        LQ_Pin_Write(P2_PWM_P,0);                       //关闭
        LQ_Pin_Write(P4_PWM_P,0);                       //关闭
        LQ_Pin_Write(P6_PWM_P,1);                       //打开
        break;

      default:
        LQ_BLDCStop();
        break;
    }
}

/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_BLDCStop(void);
@功能说明：关闭无刷电机，停止运行
@参数说明：无
@函数返回：无
@修改时间：2023/12/5
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void LQ_BLDCStop(void)
{
    LQ_Pin_Write(P4_PWM_P,1); 
    LQ_Pin_Write(P2_PWM_P,1); 
    LQ_Pin_Write(P6_PWM_P,1); 
    LQ_Set_Pwm(P1_PWM_P, 0); 
    LQ_Set_Pwm(P5_PWM_P, 0);  
    LQ_Set_Pwm(P3_PWM_P, 0); 
                          
}
#endif
/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：float LPF_velocity(float x);
@功能说明：一阶低通滤波器
@参数说明：传入需滤波的值
@函数返回：返回滤波后结果
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
float LPF_velocity(float x)
{
  float y = 0.8*y_vel_prev + 0.2*x;	
  y_vel_prev=y;	
  return y;
}


/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：int LQ_BLDCCurrent_detection(void);
@功能说明：电流检测，保护电路
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_BLDCCurrent_detection(void)
{
  g_sBLDCMag.counts ++;
  IA = (Read_ADC(ADC0_CH3_P06_3)-2048)*3.3*100/4095/5/0.03;   //U    扩大100倍计算公式：(IA-2048)*3.3*100/4095/5/0.03≈IA/2    //2048是参考电压1.65
  IB = (Read_ADC(ADC0_CH2_P06_2)-2048)*3.3*100/4095/5/0.03;   //V  
  IC = (Read_ADC(ADC0_CH1_P06_1)-2048)*3.3*100/4095/5/0.03;   //W    发现A相电流最大电阻特别容易烧，B相次之，C相最小，
  IA=fabs(IA);
  IB=fabs(IB);
  IC=fabs(IC);
  IA= (uint16_t)LPF_velocity(IA);  //一阶低通滤波
  IB= (uint16_t)LPF_velocity(IB);
  IC= (uint16_t)LPF_velocity(IC);
  while((g_sBLDCMag.Electricity_flag ==true)&&((IA/100>6.0)||(IB/100>6.0)||(IC/100>6.0)))
  {			
    LQ_BLDCStop();
    if(KEY_Read(KEY1)==0){
      g_sBLDCMag.Electricity_flag =false;
      break;
    }
  }
  if(g_sBLDCMag.counts > 1000)//超过2s，视为电机启动完毕不在检测启动电流
    g_sBLDCMag.counts=0,g_sBLDCMag.Electricity_flag =false;
  while((IA/100>6.0)&&(IB/100>6.0)&&(IC/100>6.0))
  {
    LQ_BLDCStop();//堵转后停止输出
    if(KEY_Read(KEY1)==0)break;
  }
}
/**LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void TestMotorBLDC (void);
@功能说明：无刷电机测试函数
@参数说明：无
@函数返回：无
@修改时间：2022/02/24
@备    注：占空比不要过大，20左右，驱动板贴散热片再用
  驱动板使用IR2136无刷驱动 电机使用L车模红色3650无刷电机
  无刷电机接线：
      母板接驱动板：
              PWM :P19_2->pwm1  P19_3->pwm2  P20_0->pwm3  P20_1->pwm4  P20_2->pwm5  P20_3->pwm6

      电机ABC相线判断：排线在轴的上方时  左侧是A相 中间是B相 右侧是C相
                                         橙色      黄色      蓝色
      
      霍尔接线: 红正黑负                 
                P15_2 -> HA（黄色数据线）
                P15_1 -> HB（绿色数据线）
                P15_0 -> HC（蓝色数据线）

      电流检测接线：驱动板SI接口通过PH2.0-4P转杜邦排线接到母板ADC1接口
               驱动板           母板
                GND     ->      GND
                OA      ->      6.3
                OB      ->      6.2
                OC      ->      6.1              
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ**/
void TestMotorBLDC (void)
{
    char txt[36] = {0};
    GPIO_LED_Init(); 
    GPIO_KEY_Init();
    LCD_Init();        //LCD初始化
    LCD_CLS(BLACK);
    LCD_ShowString(15,0,"LQ BLDC Test",WHITE,BLACK,32,0);
    BLDC_MotorInit(MOTOR_FREQUENCY-1);
    BLDC_HallInit();
    LQ_UART_Init_Config(UART7_RX_P23_0, UART7_TX_P23_1, 115200);
//    //电流采集ADC初始化
//    LQ_Init_ADC(ADC0_CH1_P06_1);
//    LQ_Init_ADC(ADC0_CH2_P06_2);
//    LQ_Init_ADC(ADC0_CH3_P06_3);
    
    //定时时间 = 10000/(2000000/4) = 0.02s = 20ms
    LQ_Init_Timer(P02_3_TCP_4, 50, CY_TCPWM_PRESCALER_DIVBY_4);//0.1ms触发一次  电机控制
    //LQ_Init_Timer(P02_4_TCP_3, 25000, CY_TCPWM_PRESCALER_DIVBY_4);//50ms触发一次 电流检测
    
    while(1){
        while ((KEY_Read(KEY2) == 0))      //按下KEY0键，占空比减小
        {
          if (BLDCduty > -200)
            BLDCduty -= 2;
          while ((KEY_Read(KEY2) == 0));
        }
        while((KEY_Read(KEY0) == 0))      //按下KEY2键，占空比加大
        {
          if (BLDCduty < 200)             //满占空比为200
            BLDCduty += 2;
          while((KEY_Read(KEY0) == 0));
        }
        while(KEY_Read(KEY1) == 0)       //按下KEY1键，占空比中值
        {
          if(BLDCduty>0)BLDCduty = -BLDCduty;
          else BLDCduty = -BLDCduty;
          while(KEY_Read(KEY1) == 0);
        }
        sprintf(txt, "HALL:%02X  ", g_sBLDCMag.step);
        LCD_ShowStr_24(1, 1,txt,RED_IPS,LIGHTBLUE); 
        sprintf(txt,"duty:%3d  ",BLDCduty);
        LCD_ShowStr_24(1, 2,txt,RED_IPS,LIGHTBLUE); 
        
        sprintf(txt, "IA:%6.2f  ", IA/100);
        LCD_ShowStr_24(1, 3,txt,RED_IPS,LIGHTBLUE); 
        sprintf(txt, "IB:%6.2f  ", IB/100);
        LCD_ShowStr_24(1, 4,txt,RED_IPS,LIGHTBLUE);          
        sprintf(txt, "IC:%6.2f  ", IC/100);
        LCD_ShowStr_24(1, 5,txt,RED_IPS,LIGHTBLUE);  
        ANO_DT_send_int16byte16((uint16_t)IA,(uint16_t)IB,(uint16_t)IC,0,0,0,0,0); 
        LED_Ctrl(LED1, RVS);       //电平翻转,LED闪烁
    }
}















