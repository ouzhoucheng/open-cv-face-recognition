#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx_hal.h"



void Direction_Set(uint16_t angle_level);
void Direction_Control();
void Direction_Reset();
void FPV_Yaw_Control();
void FPV_Pitch_Control();


#endif
