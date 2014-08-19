#include "stm32f4xx.h"
#include "led.h"


void LED_Init(void)
{
    LED_R_INIT();
    LED_G_INIT();
    LED_R_OFF();
    LED_G_OFF();
}


