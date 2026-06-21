#include "Gyro.h"
#include "include.h"
//float X,Y,Z;
//float acc_x,acc_y,acc_z;
//float gyro_x,gyro_y,gyro_z;
//float Q0=1,Q1=0,Q2=0,Q3=0;//记录四元数 初始化1 0 0 0
//float I_ex, I_ey, I_ez;  // 误差积分
//float icm_kp= 0.17;    // 加速度计的收敛速率比例增益
//float icm_ki= 0.004;   // 陀螺仪收敛速率的积分增益
//float pitch, roll,yaw;
//float ang_z=0;//Z轴角速度
//float offset_=0.0;//1m偏移值
////float a,b,c;
//int32_t GyroOn=0;//是否解算
//uint8 GyroINT=1;
//
//
////校准数据
//
//void gyroOffsetInit(void)
//{
//    uint16 i;
//    for (i = 0; i < 2000; ++i)
//    {
//        imu660ra_get_gyro();// 获取陀螺仪角速度
//       offset_ += imu660ra_gyro_z;
//              // 最大 1Khz
//        system_delay_ms(4);
//    }
//
//
//      offset_ /= 4000;
//    
//}
//
//
//
//
//
//
//
//
//float myRsqrt(float num)
//{
//    float halfx = 0.5f * num;
//    float y = num;
//    long i = *(long*)&y;
//    i = 0x5f375a86 - (i >> 1);
//
//    y = *(float*)&i;
//    y = y * (1.5f - (halfx * y * y));
//
//    return y;
//}
//
//void ImuGetValues(void)
//{
//    float alpha = 0.3;
//
//    //一阶低通滤波，单位g
//    acc_x = (((float) imu660ra_acc_x) * alpha) / 4096 + acc_x * (1 - alpha);
//    acc_y = (((float) imu660ra_acc_y) * alpha) / 4096 + acc_y * (1 - alpha);
//    acc_z = (((float) imu660ra_acc_z) * alpha) / 4096 + acc_z * (1 - alpha);
//
//    //! 陀螺仪角速度必须转换为弧度制角速度: deg/s -> rad/s
//    //gyro_x = (double)(((float) imu660ra_gyro_x - X)) * PI / 180 / 16.4f;
//    //gyro_y = (double)(((float) imu660ra_gyro_y - Y)) * PI / 180 / 16.4f;
//    //gyro_z = (double)(((float) imu660ra_gyro_z - Z)) * PI / 180 / 16.4f;
//    
//    gyro_x = (((float) imu660ra_gyro_x - X) * (float)PI / 180 / 16.4f);
//    gyro_y = (((float) imu660ra_gyro_y - Y) * (float)PI / 180 / 16.4f);
//    gyro_z = (((float) imu660ra_gyro_z - Z) * (float) PI / 180 / 16.4f);
//    
//}
//
//void ImuAHRSupdate(void)
//{
//    float halfT = 0.5 * delta_T;    // 采样周期一半
//    float vx, vy, vz;               // 当前姿态计算得来的重力在三轴上的分量
//    float ex, ey, ez;               // 当前加速计测得的重力加速度在三轴上的分量与用当前姿态计算得来的重力在三轴上的分量的误差
//
//    float q0 = Q0;  //四元数
//    float q1 = Q1;
//    float q2 = Q2;
//    float q3 = Q3;
//
//    float q0q0 = q0 * q0;  //先相乘，方便后续计算
//    float q0q1 = q0 * q1;
//    float q0q2 = q0 * q2;
// //   float q0q3 = q0 * q3;
//    float q1q1 = q1 * q1;
// //   float q1q2 = q1 * q2;
//    float q1q3 = q1 * q3;
//    float q2q2 = q2 * q2;
//    float q2q3 = q2 * q3;
//    float q3q3 = q3 * q3;
//    float norm;
//    // 正常静止状态为-g 反作用力。
////    if(acc_x * acc_y * acc_z == 0) // 加计处于自由落体状态时(此时g = 0)不进行姿态解算，因为会产生分母无穷大的情况
////        return;
//
////      if(acc_y<0.01)return;
//
//    // 对加速度数据进行归一化 得到单位加速度 (a^b -> 载体坐标系下的加速度)
//    float acc_sum = acc_x * acc_x + acc_y * acc_y + acc_z * acc_z;
//   
//        norm = myRsqrt(acc_sum);
//        acc_x = acc_x * norm;
//        acc_y = acc_y * norm;
//        acc_z = acc_z * norm;
//    
//
//    // 载体坐标系下重力在三个轴上的分量
//    vx = 2 * (q1q3 - q0q2);
//    vy = 2 * (q0q1 + q2q3);
//    vz = q0q0 - q1q1 - q2q2 + q3q3;
//
//    // g^b 与 a^b 做向量叉乘，得到陀螺仪的校正补偿向量e的系数
//    ex = acc_y * vz - acc_z * vy;
//    ey = acc_z * vx - acc_x * vz;
//    ez = acc_x * vy - acc_y * vx;
//
//    // 误差累加
//    I_ex += halfT * ex;
//    I_ey += halfT * ey;
//    I_ez += halfT * ez;
////      if(I_ey>0.1)I_ey=0;
////      if(I_ex>0.01)I_ex=0;
//
//
//    // 使用PI控制器消除向量积误差(陀螺仪漂移误差)
//    gyro_x = gyro_x + icm_kp* ex + icm_ki* I_ex;
//    gyro_y = gyro_y + icm_kp* ey + icm_ki* I_ey;
//    gyro_z = gyro_z + icm_kp* ez + icm_ki* I_ez;
//
//    // 一阶龙格库塔法求解四元数微分方程，其中halfT为测量周期的1/2，gx gy gz为b系陀螺仪角速度。
//    q0 = q0 + (-q1 * gyro_x - q2 * gyro_y - q3 * gyro_z) * halfT;
//    q1 = q1 + (q0 * gyro_x + q2 * gyro_z - q3 * gyro_y) * halfT;
//    q2 = q2 + (q0 * gyro_y - q1 * gyro_z + q3 * gyro_x) * halfT;
//    q3 = q3 + (q0 * gyro_z + q1 * gyro_y - q2 * gyro_x) * halfT;
//
//    // 单位化四元数在空间旋转时不会拉伸，仅有旋转角度，下面算法类似线性代数里的正交变换
//   float q_sum = q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3;
//   
//        norm = myRsqrt(q_sum);
//        Q0 = q0 * norm;
//        Q1 = q1 * norm;
//        Q2 = q2 * norm;
//        Q3 = q3 * norm;  // 用全局变量记录上一次计算的四元数值
//    
//}
//
//void ReadGyro(void)
//{
//    // atan2返回输入坐标点与坐标原点连线与X轴正方形夹角的弧度值
//   // pitch = asin((double)(2 * Q0 * Q2 - 2 * Q1 * Q3)) * 180 / PI;
//   // roll = atan2((double)(2 * Q2 * Q3 + 2 * Q0 * Q1), (double)(-2 * Q1 * Q1 - 2 * Q2 * Q2 + 1)) * 180 / PI;
//   //yaw = atan2((double)(2 * Q1 * Q2 + 2 * Q0 * Q3), (double)(-2 * Q2 * Q2 - 2 * Q3 * Q3 + 1)) * 180 / PI;
//
//    pitch = asinf(2 * Q0 * Q2 - 2 * Q1 * Q3) * 180 / (float)PI;
//    roll = atan2f(2 * Q2 * Q3 + 2 * Q0 * Q1, -2 * Q1 * Q1 - 2 * Q2 * Q2 + 1) * 180 / (float)PI;
//    yaw = atan2f(2 * Q1 * Q2 + 2 * Q0 * Q3, -2 * Q2 * Q2 - 2 * Q3 * Q3 + 1) * 180 / (float)PI;
//    
//}
//
//void get_gyro(void)
//{
//    imu660ra_get_gyro();
//    static int16_t imu660ra_gyro_z_last;
//    static int16_t imu660ra_gyro_z_low;
//    imu660ra_gyro_z_low = (int)(0.09*(imu660ra_gyro_z) + 0.91* (imu660ra_gyro_z_last));
//    imu660ra_gyro_z_last=imu660ra_gyro_z_low;
//    
////
////    if(imu660ra_gyro_z>2500)imu660ra_gyro_z=2500;
////    else if(imu660ra_gyro_z<-2500)imu660ra_gyro_z=-2500;
//
//    ang_z=(float)(imu660ra_gyro_transition((float)imu660ra_gyro_z_low- offset_)*(float)PI/180);
//}
//
//void ClearGyro(void)
//{
//    Q0=1;
//    Q1=0;
//    Q2=0;
//    Q3=0;
//    I_ex=0;
//    I_ey=0;
//    I_ez=0;
//}
//
//void GyroResolve(void)
//{
//    if(GyroINT)
//    {
//        get_gyro();
//
//        if(GyroOn)
//        {
//            ImuGetValues();
//            ImuAHRSupdate();
//            ReadGyro();
//        }
//        GyroINT=0;
//    }
//
//}
//****************************************************************版本2*********************************************************************************************************************************************

int32_t GyroOn=1;//是否解算
uint8 GyroINT=1;//暂时没用
float ang_z=0;//Z轴角速度
float offset_=0.0;//1m偏移值
float current_yaw;
///**************************去零漂*********************************************************************
float CALIBRATION_SAMPLES = 50 * 2;
float gyroXOffset = 0;
float gyroXOffset_rcc = 0;

float gyroYOffset = 0;
float gyroYOffset_rcc = 0;

float gyroZOffset = 0;
float gyroZOffset_rcc = 0;

void calibrateGyro() {          //陀螺仪初始化校准

    ICM_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);   //得到加速度传感器数据

    float sumX = 0,sunX_rcc = 0;
    float sumY = 0,sunY_rcc = 0;
    float sumZ = 0,sunZ_rcc = 0;
    for (int i = 0; i < CALIBRATION_SAMPLES; i++)                     //2s
    {
        ICM_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);   //得到加速度传感器数据
        sumX += imu660ra_gyro_transition(gyrox);
        sunX_rcc += imu660ra_acc_transition(aacx);

        sumY += imu660ra_gyro_transition(gyroy);
        sunY_rcc += imu660ra_acc_transition(aacy);

        sumZ += imu660ra_gyro_transition(gyroz);
        sunZ_rcc += imu660ra_acc_transition(aacz);

          delay_ms(20);
    }
    gyroXOffset = sumX / CALIBRATION_SAMPLES ;
    gyroXOffset_rcc = sunX_rcc / CALIBRATION_SAMPLES;

    gyroYOffset = sumY / CALIBRATION_SAMPLES ;
    gyroYOffset_rcc = sunY_rcc / CALIBRATION_SAMPLES;

    gyroZOffset = sumZ / CALIBRATION_SAMPLES ;
    gyroZOffset_rcc = sunZ_rcc / CALIBRATION_SAMPLES;
}

///**************************卡尔曼滤波*********************************************************************

typedef struct
{
    float LastP;//上次估算协方差
    float Now_P;//当前估算协方差
    float out;//卡尔曼滤波器输出
    float Kg;//卡尔曼增益
    float Q;//过程噪声协方差
    float R;//观测噪声协方差
}KFP;//Kalman Filter parameter

//KFP KFP_height_1={0.02,0,0,0,0.1,0.1};
#define GRAVITY 9.81  // 重力加速度 (m/s2)
KFP KFP_height_1={0.02,0,0,0,0.001,0.543};
KFP KFP_height_2={0.02,0,0,0,0.001,0.543};
KFP KFP_height_3={0.02,0,0,0,0.001,0.543};

float kalmanFilter_1(KFP *kfp,float input)           //卡尔曼滤波
{
     float really_jiaodu = input ;
     //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
     kfp->Now_P = kfp->LastP + kfp->Q;
     //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
     kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
     //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
     kfp->out = kfp->out + kfp->Kg * (really_jiaodu -kfp->out);//因为这一次的预测值就是上一次的输出值
     //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
     kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
     return kfp->out;
}

///**************************四元素获得累角*********************************************************************
//功　能: 快速计算 1/Sqrt(x)
//参  数：要计算的值
//返回值：结果
//备  注：比普通Sqrt()函数要快四倍
//*********************************************************************************************************/
static float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

//=============== 类型定义（新增部分需补充） ==============
typedef struct {
    float X;
    float Y;
    float Z;
} FLOAT_XYZ;

typedef struct {
    float yaw;   // Z轴偏航角（直接来自四元数）
    float pit;
    float rol;
} FLOAT_ANGLE;

//=============== 算法核心修改 ================

//代码中四元数初始化为 q0=1, q1=q2=q3=0（无旋转状态）

#define Kp 3.0f    // 提高收敛速度
#define Ki 0.001f  // 降低积分影响
#define halfT 0.001f//2ms
#define RadtoDeg 57.295779513f

static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;

static float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;

float lv_gyro_z;

void IMUupdate(FLOAT_XYZ *Gyr_rad, FLOAT_XYZ *Acc_filt, FLOAT_ANGLE *Att_Angle)
{
    // 输入数据预处理（兼容你的原始参数）
    float ax = Acc_filt->X;
    float ay = Acc_filt->Y;
    float az = Acc_filt->Z;
    float gx = Gyr_rad->X;  // 确保已经是弧度单位
    float gy = Gyr_rad->Y;
    float gz = Gyr_rad->Z;

    // 加速度计归一化
    float norm = invSqrt(ax*ax + ay*ay + az*az);
    ax *= norm;
    ay *= norm;
    az *= norm;

    // 四元数预测
    float vx = 2.0f*(q1*q3 - q0*q2);
    float vy = 2.0f*(q0*q1 + q2*q3);
    float vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

    // 误差补偿
    float ex = (ay*vz - az*vy);
    float ey = (az*vx - ax*vz);
    float ez = (ax*vy - ay*vx);

    exInt += ex * Ki;
    eyInt += ey * Ki;
    ezInt += ez * Ki;

    // 角速度补偿
    gx += Kp*ex + exInt;
    gy += Kp*ey + eyInt;
    gz += Kp*ez + ezInt;

    // 四元数更新
    float q0_pred = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    float q1_pred = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    float q2_pred = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    float q3_pred = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

    // 四元数归一化
    norm = invSqrt(q0_pred*q0_pred + q1_pred*q1_pred + q2_pred*q2_pred + q3_pred*q3_pred);
    q0 = q0_pred * norm;
    q1 = q1_pred * norm;
    q2 = q2_pred * norm;
    q3 = q3_pred * norm;

    // 直接计算Z轴角度（关键修改点）
    Att_Angle->yaw = atan2f(2.0f*(q1*q2 + q0*q3),
                          q0*q0 + q1*q1 - q2*q2 - q3*q3) * RadtoDeg;
}

float gyro_x_pretreatment = 0;
float gyro_y_pretreatment = 0;
float gyro_z_pretreatment = 0;

float roll,pitch;

unsigned char imu660ra_in_once_flag = 0;

float GetFusedZAngle()
{


    gyro_x_pretreatment = imu660ra_gyro_transition(gyrox)  - gyroXOffset;
    gyro_x_pretreatment = kalmanFilter_1(&KFP_height_1,imu660ra_gyro_transition(gyrox));                   //卡尔曼滤波
    gyro_x_pretreatment = (float)((int)gyro_x_pretreatment);

    gyro_y_pretreatment = imu660ra_gyro_transition(gyroy)  - gyroYOffset;
    gyro_y_pretreatment = kalmanFilter_1(&KFP_height_2,imu660ra_gyro_transition(gyroy));                   //卡尔曼滤波
    gyro_y_pretreatment = (float)((int)gyro_y_pretreatment);

    gyro_z_pretreatment = imu660ra_gyro_transition(gyroz)  - gyroZOffset;
    gyro_z_pretreatment = kalmanFilter_1(&KFP_height_3,imu660ra_gyro_transition(gyroz));                   //卡尔曼滤波
    gyro_z_pretreatment = (float)((int)gyro_z_pretreatment);

    // 数据转换（使用你的原始参数）
    FLOAT_XYZ acc = {
        .X = imu660ra_acc_transition(aacx) - gyroXOffset_rcc,
        .Y = imu660ra_acc_transition(aacy) - gyroYOffset_rcc,
        .Z = imu660ra_acc_transition(aacz) - gyroZOffset_rcc
    };

    FLOAT_XYZ gyro_rad = {
        .X = gyro_x_pretreatment * 0.0174532925f, // 转弧度
        .Y = gyro_y_pretreatment * 0.0174532925f,
        .Z = gyro_z_pretreatment * 0.0174532925f
    };

    if(imu660ra_in_once_flag == 0)
    {
       roll = atan2f(acc.Y, acc.Z);
       pitch = atan2f(-acc.X, sqrtf(acc.Y*acc.Y + acc.Z*acc.Z));
       q0 = cos((double)(roll/2)) * cos((double)(pitch/2));
       q1 = sin((double)(roll/2)) * cos((double)(pitch/2));
       q2 = cos((double)(roll/2)) * sin((double)(pitch/2));
       q3 = sin((double)(roll/2)) * sin((double)(pitch/2));
      imu660ra_in_once_flag = 1;
    }


    FLOAT_ANGLE angle;
    IMUupdate(&gyro_rad, &acc, &angle);


    lv_gyro_z = gyro_rad.Z;

    return angle.yaw; // 直接返回四元数计算的Z轴角度
}




//=================== 角度连续化处理 ====================
typedef struct {
    float total_angle;   // 累计角度（连续）
    float prev_raw;      // 前一次原始角度
    uint8_t is_first;    // 首次调用标志
} AngleUnwrapper;

float UnwrapAngle(AngleUnwrapper* handler, float current_angle)
{
    if(handler->is_first)
    {
        handler->total_angle = current_angle;
        handler->prev_raw = current_angle;
        handler->is_first = 0;
        return handler->total_angle;
    }

    // 计算原始角度变化量
    float delta = current_angle - handler->prev_raw;

    // 角度跳变补偿（核心算法）
    if(delta > 180.0f) {
        delta -= 360.0f;
    } else if (delta < -180.0f) {
        delta += 360.0f;
    }

    // 累加有效变化量
    handler->total_angle += delta;
    handler->prev_raw = current_angle;

    return handler->total_angle;
}



AngleUnwrapper z_angle_handler = {.is_first=1};

float GetContinuousZAngle(void)
{
    // 1. 获取原始Z轴角度（来自四元数）
    float raw_angle = GetFusedZAngle();

    // 2. 进行角度连续化处理
    return UnwrapAngle(&z_angle_handler, raw_angle);
}

static float yawSoftwareOffset = 0.0f;

// 清零函数
void ResetYawZero(void) {
    yawSoftwareOffset = GetContinuousZAngle();  // 获取当前累积角度
}

// 带偏移的角度获取
float GetCurrentYaw(void) {
    return GetContinuousZAngle() - yawSoftwareOffset;
}




//void get_gyro(void)
//{
//    imu660ra_get_gyro();
//    static int16_t imu660ra_gyro_z_last;
//    static int16_t imu660ra_gyro_z_low;
//    imu660ra_gyro_z_low = (int)(0.09*(imu660ra_gyro_z) + 0.91* (imu660ra_gyro_z_last));
//    imu660ra_gyro_z_last=imu660ra_gyro_z_low;
//    
////
////    if(imu660ra_gyro_z>2500)imu660ra_gyro_z=2500;
////    else if(imu660ra_gyro_z<-2500)imu660ra_gyro_z=-2500;
//
//    ang_z=(float)(imu660ra_gyro_transition((float)imu660ra_gyro_z_low- offset_)*(float)PI/180);
//}

void GyroResolve(void)
{
    
       

        if(GyroOn)
        {
            current_yaw = GetCurrentYaw();
        }
  
    

}



