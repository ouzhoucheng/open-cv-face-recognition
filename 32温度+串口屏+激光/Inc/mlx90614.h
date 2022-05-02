/*******************************************************************************
* 文件名		: mlx90614.h
* 作  者	: 
* 版  本	: 
* 日  期	: 2013-08-07
* 描  述	: mlx90614函数
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MLX90614_H
#define __MLX90614_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
//#include "sys.h" 
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//#define SDA_Pin GPIO_PIN_7
//#define SDA_GPIO_Port GPIOG
//#define SCL_Pin GPIO_PIN_6
//#define SCL_GPIO_Port GPIOG

#define ACK	 0 //应答
#define	NACK 1 //无应答
#define SA				0x00 //Slave address 单个MLX90614时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS		0x00 //RAM access command RAM存取命令
#define EEPROM_ACCESS	0x20 //EEPROM access command EEPROM存取命令
#define RAM_TA      	0x06
#define RAM_TOBJ1		0x07 //To1 address in the eeprom 目标1温度,检测到的红外温度 -70.01 ~ 382.19度
#define RAM_TOBJ2		0x08

#define SMBUS_SCK_PORT	GPIOG      //PG端口(端口和下面的两个针脚可自定义)
#define SMBUS_SDA_PORT	GPIOG      //PG端口(端口和下面的两个针脚可自定义)
#define SMBUS_SCK		GPIO_PIN_6 //PG6：SCL
#define SMBUS_SDA		GPIO_PIN_7 //PG7：SDA

void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(uint8_t);
uint8_t SMBus_SendByte(uint8_t);
uint8_t SMBus_ReceiveBit(void);
uint8_t SMBus_ReceiveByte(uint8_t);
void SMBus_Delay(uint16_t);
void SMBus_Init(void);
uint16_t SMBus_ReadMemory(uint8_t, uint8_t);
void SMBus_WriteMemory(uint8_t slaveAddress, uint8_t command, uint16_t Emissivity);
uint8_t PEC_Calculation(uint8_t*);
float SMBus_ReadTemp(void); //获取温度值
float SMBus_ReadTemp2(void);
float SMBus_ReadTempA(void);
void Parameter_Init(int ReflectRate);	//参数初始化

void Real_Kalmen();
void T_Kalmen(char T_Choose);

void T_Advance(char T_Choose);

#endif

/*********************************END OF FILE*********************************/
