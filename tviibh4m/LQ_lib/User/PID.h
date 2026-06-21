#ifndef __PID_H_
#define __PID_H_

#include "include.h"

typedef struct PID
{
    long SumError;  //误差累计
    int32 LastError;    //Error[-1]
    int32 PrevError;    //Error[-2]
    int32 LastSpeed;    //Speed[-1]
    float KP;                              //比例常数 Proportional Const
    float KI;                                //积分常数 Integral Const
    float KD;                              //微分常数 Derivative Const
} PID;

typedef struct{
  float Kp;
  float Ki;
  float Kd;
  float Kp2;
  float Kd2;
  float p_out;
  float i_out;
  float d_out;
  float d2_out;
  float Err;
  float Last_Err;       // ?′???
  float Previous_Err;   // ??′???
  float Output;
  float Target;
  float Measure;
    
}RS_Incremental_PID;

float Position_PID (RS_Incremental_PID *pid , int16 error , int16 gyro_z);
float IncrementalPID_L(RS_Incremental_PID *pid ,int16 target_speed, int16 measured_value );
float IncrementalPID_R(RS_Incremental_PID *pid ,int16 target_speed, int16 measured_value );
void PlacePID_Init(PID *sptr);
void IncPID_Init(PID *sptr, float *MOTOR_PID);
void PID_Init(void);
float PID_Realize(PID *sptr, float ActualSpeed, float SetSpeed);
float PID_Realize_Place(PID *sptr, float ActualSpeed, float SetSpeed);
extern RS_Incremental_PID turn_pid;
extern RS_Incremental_PID pid_L;
extern RS_Incremental_PID pid_R;
extern PID L_MOTOR_PID,R_MOTOR_PID,Speed_PID;
extern float Left_MOTOR[3],Right_MOTOR[3],Speed_MOTOR[3];
extern float speedp,speedi;
extern int Out_dn,Out_dw ;
extern  int time;
extern float dir_p,dir_d,dirn_p,dirn_d;
int dircontrol(int chazhi);//外环
int diranglecontrol(int out_dw);//内环
#endif