#include "steer.h"
#include "tim.h"
#include "motor.h"

#define ReactSpeed 75 //smaller and faster

/*
    Front wheel steering gear, 180 into 10 parts, input 1-11
*/
void Direction_Set(uint16_t angle_level)
{
   __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, angle_level+2);
}

extern uint16_t Motor_Forward, Motor_Retreat,\
    Steer_Left, Steer_Right, Direction_Angle;

void Direction_Control()
{
    if(Steer_Left == 1){
        Direction_Angle--;HAL_Delay(ReactSpeed);
    }
    else if(Steer_Right == 1){
        Direction_Angle++;HAL_Delay(ReactSpeed);
    }
//    else if((Steer_Left == 0)&&(Steer_Right == 0)){
//        Direction_Angle = 6;
//    }
    if(Direction_Angle > 11)Direction_Angle = 11;
    if(Direction_Angle < 1)Direction_Angle = 1;
//    if((Steer_Left == 0)&&(Steer_Right == 0)){
//        Direction_Angle = 6;
//    }
    Direction_Set(Direction_Angle);
}

void Direction_Reset()
{
    Direction_Angle = 6;
    Direction_Set(Direction_Angle);
}

extern uint16_t FPV_Yaw_Left, FPV_Yaw_Right, FPV_Yaw_Angle;
void FPV_Yaw_Control()
{
    if(FPV_Yaw_Left == 1){
        FPV_Yaw_Angle++;HAL_Delay(ReactSpeed);
    }
    else if(FPV_Yaw_Right == 1){
        FPV_Yaw_Angle--;HAL_Delay(ReactSpeed);
    }
    
    if(FPV_Yaw_Angle > 13)FPV_Yaw_Angle = 13;
    if(FPV_Yaw_Angle < 3)FPV_Yaw_Angle = 3;
    
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, FPV_Yaw_Angle);
}

extern uint16_t FPV_Pitch_Left, FPV_Pitch_Right, FPV_Pitch_Angle;
void FPV_Pitch_Control()
{
    if(FPV_Pitch_Left == 1){
        FPV_Pitch_Angle++;HAL_Delay(ReactSpeed);
    }
    else if(FPV_Pitch_Right == 1){
        FPV_Pitch_Angle--;HAL_Delay(ReactSpeed);
    }
    
    if(FPV_Pitch_Angle > 13)FPV_Pitch_Angle = 13;
    if(FPV_Pitch_Angle < 3)FPV_Pitch_Angle = 3;
    
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, FPV_Pitch_Angle);
}

void Car_Reset()
{
    Zx_MotorRunN(1,0);
    Zx_MotorRunS(2,0);  //turn off Motor
    
    Steer_Left = 0, Steer_Right = 0;
    Motor_Forward = 0, Motor_Retreat = 0;
    Direction_Angle = 6;
    Direction_Set(Direction_Angle);
    
    FPV_Yaw_Left = 0, FPV_Yaw_Right = 0;
    FPV_Pitch_Left = 0, FPV_Pitch_Right = 0;
    FPV_Yaw_Angle = 8;
    FPV_Yaw_Control();
    FPV_Pitch_Angle = 8;
    FPV_Pitch_Control();
    
}
