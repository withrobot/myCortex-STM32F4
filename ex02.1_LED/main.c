
/*
 * ex02.1_LED
 * Red/Green LED를 초기화하고 제어하는 예제
 */

#include "stm32f4xx.h"
#include "led.h"

int main()
{
    LED_Init();

    LED_R_ON();
    LED_G_ON();

}

