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
#include "include.h"

/*************************************************************************
*  函数名称：void LQ_motor_init(void)
*  功能说明：初始化单路驱动引脚
*  参数说明：无
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
void LQ_motor_init(void)
{
#ifdef USE7843or7971
  LQ_PWM_Init(P19_2_TCP_27, 10000);
  LQ_PWM_Init(P19_3_TCP_28, 10000);
  LQ_PWM_Init(P20_0_TCP_30, 10000);
  LQ_PWM_Init(P20_1_TCP_49, 10000);
#else 
  LQ_PWM_Init(P19_2_TCP_27, 10000);
  LQ_GPIO_Init_Config(P19_3, GPIO_OUT_STRONG);  
  LQ_PWM_Init(P20_0_TCP_30, 10000);
  LQ_GPIO_Init_Config(P20_1, GPIO_OUT_STRONG);
#endif
}

/*************************************************************************
 *  函数名称：void MotorCtrl (int16_t motor1,int16_t motor2)
 *  功能说明：控制电机转速（单路）
 *  参数说明：   @param    motor   ： 电机1占空比
 *  函数返回：无
 *  修改时间：2025年2月21日
 *  备    注：驱动2个电机
 *************************************************************************/
void MotorCtrl (int16_t motor1,int16_t motor2)
{
#ifdef USE7843or7971
  if (motor2 > 0)
  {
    LQ_Set_Pwm(P19_2_TCP_27, motor2);
    LQ_Set_Pwm(P19_3_TCP_28, 0);
  }
  else
  {
    LQ_Set_Pwm(P19_2_TCP_27, 0);
    LQ_Set_Pwm(P19_3_TCP_28, -motor2);
  }
  if (motor1 > 0)
  {
    LQ_Set_Pwm(P20_0_TCP_30, motor1);
    LQ_Set_Pwm(P20_1_TCP_49, 0);
  }
  else
  {
    LQ_Set_Pwm(P20_0_TCP_30, 0);
    LQ_Set_Pwm(P20_1_TCP_49, -motor1);
  }

#else
  if (motor2 > 0)
  {
    LQ_Set_Pwm(P19_2_TCP_27, motor2);
    LQ_Pin_Write(P19_3, 0);
  }
  else
  {
    LQ_Set_Pwm(P19_2_TCP_27, -motor2);
    LQ_Pin_Write(P19_3,1);
  }
  if (motor1 > 0)
  {
    LQ_Set_Pwm(P20_0_TCP_30, motor1);
    LQ_Pin_Write(P20_1, 1);
  }
  else
  {
    LQ_Set_Pwm(P20_0_TCP_30, -motor1);
    LQ_Pin_Write(P20_1, 0);
  }
#endif
}
/*************************************************************************
 *  函数名称：void Test_motor(void )
 *  功能说明：测试电机接口PWM 控制电机转速
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2023年11月28日
 *  备    注：驱动电机
    连接好驱动板与母板，将母板电源接到驱动板电源输入处，
    先不要接电机
    连接好后打开母板电源，用万用表测量驱动板的输出端，测量其电压大小
  确定最大值：
    按下母板上的按键观察电压变化情况，确定输出电压达到电机的最大输入值，
此时记录下屏幕中的duty数值，然后修改宏定义MAX_DUTY的值为此值，编译后重新下载
 *************************************************************************/
void Test_motor(void)
{
  int16_t duty=0;
  char txt[32];
  
  GPIO_LED_Init(); 
  GPIO_KEY_Init();
  LQ_UART_Init_Config(UART7_RX_P23_0, UART7_TX_P23_1, 115200);
  LQ_Printf("\nUART TEST (driver ver=%d.%d)\n\r", CY_SCB_DRV_VERSION_MAJOR, CY_SCB_DRV_VERSION_MINOR);
  LQ_motor_init();

  LCD_Init();        //LCD初始化
  LCD_CLS(BLACK);
  LCD_ShowString(15,0,"LQ Motor Test",WHITE,BLACK,32,0);
  while(1)
  {
    if(KEY_Read(KEY0) == 0)
    {
      Cy_SysTick_DelayInUs(10000);
      if(KEY_Read(KEY0) == 0)
      {
        if(duty < MAX_DUTY)
          duty += 1;
      }
    }
    if(KEY_Read(KEY1) == 0)
    {
      Cy_SysTick_DelayInUs(10000);
      if(KEY_Read(KEY1) == 0)
      {
        duty = 50;
      }
    }
    if(KEY_Read(KEY2) == 0)
    {
      Cy_SysTick_DelayInUs(10000);
      if(KEY_Read(KEY2) == 0)
      {
        if(duty > -MAX_DUTY)
          duty -= 1;
      }
    }
    LQ_Printf("duty:%06d \n", duty);
    sprintf(txt, "duty:%06d", duty);
    LCD_ShowStr_24(0,2,txt,RED_IPS,LIGHTBLUE); 
    LED_Ctrl(LED0, RVS);
    MotorCtrl(duty,duty);
  }
  
}

/*************************************************************************
*  函数名称：void LQ_servo_init(void)
*  功能说明：初始化舵机引脚
*  参数说明：Hz PWM频率
*  函数返回：无
*  修改时间：2023年11月23日
*  备    注：
*************************************************************************/
void LQ_servo_init(uint32_t Hz)
{
  LQ_PWM_Init(P10_1_TCP_29, Hz);
  LQ_PWM_Init(P10_3_TCP_31, Hz);
  LQ_PWM_Init(P10_4_TCP_32, Hz);
}

/*************************************************************************
 *  函数名称：void ServoCtrl(uint32_t duty)
 *  功能说明：舵机转角函数，由于小车拉杆范围限制，较小
 *  参数说明：short duty，占空比
 *  函数返回：无
 *  修改时间：2023年11月23日
 *  备    注：驱动2个舵机
 *************************************************************************/
void ServoCtrl (uint32_t duty)
{
//    if (duty >= Servo_Left_Max)                  //限制幅值
//        duty = Servo_Left_Max;
//    else if (duty <= Servo_Right_Min)            //限制幅值
//        duty = Servo_Right_Min;

    LQ_Set_Pwm(P10_1_TCP_29, duty);
    LQ_Set_Pwm(P10_3_TCP_31, duty);
    LQ_Set_Pwm(P10_4_TCP_32, duty);
}
/*************************************************************************
 *  函数名称：void Test_servo(void )
 *  功能说明：测试舵机接口PWM 控制舵机角度
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2024年11月28日
 *  备    注：
    测试舵机前需要将舵机接口的中间引脚电压调至5-6V，通过母版上开关附近的蓝色
  电位器调整该电压。
  输入舵机的PWM频率一般为50Hz，当占空比、频率、电压不合适时舵机会出现无力状态，
  因为舵机都有固定的限制角度，所以我们尽可能的将舵机处于中间位置时安装到车上，
  步骤如下：
    1、将下面程序中的“if (duty > Servo_Left_Max)”以及“if(duty1 > Servo_Right_Min)”
       注释掉，将“ServoCtrl（）”函数中的限制幅值语句注释掉。
    2、将母板上舵机接口的电压调整到5V，调整初始化频率为50或100Hz
    3、烧录程序，将舵机插入舵机接口。按按键调整舵机PWM占空比，从0开始到3000之间
       如果舵机一直无反应，则修改初始化的频率后再次测试。
    4、记录舵机的极限值，例如：最小值为200，最大值为500。则舵机的中值为350，左右极限为±150
    5、修改宏定义“Servo_Center_Mid”的值为350。修改“Servo_Delta”的值为150。
    6、重新打开步骤1中注释的语句，编译后进行烧录。
 *************************************************************************/
void Test_servo(void)
{
  int16_t duty1=Servo_Center_Mid;
  char txt[32];
  
  GPIO_LED_Init(); 
  GPIO_KEY_Init();
  LQ_UART_Init_Config(UART7_RX_P23_0, UART7_TX_P23_1, 115200);
  LQ_Printf("\nUART TEST (driver ver=%d.%d)\n\r", CY_SCB_DRV_VERSION_MAJOR, CY_SCB_DRV_VERSION_MINOR);
  LQ_servo_init(50);
  
  LCD_Init();        //LCD初始化
  LCD_CLS(BLACK);
  LCD_ShowString(15,0,"LQ Servo Test",WHITE,BLACK,32,0);
  
  while(1)
  {
    if(KEY_Read(KEY0) == 0)
    {
      Cy_SysTick_DelayInUs(10000);
      if(KEY_Read(KEY0) == 0)
      {
        if(duty1 < Servo_Left_Max)
          duty1 += 10;
      }
    }
    if(KEY_Read(KEY1) == 0)
    {
      Cy_SysTick_DelayInUs(10000);
      if(KEY_Read(KEY1) == 0)
      {
        duty1 = Servo_Center_Mid;
      }
    }
    if(KEY_Read(KEY2) == 0)
    {
      Cy_SysTick_DelayInUs(10000);
      if(KEY_Read(KEY2) == 0)
      {
        if(duty1 > Servo_Right_Min)
          duty1 -= 10;
      }
    }
    LQ_Printf("duty:%06d \n", duty1);
    sprintf(txt, "duty:%06d", duty1);
    LCD_ShowStr_24(0,2,txt,RED_IPS,LIGHTBLUE); 
    LED_Ctrl(LED0, RVS);
    ServoCtrl(duty1);
  }
}








