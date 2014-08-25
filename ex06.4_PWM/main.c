
/*
  ex06.4_PWM

  timer 예제
  PWM 출력
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"


#define TIMER_PRESCALER_FREQ        1000000             // timer 입력 클력 1MHz
#define PWM_FREQ                    1000                // PWM 반복 주기 1ms = 1kHz
#define PWM_WIDTH                   (TIMER_PRESCALER_FREQ / PWM_FREQ)

#define DUTY_IDLE                   (PWM_WIDTH / 2)     // 50% duty
#define DUTY_MAX                    (PWM_WIDTH - 1)     // 최대 duty 값.



int main()
{
    uint16_t PrescalerValue;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned short duty;
    char buff[128];


    LED_Init();
    LED_R_ON();
    LED_G_ON();

    LLIO_Init(115200);



    /* TIM3, GPIOB clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    /* Compute the prescaler value */
    SystemCoreClockUpdate();
    PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ) - 1;   // timer base counter에 1MHz 입력

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = TIMER_PRESCALER_FREQ / PWM_FREQ - 1;     // 1kHz timer
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = DUTY_IDLE;       // default 50%
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);



    printf("\r\nTimer PWM example\r\n");

    printf("Enter duty ratio[0..%d]\r\n", DUTY_MAX);

    while(1)
    {
        if (scanf("%d", (int*)&duty) == 1)
        {
            if (duty > DUTY_MAX)
                duty = DUTY_MAX;

            TIM_SetCompare4(TIM3, (uint16_t)duty);

            printf("PWM duty = %.1f%\r\n\r\n", duty / 10.);
            printf("Enter duty ratio[0..%d]\r\n", DUTY_MAX);
            LED_G_TOGGLE();
        }
        else
        {
            scanf("%s", buff);      // flush read FIFO
        }
    }
}




