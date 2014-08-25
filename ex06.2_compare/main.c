
/*
  ex06.2_compare

  timer 예제
  RC 서보 제어를 위한 PWM 출력
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"


#define TIMER_PRESCALER_FREQ        1000000     // timer 입력 클력 1MHz
#define TIMER_FREQ                  50          // timer 반복 주기 20ms = 50Hz


#define DUTY_IDLE                   1500        // 0도 위치에서의 PWM high 시간(us)
#define DUTY_MAX                    2300        // 90도 위치에서의 PWM high 시간(us)
#define DUTY_STROKE                 (DUTY_MAX - DUTY_IDLE)
#define ANGLE_MAX                   90          // 최대 각도(deg). 가동 범위 = -90~90도



int main()
{
    uint16_t PrescalerValue;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    int angle, val;
    char buff[128];


    LED_Init();
    LED_R_ON();
    LED_G_ON();

    LLIO_Init(115200);



    /* TIM3, GPIOB clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // PB1 핀을 TIM3_CH4 output으로 설정
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    /* Compute the prescaler value */
    // timer base counter에 1MHz 입력.
    // TIMER3는 APB1에 연결되어 있다.
    // TIMER input clock은 APBx의 clock divider가 1인 경우 HCLK의 클럭이 그대로 쓰이고,
    //                                            1이 아닌 경우 HCLK의 클럭 /2가 사용된다.
    // 현재 소스 구성에서는 drv/system_stm32f4xx.c 파일의 SetSysClock()함수 내에서 APB1 clock은 HCLK /4로 설정하고 있다.
    SystemCoreClockUpdate();
    PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ) - 1;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = TIMER_PRESCALER_FREQ / TIMER_FREQ - 1;           // 50Hz timer
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = DUTY_IDLE;       // default 1.5ms
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);



    printf("\r\nTimer Compare example\r\n");
    printf("Enter target angle in degree[-90 ~ 90]\r\n");
    while(1)
    {
        if (scanf("%d", &angle) == 1)
        {
            val = (angle * DUTY_STROKE / ANGLE_MAX) + DUTY_IDLE;

            if (val < (DUTY_IDLE - DUTY_STROKE))
                val = DUTY_IDLE - DUTY_STROKE;

            if (val > (DUTY_IDLE + DUTY_STROKE))
                val = DUTY_IDLE + DUTY_STROKE;

            TIM_SetCompare4(TIM3, (uint16_t)val);

            printf("PWM output high time = %d[us]\r\n", val);
            printf("Enter target angle in degree[-90 ~ 90]\r\n");
            LED_G_TOGGLE();
        }
        else
        {
            scanf("%s", buff);      // flush read FIFO
        }
    }
}




