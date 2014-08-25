
/*
  ex05.1_systick

  SysTick timer 예제
  타이머 설정과 인터럽트 예시
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"

static volatile unsigned long tick_count;

int main()
{
    LED_Init();
    LED_R_ON();
    LED_G_ON();

    LLIO_Init(115200);


    //
    // SysTick Timer 설정
    //
    // SysTick timer는 24bit counter register를 가지고 있다.
    // 2^24 = 16777216(16M)
    // system core clock을 1:1로 count하므로 최대 주기는 168M/16M = 10.5Hz = 95ms
    // 이보다 더 느린 주기로 SysTick 타이머를 설정하면 SysTick_Config()이 에러 리턴한다.
    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / 20))   // 20Hz tick interrupt
    {
        assert_param(0);
    }



    while(1)
    {
        __WFI();

        printf("System on time = %.2f[s]\r\n", tick_count / 20.);
    }
}


void SysTick_Handler(void)
{
    LED_G_TOGGLE();

    tick_count++;
}


