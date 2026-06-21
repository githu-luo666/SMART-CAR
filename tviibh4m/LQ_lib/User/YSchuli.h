#ifndef __YSchuli_H_
#define __YSchuli_H_

#include "include.h"
typedef struct {
    float filtered;
    float alpha;  // 滤波系数(0.0-1.0)，值越小滤波越强
} LowPassFilter;

extern LowPassFilter left_lpf;
extern LowPassFilter right_lpf;

#define YS_HEIGHT 100
#define YS_WIDTH 114
extern float jiaodu;
extern float gyro_bias;
uint8 Yuansu(uint8_t(*ni_img)[114]) ;
float LowPass_Update(LowPassFilter* filter, float new_value);
void update_yaw_accumulated();
void Encoder_Filter_Init(float alpha); // 新增初始化函数
#endif  // __CONTROL_H__
