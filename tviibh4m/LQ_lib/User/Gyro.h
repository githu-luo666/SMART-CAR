#ifndef _GRYO_H_
#define _GRYO_H_
#include "include.h"

//#define PI 3.1415f
#define delta_T 0.001f //1ms

//extern float X,Y,Z;
//extern float acc_x,acc_y,acc_z;
//extern float gyro_x,gyro_y,gyro_z;
//extern float Q0,Q1,Q2,Q3;//记录四元数 初始化1 0 0 0
//extern float I_ex, I_ey, I_ez;  // 误差积分
//extern float icm_kp;    // 加速度计的收敛速率比例增益
//extern float icm_ki;   // 陀螺仪收敛速率的积分增益
//extern float pitch, roll,yaw;
//extern float ang_z;
//extern int32_t GyroOn;
//extern uint8 GyroINT;
//extern float offset_;
//
//void gyroOffsetInit(void);
//float myRsqrt(float num);
//void ImuGetValues(void);
//void ImuAHRSupdate(void);
//void ReadGyro(void);
//void ClearGyro(void);
//void GyroResolve(void);
//*************************************************************************************版本2***************************************************************************************



extern float ang_z;
extern int32_t GyroOn;
extern uint8 GyroINT;
extern float current_yaw;
extern float lv_gyro_z;
float GetFusedZAngle(void);//卡尔曼滤波四元数解算
float GetContinuousZAngle(void);
void calibrateGyro(void);//除零漂
void ResetYawZero(void);//清零
float GetCurrentYaw(void);
void GyroResolve(void);


#endif

