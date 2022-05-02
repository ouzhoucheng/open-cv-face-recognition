#include "motor.h"
#include "usart.h"
#include <stdio.h>

#define GET_LOW_BYTE(A)  ((uint8_t)(A))
#define GET_HIGH_BYTE(A) ((uint8_t)((A) >> 8))

uint8_t Zx_MotorCheckSum(uint8_t buf[])
{
	uint8_t i;
	uint32_t sum=0;
	for(i=2;i<8;i++)
	{
		sum +=buf[i];
	}
	if(sum>255)sum&=0x00FF;
  return sum;
}

/*
  电机正转  其中id为电机id号 ，rpm为给定电机转数
*/
void Zx_MotorRunS(uint8_t id,uint32_t rpm)
{
	uint8_t  buf[10];
	buf[0]=Zx_Motor_FRAME_HEADER1;
	buf[1]=Zx_Motor_FRAME_HEADER2;
	buf[2]=id;
	buf[3]=0x01  ;
	buf[4]=Zx_Motor_RUNS;
	buf[5]=0x00;
	buf[6]=GET_HIGH_BYTE(rpm);
	buf[7]=GET_LOW_BYTE(rpm);
	buf[8]=Zx_MotorCheckSum(buf);
	buf[9]= Zx_Motor_FRAME_END;
    
    HAL_UART_Transmit(&huart3, buf, 10, 20);
}
/*
  电机反转  其中id为电机id号 ，rpm为给定电机转数
*/
void Zx_MotorRunN(uint8_t id,uint32_t rpm)
{
	uint8_t  buf[10];
	buf[0]=Zx_Motor_FRAME_HEADER1;
	buf[1]=Zx_Motor_FRAME_HEADER2;
	buf[2]=id;
	buf[3]=0x01  ;
	buf[4]=Zx_Motor_RUNN;
	buf[5]=0x00;
	buf[6]=GET_HIGH_BYTE(rpm);
	buf[7]=GET_LOW_BYTE(rpm);
	buf[8]=Zx_MotorCheckSum(buf);
	buf[9]= Zx_Motor_FRAME_END;
	
    HAL_UART_Transmit(&huart3, buf, 10, 20);
}

/*
  电机反转  其中id为电机id号 ，rpm为给定电机转数
*/
void Zx_MotorLED(uint8_t id, uint8_t LED_State)
{
	uint8_t  buf[10];
	buf[0]=Zx_Motor_FRAME_HEADER1;
	buf[1]=Zx_Motor_FRAME_HEADER2;
	buf[2]=id;
	buf[3]=0x04  ;
	buf[4]=LED_State;
	buf[5]=0x00;
	buf[6]=0x00;
	buf[7]=0x00;
	buf[8]=Zx_MotorCheckSum(buf);
	buf[9]= Zx_Motor_FRAME_END;
	
    HAL_UART_Transmit(&huart3, buf, 10, 20);
}

extern uint16_t Motor_Forward, Motor_Retreat;
void Run_Control()
{
    if(Motor_Forward==1){
        Zx_MotorRunN(1,500);
        Zx_MotorRunS(2,500);
    }
    if(Motor_Retreat==1){
        Zx_MotorRunN(2,500);
        Zx_MotorRunS(1,500);
    }
    if((Motor_Forward==0)&&(Motor_Retreat==0)){
        Zx_MotorRunN(2,0);
        Zx_MotorRunS(1,0);
    }
}
