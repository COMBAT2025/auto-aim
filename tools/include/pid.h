#ifndef __PID_H
#define __PID_H


typedef struct
{
	float p;
	float i;
	float d;
	float CurrentError;
	float LastError;
	float ErrorIgnored;
	float Pout;
	float Iout;
	float Dout;
	float PIDout;
	float PIDOutCompensate;
	float PMax;	
	float IMax;
	float DMax;
	float PIDOutMax;
	float PIDOutLast;
	float Target_Speed_Last;
	float Speed_Ratio;
	float Acceleration;
	unsigned char Motor_Type;
}PID_TypeDef;
extern PID_TypeDef PID_Chassis[4];
extern PID_TypeDef PID_Gimbal_Angle[5];
extern PID_TypeDef PID_Gimbal_Speed[5];
extern PID_TypeDef PID_Chassis_Omega;
extern PID_TypeDef PID_Shoot[2];
extern PID_TypeDef PID_Trigger;
//舵PID
extern PID_TypeDef PID_Rudder_Angle[4];
extern PID_TypeDef PID_Rudder_Speed[4];

//拨弹盘单发角度环
extern PID_TypeDef PID_Trigger_Angle;
extern PID_TypeDef PID_Trigger_Speed;
void Pid_Set(PID_TypeDef *PID,float Kp,float Ki,float Kd,float Pmax,float Imax,float Dmax,float PIDmax,float Speed_Ratio,float Acceletation,float Error_Ignored,unsigned char Motor_Type);
float Pid_Calc(PID_TypeDef *PID,float Current_Speed,float Target_Current);
void Pid_Reset(PID_TypeDef *PID);

#endif