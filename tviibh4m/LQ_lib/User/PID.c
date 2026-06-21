#include "include.h"

PID L_MOTOR_PID,R_MOTOR_PID,Speed_PID;
float speedp=0,speedi=0;
int Out_dn,Out_dw ;
int time=0 ;
float Left_MOTOR[3] =  {1.3,1,0};      //KP KI KD
float Right_MOTOR[3] = {0.4,0.0175,0};    //KP KI KD
float Speed_MOTOR[3] = {0.4,0.0175,0};      //KP KI KD0.02 0.01850{0.01,0.0175,0};0.07   0.35{0.32,0.015,0}0.4,0.0175,0};  
RS_Incremental_PID turn_pid;
RS_Incremental_PID pid_L;
RS_Incremental_PID pid_R;
//位置式PID参数初始化
void PlacePID_Init(PID *sptr)
{
    sptr->SumError = 0;
    sptr->LastError = 0;    //Error[-1]
    sptr->PrevError = 0;    //Error[-2]
}
void PID_Init(void)
{
       // 左轮速度环PID参数（增
    pid_L.Kp =0.20;// 比例系 
    pid_L.Ki =0.049;   // 积分系数 
    pid_L.Kd = 0;    // 微分系数
    pid_L.Target =0;//速度（单位：编码器脉冲/控制周期）

    // 右轮速度环PID参数（增量式）
    pid_R.Kp =0.20 ;  // 比例系数 
    pid_R.Ki = 0.049;   // 积分系数 
    pid_R.Kd = 0;   // 微分系数
    pid_R.Target =0; //速度

    // 转向环参数（位置式）
    turn_pid.Kp = 0;  // 误差比例系数（原注释参数65）7.1    160
    turn_pid.Kp2 = 0;// 误差二次项系数（原注释参数2）0.01
    turn_pid.Kd = 0;  // 误差微分系数（原注释参数60）20.0
    turn_pid.Kd2 = 0;//陀螺仪阻尼系数（原注释参数0.5）0.01
    
    
     dir_p=speedp;
     dir_d=speedi;
     dirn_p=0.1;
     dirn_d=0.06;
}

//增量式PID参数初始化
void IncPID_Init(PID *sptr, float *MOTOR_PID)
{
    sptr->SumError = 0;
    sptr->LastError = 0;    //Error[-1]
    sptr->PrevError = 0;    //Error[-2]
    sptr->LastSpeed = 0;
        sptr->KP = *MOTOR_PID;
        sptr->KI = *(MOTOR_PID+1);
        sptr->KD = *(MOTOR_PID+2);

}


float IncrementalPID_L (RS_Incremental_PID *pid ,int16 target_speed, int16 measured_value )
{
  pid->Err = target_speed - measured_value ;
  
  pid->Err =(int)( 0.8f*pid->Err + 0.2f*pid->Last_Err);
  
  pid->p_out = pid->Kp * (pid->Err - pid->Last_Err);
  pid->i_out = pid->Ki * pid->Err;
  pid->d_out = pid->Kd * (pid->Err - 2*pid->Last_Err + pid->Previous_Err);
  
  pid->Output += pid->p_out + pid->i_out + pid->d_out; 
  
  pid->Previous_Err = pid->Last_Err;
  pid->Last_Err =  pid->Err;
  
  return pid->Output;
}

float IncrementalPID_R (RS_Incremental_PID *pid ,int16 target_speed, int16 measured_value )
{
  pid->Err = target_speed - measured_value ;
  
  pid->Err =(int)( 0.8f*pid->Err + 0.2f*pid->Last_Err);
  
  pid->p_out = pid->Kp * (pid->Err - pid->Last_Err);
  pid->i_out = pid->Ki * pid->Err;
  pid->d_out = pid->Kd * (pid->Err - 2*pid->Last_Err + pid->Previous_Err);
  
  pid->Output += pid->p_out + pid->i_out + pid->d_out; 
  
  pid->Previous_Err = pid->Last_Err;
  pid->Last_Err =  pid->Err;
  
  return pid->Output;
}

//增量式PID电机控制
float PID_Realize(PID *sptr, float ActualSpeed, float SetSpeed)
{
    //当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
//  register int32 iError,      //当前误差
//                  Increase;   //最后得出的实际增量

    float iError,       //当前误差
          Increase;   //最后得出的实际增量
    
    iError = SetSpeed - ActualSpeed;//计算当前误差

    Increase = sptr->KP * (iError - sptr->LastError)
             + sptr->KI * iError
             + sptr->KD * (iError - 2 * sptr->LastError + sptr->PrevError);
    sptr->PrevError = sptr->LastError;  //更新前次误差
    sptr->LastError = (int32_t)roundf(iError);           //更新上次误差
    
    return Increase;
}

float PID_Realize_Place(PID *sptr, float ActualSpeed, float SetSpeed)
{
    //当前误差，定义为寄存器变量，只能用于整型和字符型变量，提高运算速度
//  register int32 iError,      //当前误差
//                  Increase;   //最后得出的实际增量

    float iError,       //当前误差
          OUT;   //最后得出的实际增量
    
    sptr->LastError = (int32_t)roundf(iError);           //更新上次误差
    iError = SetSpeed - ActualSpeed;//计算当前误差
    
    sptr->SumError+=iError;
    if (sptr->SumError > 1000) {sptr->SumError = 1000;}
    if (sptr->SumError < -1000) {sptr->SumError = -1000;}
    
    OUT = (sptr->KP * iError) + (sptr->KI * sptr->SumError) + (sptr->KD * (iError - sptr->LastError));
    return OUT;
}
float dir_p,dir_d,dirn_p,dirn_d;
int dircontrol(int chazhi)//外环
{
	int out_dw;
	static int last_chazhi;
	out_dw=(int)(chazhi*dir_p+(chazhi-last_chazhi)*dir_d);
	last_chazhi=chazhi;
    return out_dw;
}
int diranglecontrol(int out_dw)//内环
{
	int out_dn,error;
    float gyroxx;
	static int last_error;
    gyroxx=gyrox/32.8f;
	error=(int)(out_dw-gyroxx);//内环的偏差是外环输出与陀螺仪z轴的角速度之差
	out_dn=(int)(error*dirn_p+(error-last_error)*dirn_d);
	last_error=error;
	return out_dn;
}