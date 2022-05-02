#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx_hal.h"

#define Zx_Motor_FRAME_HEADER1  0XFA
#define Zx_Motor_FRAME_HEADER2  0XAF
#define Zx_Motor_MOVE_ANGLE     0X01
#define Zx_Motor_LED            0X04
#define Zx_Motor_READ_ANGLE     0X02
#define Zx_Motor_ID_WRITE       0XCD
#define Zx_Motor_SET_OFFSET     0XD2
#define Zx_Motor_READ_OFFSET    0XD4
#define Zx_Motor_VERSION        0X01
#define Zx_Motor_FRAME_END      0XED
#define Zx_Motor_RUNS           0XFD
#define Zx_Motor_RUNN           0XFE

void Zx_MotorRunS(uint8_t id,uint32_t rpm);
void Zx_MotorRunN(uint8_t id,uint32_t rpm);
void Zx_MotorLED(uint8_t id, uint8_t LED_State);
void Run_Control();

#endif  
