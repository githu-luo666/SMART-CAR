#ifndef __Motor_H_
#define __Motor_H_

#include "include.h"
/*******电机初始化********/
#define DIR_L1          (P18_6)
#define DRV_PWM_L2      (TCPWM_CH50_P18_7)

#define DIR_R1          (P00_2)
#define DRV_PWM_R2      (TCPWM_CH13_P00_3)
/*******电机初始化********/

/*******电机控制结构体********/
typedef struct 
{
    uint8_t Yuansu_Flag;    // 元素标志位 0-5
    float yaw_accumulated;
    float left_pwm;            // 左轮PWM值
    float right_pwm;           // 右轮PWM值
    uint8_t Locked;          //直角处理时陀螺仪是否积到45°？如果就不让改变标志位
    uint8_t Yuansu_Triggered;//固定差速只能加减一次
} Motor_Control;

extern Motor_Control Motor_control;
/*******电机控制结构体********/

/*******控制函数声明********/
void DRV8701_Init_D(void);//DRV引脚初始化
void DRV8701_MOTOR_DRIVER_D(int L_SPEED,int R_SPEED);//DRV两个电机速度设置
void Speed_Set(void);
void Speed_Control(void);
void Encoder_Get(void);
void contral(void);
/*******控制函数声明********/

/*******编码器测出的速度********/
extern float Speed_Left_Actual,Speed_Right_Actual,Speed_Actual;
extern float Speed_Left_Actual1,Speed_Right_Actual1,Speed_Actual1,Speed_Actualall;
extern float left_target_speed;
extern float right_target_speed;
extern uint8 run_flag;
/*******编码器测出的速度********/

extern float Base_Speed_PWM1;
extern float Base_Speed_PWM2;
extern float Target_Speed;
extern float Target_Speed1;
extern float Target_Speed_left;
extern float Target_Speed_right;
extern int a;
extern int chasu;
extern int motor_start;

#endif