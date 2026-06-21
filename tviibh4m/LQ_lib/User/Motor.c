#include "include.h"
int spd;
/*************速度环速度*************/
int Speed_Left=60,Speed_Right=60,Speed_1,Speed_2;
/*************速度环速度*************/

float Speed_Left_Actual1=0,Speed_Right_Actual1=0,Speed_Actual1=0,Speed_Actualall=0;
float Speed_Left_Actual=0,Speed_Right_Actual=0,Speed_Actual=0;
float Base_Speed_PWM1=0;
float Base_Speed_PWM2=0;
float Target_Speed=0;
float Target_Speed1=0;
float Target_Speed_left=0;
float Target_Speed_right=0;
int motor_start=0;
int xianfu=70;
int a=1;
uint8 run_flag=0;
/*************固定差速大小*************/
int chasu =2;
/*************固定差速大小*************/

/*************固定差速大小*************/
#define Huandao_Guding_Chasu                3
/*************固定差速大小*************/

// 全局状态变量定义
Motor_Control Motor_control = {0};

//根据元素标志位采取不同的电机控制方案
void Encoder_Get(void)
{
      //  Chasu_PWM = Camera_Error_Get();        //普通搜线得到的偏差
    Speed_Left_Actual1 = (-Read_Encoder_SDZ(P05_0_TCP_9,  P05_1));
    Speed_Right_Actual1 = (Read_Encoder_SDZ(P05_2_TCP_11, P05_3))/2;
    
    Speed_Left_Actual = (int)LowPass_Update(&left_lpf, Speed_Left_Actual1);
    Speed_Right_Actual =(int)LowPass_Update(&right_lpf, Speed_Right_Actual1);

   Speed_Actualall=(Speed_Left_Actual+Speed_Right_Actual)/2;
    Chasu_PWM = Camera_Error_Get();        //普通搜线得到的偏差     
//   Target_Speed_left=Target_Speed-Chasu_PWM;
//   Target_Speed_right=Target_Speed+Chasu_PWM;

// Target_Speed_left=Target_Speed;
// Target_Speed_right=Target_Speed;
}


float left_target_speed;
float right_target_speed;
float speed_diff = 0;
void contral(void)
{
     
// speed_diff = Position_PID(&turn_pid , image_error , gyrox);
      
//  left_target_speed=Target_Speed-Chasu_PWM;
//  right_target_speed=Target_Speed+Chasu_PWM;
//     left_target_speed= Target_Speed-Chasu_PWM;
//    right_target_speed =Target_Speed+Chasu_PWM;
    left_target_speed= Target_Speed;
   right_target_speed =Target_Speed;
 // Base_Speed_PWM1 += PID_Realize(&Speed_PID, Speed_Left_Actual1, Target_Speed_left); // 整体行进速度闭环
  //Base_Speed_PWM2 += PID_Realize(&R_MOTOR_PID, Speed_Right_Actual1, Target_Speed_right); // 整体行进速度闭环
    Base_Speed_PWM1 = IncrementalPID_L(&pid_L , left_target_speed ,Speed_Actualall);
    Base_Speed_PWM2  = IncrementalPID_R(&pid_R , right_target_speed ,Speed_Actualall);


    
    if(Base_Speed_PWM1  > xianfu)Base_Speed_PWM1  = xianfu;
    if(Base_Speed_PWM1  < -xianfu)Base_Speed_PWM1  = -xianfu;
     
    if(Base_Speed_PWM2 > xianfu)Base_Speed_PWM2 = xianfu;
    if(Base_Speed_PWM2 < -xianfu)Base_Speed_PWM2 = -xianfu;
    
//  Motor_control.left_pwm  =Base_Speed_PWM1+Chasu_PWM;
//  Motor_control.right_pwm =Base_Speed_PWM2-Chasu_PWM;
    
  Motor_control.left_pwm  =Base_Speed_PWM1;
  Motor_control.right_pwm =Base_Speed_PWM2 ; 

//     Motor_control.left_pwm  =-Out_dn ;
 //    Motor_control.right_pwm =Out_dn ; 
    
    if(Motor_control.left_pwm  > xianfu)Motor_control.left_pwm  = xianfu;
    if(Motor_control.left_pwm  < -xianfu)Motor_control.left_pwm  = -xianfu;
     
    if(Motor_control.right_pwm > xianfu)Motor_control.right_pwm = xianfu;
    if(Motor_control.right_pwm < -xianfu)Motor_control.right_pwm = -xianfu;

  

 //     MotorCtrl( (int)  Motor_control.left_pwm, (int)   Motor_control.right_pwm);
     MotorCtrl( (int)  Motor_control.right_pwm, (int)   Motor_control.left_pwm);
  //      MotorCtrl( 30, 30 );
}