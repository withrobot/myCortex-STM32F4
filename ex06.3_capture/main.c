
/*
  ex06.3_capture

  timer 예제
  SW1이 눌려진 시간 간격을 us 단위로 출력한다.
  capture 예제에서 사용한 RC 서보 제어 신호를 해석할 때 사용할 수 있다.
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"


#define TIMER_PRESCALER_FREQ        1000000     // timer 입력 클력 1MHz
#define TIMER_PERIOD                0xffff      // timer base의 auto-reload


static int capture_prev;
static int diff;
static int timer_update_cnt;
static unsigned char capture_flag;

int main()
{
    uint16_t PrescalerValue;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    LED_Init();
    LED_R_ON();
    LED_G_ON();

    LLIO_Init(115200);



    /* TIM2, GPIOA clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);


    /* Enable the TIM2 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    // PA0 핀을 TIM2_CH1 input으로 설정
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /* Compute the prescaler value */
    SystemCoreClockUpdate();
    PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ) - 1;           // timer base counter에 1MHz 입력

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = TIMER_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0;

    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);

    TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE);



    printf("\r\nTimer Capture example\r\n");
    while(1)
    {
        __WFI();

        if (capture_flag)
        {
            capture_flag = 0;

            printf("time diff = %d[us]\r\n", diff);

            LED_G_TOGGLE();
        }

    }
}


void TIM2_IRQHandler(void)
{
    int capture;

    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        timer_update_cnt++;
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)
    {
        capture = TIM_GetCapture1(TIM2);

        diff = timer_update_cnt * TIMER_PERIOD + capture - capture_prev;

        timer_update_cnt = 0;
        capture_flag = 1;
        capture_prev = capture;
    }
}



