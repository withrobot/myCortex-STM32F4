
/*
 * ex02.2_SW
 * GPIO 입력을 이용해 스위치입력을 받아 LED를 제어하는 예제
 */

#include "stm32f4xx.h"
#include "led.h"
#include "switch.h"

int main()
{
    LED_Init();

    SW_INIT();

    while(1)
    {
        // SW1(PE0)이 눌려져 있는 동안 붉은색 LED를 켠다.
        if (SW_1_IS_PUSHED())
            LED_R_ON();
        else
            LED_R_OFF();

        // SW2(PE1)이 눌려져 있는 동안 녹색 LED를 켠다.
        if (SW_2_IS_PUSHED())
            LED_G_ON();
        else
            LED_G_OFF();
    }

}

