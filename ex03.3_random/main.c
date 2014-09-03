
/*
  ex03.3_random

  난수 생성 예제
  random number generator를 사용해 난수를 발생시키고 이를 출력
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "llio.h"

int main()
{
    unsigned long r;


    LED_Init();
    LED_R_ON();

    LLIO_Init(115200);
    printf("\r\nRandom Number Generator test\r\n");


    // 난수 생성기 초기화
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

    RNG_Cmd(ENABLE);

    while(1)
    {
        delay_ms(500);         // 0.5초 딜레이

        // 새로운 난수가 만들어지지 않았다면 생성될 때 까지 대기
        while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET);

        // 발생된 난수를 출력
        r = RNG_GetRandomNumber();
        printf("%u\r\n", r);

        LED_G_TOGGLE();
    }
}



