#include "rgb_led.h"
#include "main.h"

void RGB_LED_Write0(void)
{
	HAL_GPIO_WritePin(RGB_GPIO_Port, RGB_Pin, GPIO_PIN_SET);
	__nop();__nop();__nop();__nop();__nop();__nop();
    //f4
//    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    //f4
	HAL_GPIO_WritePin(RGB_GPIO_Port, RGB_Pin, GPIO_PIN_RESET);
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
    //f4
//    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//    __nop();__nop();__nop();//f4
}

void RGB_LED_Write1(void)
{
	HAL_GPIO_WritePin(RGB_GPIO_Port, RGB_Pin, GPIO_PIN_SET);
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
    //f4
//    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    //f4
	HAL_GPIO_WritePin(RGB_GPIO_Port, RGB_Pin, GPIO_PIN_RESET);
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
    //f4
//    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();
    //f4
}

void RGB_LED_Reset(void)
{
	HAL_GPIO_WritePin(RGB_GPIO_Port, RGB_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
}

void RGB_LED_Write_Byte(uint8_t byte)
{
	uint8_t i;

	for(i=0;i<8;i++)
    {
        if(byte&0x80)
        {
            RGB_LED_Write1();
        }
        else
        {
            RGB_LED_Write0();
        }
        byte <<= 1;
	}
}

void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue)
{
	RGB_LED_Write_Byte(green);
	RGB_LED_Write_Byte(red);
	RGB_LED_Write_Byte(blue);
}

void RGB_LED_Red(void)
{
	 uint8_t i;
	//4?LED???
	for(i=0;i<4;i++)
		{
			RGB_LED_Write_24Bits(0, 0xff, 0);
	}
}

void RGB_LED_Green(void)
{
	uint8_t i;

	for(i=0;i<4;i++)
		{
			RGB_LED_Write_24Bits(0xff, 0, 0);
	}
}

void RGB_LED_Blue(void)
{
	uint8_t i;

	for(i=0;i<4;i++)
		{
			RGB_LED_Write_24Bits(0, 0, 0xff);
	}
}

void RGB(void)
{
//    RGB_LED_Red();
//    RGB_LED_Green();
//    RGB_LED_Blue();
//    RGB_LED_Red();
//    RGB_LED_Green();
//    RGB_LED_Blue();
//    RGB_LED_Red();
//    RGB_LED_Green();
//    RGB_LED_Blue();
    
    uint8_t i;
    for(i=0;i<36;i++)
    {
        RGB_LED_Write_24Bits(0xff, 0xff, 0xff);
	}
}
