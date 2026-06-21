#include "include.h"
#define YS_HEIGHT 100
#define YS_WIDTH 114
//------------------------------------------------------------------
// 角度累积计算（优化积分逻辑）
//------------------------------------------------------------------
#define GYRO_THRESHOLD 4.0f
#define SAMPLE_TIME    0.01f

LowPassFilter left_lpf = {0};
LowPassFilter right_lpf = {0};
float jiaodu;
float gyro_bias = 0.0f;  // 全局变量
void update_yaw_accumulated()
{
    static float last_gyro_z = 0;
    float gyro_z = gyrox / 32.8f;  // 补偿零偏
   
    // 死区处理
    if(fabsf(gyro_z) < GYRO_THRESHOLD) gyro_z = 0;
    
    // 梯形积分提高精度
    Motor_control.yaw_accumulated += (gyro_z + last_gyro_z) * 0.5f * SAMPLE_TIME; //积分
//    if(a==1)
//    {
//        Motor_control.yaw_accumulated= Motor_control.yaw_accumulated+129;
//        a=0;
//    }

    last_gyro_z = gyro_z;
}

float LowPass_Update(LowPassFilter* filter, float new_value)
{
    filter->filtered = filter->alpha * new_value + 
                      (1 - filter->alpha) * filter->filtered;
    return filter->filtered;
}

void Encoder_Filter_Init(float alpha)
{
    left_lpf.filtered = 0;    // 操作全局变量
    left_lpf.alpha = alpha;   // 操作全局变量
    right_lpf.filtered = 0;   // 操作全局变量
    right_lpf.alpha = alpha;  // 操作全局变量
}