
/*
  ex03.2_delay

  delay 구현 예제
  타이머를 사용하지 않으면서 정확한 시간의 delay를 구현.
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"


int main()
{
    int i = 0;


    LED_Init();
    LED_R_ON();
    LED_G_ON();



    while(1)
    {
        delay_us(1000);         // 1000us = 1ms.

        LED_G_TOGGLE();         // 1kHz

        if (++i == 1000)
        {
            LED_R_TOGGLE();     // 1Hz
            i = 0;
        }
    }
}



