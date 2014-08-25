
/*
  ex06.1_repeat_timer

  timer 예제
  50ms 주기 반복 타이머 설정.
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"


#define TIMER_PRESCALER_FREQ        1000000     // timer 입력 클력 1MHz
#define TIMER_FREQ                  20          // timer 반복 주기 50ms = 20Hz

static volatile unsigned long tick_count;

int main()
{
    uint16_t PrescalerValue;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    LED_Init();
    LED_R_ON();
    LED_G_ON();

    LLIO_Init(115200);


    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* Compute the prescaler value */
    // timer base counter에 1MHz 입력.
    // TIMER3는 APB1에 연결되어 있다.
    // TIMER input clock은 APBx의 clock divider가 1인 경우 HCLK의 클럭이 그대로 쓰이고,
    //                                            1이 아닌 경우 HCLK의 클럭 /2가 사용된다.
    // 현재 소스 구성에서는 drv/system_stm32f4xx.c 파일의 SetSysClock()함수 내에서 APB1 clock은 HCLK /4로 설정하고 있다.
    SystemCoreClockUpdate();
    PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ) - 1;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = TIMER_PRESCALER_FREQ / TIMER_FREQ - 1;           // 20Hz timer
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* TIM IT enable */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);




    while(1)
    {
        __WFI();

        printf("System on time = %.2f[s]\r\n", (float)tick_count / (float)TIMER_FREQ);
    }
}


void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

        LED_G_TOGGLE();

        tick_count++;
    }
}


