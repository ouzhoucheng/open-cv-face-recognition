#ifndef __RGB_LED_H
#define __RGB_LED_H

#include "stm32f4xx_hal.h"
#include "main.h"

void RGB_LED_Reset(void);
void RGB_LED_Blue(void);
void RGB_LED_Green(void);
void RGB_LED_Red(void);
void RGB(void);


#endif  // __RGB_LED_H
