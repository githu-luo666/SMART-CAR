#include "include.h"

int main(void)
{
    LQ_motor_init();                                              // 电机初始化
    // 系统初始化
    SystemInit();
    __enable_irq();
    Cy_Cpu_CoreSlowClkDividerSet(0);
    Cy_Cpu_CorePeriDividerSet(0);

/************************************************************************************************************************************************
    以下为模块测试函数，每一个Test_xx中都含有死循环，可用来测试学习板，需要测试哪个模块就将相应Test_xx取消注释即可。
*************************************************************************************************************************************************/     
//    Test_LED();         //PASS, 核心板LED测试程序， 绿灯:P05.4  蓝灯:P08.3 红灯:P19.4 注意，可能其他核中有LED闪灯程序造成不同步闪烁
//    Test_Bluetooth();   //PASS, 测试蓝牙接口，蓝牙接口与下载器接口处的串口用的是同一个 串口号:UART7 RX:P23.0 TX:P23.1
//  Test_GPIO_KEY();    //PASS, 测试母板按键，按键按下LED等亮起，注意其他核中可能调用LED灯。
//    Test_reed();        //PASS, 测试母板上干簧管接口，触发后核心板上LED0（绿灯）翻转，注意，可能短时间内有多次触发，实际使用时需注意
//    Test_buzzer();      //PASS, 测试母板上蜂鸣器，采用宏定义形式，引脚P03.0
//    Test_IPSLCD();      //PASS, 测试IPS屏幕，宏定义切换屏幕尺寸，mini主板可选用屏幕尺寸包括1.14寸、1.3寸和1.54寸
  Test_motor();       //PASS, 测试母板电机接口，单路驱动8701 7971通过宏定义切换。
//    Test_servo();       //PASS, 测试舵机，舵机接口输出PWM频率在程序中进行修改,MINI母板上无刷1、无刷2接口也可以驱动舵机，如果使用直流电机做负压可使用转接板连接8701驱动。
//  Test_counter();     //PASS, 测试母板编码器，屏幕显示，串口输出到上位机。暂时仅支持带方向输出的编码器，正交解码还在开发中。
// Test_SPI_Gyro();    //PASS，测试母版上陀螺仪接口，在屏幕上显示数据，自动识别20689、20602、42605、42688陀螺仪；
//   Test_IIC_Gyro();    //PASS，测试母版上IIC陀螺仪，在屏幕上显示数据，软件I2C接口，SCL-->P12_2;SDA-->P12_1;
//    Test_timer();       //PASS, 测试定时器,定时20ms。  
//    Test_EEPROM();      //PASS, 测试EEPROM读写功能，存放数据掉电不丢失。
//  Test_ipc_M0();      //PASS, 测试不同核之间的数据交换，此历程可以将M0中的数据传输到M7_0中，需要同时开启M7_0中的此函数，可以在TFT1.8屏幕上显示传输的数据
// Test_MT9V034();     //PASS, 测试神眼摄像头。
/************************************************************************************************************************************************
    以下模块测试函数在mini母板上没有用到，大家可以拓展学习使用。
*************************************************************************************************************************************************/         
//    Test_TFT18();       //PASS, 测试TFT1.8屏幕，宏定义切换 软硬件SPI，硬件SPI比软件SPI快7倍多，注意：屏幕初始化，软件SPI使用强输出模式，推挽输出对个别屏幕可能导致不显示
//    Test_ADC();         //PASS, 测试母板ADC功能，MINI母板没有ADC接口
//    Test_OLED();        //PASS, 测试母板OuijLED0.96寸屏幕，暂时采用软件SPI通信。通过宏定义修改引脚（CK-P02_4 DI-P02_0 RST-P02_1 DC-P02_3）
 //   Test_ipc_M71(); 
//    Test_motor();       //PASS, 测试母板电机接口，单路驱动8701 7971通过宏定义切换。
// 初始化 用户代码

    GPIO_KEY_Init();                                              // 按键初始化
    MT9V034_Init(100);                                            // 摄像头初始化
    LCD_Init();                                                   // 屏幕初始化
    LCD_CLS(u16BLACK);                                            // 清屏
    MT9V034_Init(100);                                            // 摄像头初始化
    LQ_Init_Timer(P06_5_TCP_2, 5000, CY_TCPWM_PRESCALER_DIVBY_4); // 10ms定时器初始化
    LQ_Init_Timer(P06_1_TCP_0, 5000,CY_TCPWM_PRESCALER_DIVBY_8); // 10ms定时器初始化
  // LQ_Init_Timer(P06_7_TCP_3, 10000,CY_TCPWM_PRESCALER_DIVBY_16); // 10ms定时器初始化
    LQ_Counter_Init_SDZ(P05_0_TCP_9, P05_1);
    LQ_Counter_Init_SDZ(P05_2_TCP_11, P05_3);
    IncPID_Init(&L_MOTOR_PID,Left_MOTOR); //左电机增量式PID初始化
    IncPID_Init(&R_MOTOR_PID,Right_MOTOR);//右电机增量式PID初始化
    IncPID_Init(&Speed_PID,Speed_MOTOR);//整体速度增量式PID初始化
    Encoder_Filter_Init(0.3f);
    LQ_UART_Init_Config(UART7_RX_P23_0, UART7_TX_P23_1, 115200);
    Soft_SPI_Init();
    char txt[32];
    unsigned  char res;
    res = SPI_Gryo_Init();
    speedp=0;
    speedi=0;
    time=0;
    Target_Speed1=70;


    // 主循环
    for (;;)
    {
        PID_Init();
        if(mt9v034Flag)         //在屏幕上显示采集的图像
        {
        Get_image(mt9v034Image);//复制原图像到数组
        Image_Binarization();//二值化
//        sobel(mt9v03x_image, image_two_value, 100);
//        image_filter(image_two_value);//形态学滤波
        circular();//圆环
        findline(); //扫线
        stright_angle();//直角
 //            cross();//十字
 //      open_flag= is_duanlu();
       get_error();
       if(motor_start==0)
       {
      drawkline();
      LCDSPI_Road(0, 0, 60, 94, (unsigned char *)&image_two_value[0][0]);  // 图像显示
       }
         mt9v034Flag = 0;            //处理完毕
        }
    LQ_Printf("%f,%f,%f,%f,%f\n",(float)Base_Speed_PWM1,(float)Base_Speed_PWM2,(float)Speed_Left_Actual,(float)Speed_Right_Actual,(float)Target_Speed);
    sprintf((char*)txt,"Target_Speed1:%f",Target_Speed1);
    LCD_ShowString(80,16,txt,WHITE,BLACK,16,0);
    sprintf((char*)txt,"KP2:%f",KP2);
    LCD_ShowString(80,80,txt,WHITE,BLACK,16,0);
    sprintf((char*)txt,"KP:%f",KP);
    LCD_ShowString(80,96,txt,WHITE,BLACK,16,0);
    sprintf((char*)txt,"GKD:%f",GKD);
    LCD_ShowString(80,112,txt,WHITE,BLACK,16,0);
    sprintf((char*)txt,"Right_Actual1:%f",Motor_control.right_pwm);
    LCD_ShowString(80,32,txt,WHITE,BLACK,16,0);
    sprintf((char*)txt,"Left_Actual:%f",Motor_control.left_pwm);
    LCD_ShowString(80,48,txt,WHITE,BLACK,16,0);
    sprintf((char*)txt,"current_yaw:%f", Motor_control.yaw_accumulated);
    LCD_ShowString(80,64,txt,WHITE,BLACK,16,0);
    Target();
    } 
}

// 10毫秒定时器中断服务程序
void timer_handler_10ms(void)
{
  if(motor_start==1) Target_Speed=Target_Speed1;
  else Target_Speed=0;
  
Encoder_Get();
//Speed_Set   ();
//Speed_Control();
contral();


}
void timer_handler_05ms(void)
{
    ICM_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);   //得到加速度传感器数据
    if( cir_turn_right== 1 || cir_turn_left==1||cir_turn_right==2||cir_turn_left==2)
    {
      update_yaw_accumulated();  //陀螺仪积分
    }
//        if( open_flag== 1 &&youxian==0)
//    {
//      update_yaw_accumulated();  //陀螺仪积分
//    }
//    
    

}


void  timer_handler_5ms(void)
{

}