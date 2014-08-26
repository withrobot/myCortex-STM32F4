
/*
  ex07.2_ADC_int

  ADC peripheral 동작 확인.
  가변 저항의 값을 읽어 전압과 저항값을 출력한다.
  ADC 인터럽트와 타이머를 이용한 트리거 사용.
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"

#define TIMER_PRESCALER_FREQ        1000000
#define TIMER_FREQ                  100            // 100Hz 주기로 트리거 발생



int main()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    uint16_t PrescalerValue;




    LED_Init();
    LED_R_ON();

    LLIO_Init(115200);


    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);


    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    /* ADC Common Init */
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;     // ADC clock = PCLK2/prescaler. ADC freq = typical 30MHz, Max 36MHz. PCLK2 = 168/2=84MHz.
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);



    /* ADC channel 10 (PC0) configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    /* ADC channel configuration */
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;  // TIM3의 트리거 발생시 ADC 수행
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);


    /* ADC1 regular channel10 configuration */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);

    // Enable ADC End Of Conversion interrupt
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);





    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Compute the prescaler value */
    SystemCoreClockUpdate();
    PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ) - 1;           // timer base counter에 1MHz 입력

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = TIMER_PRESCALER_FREQ / TIMER_FREQ - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // TIM3 update 이벤트에서 트리거 발생하도록 설정
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);






    printf("\r\nADC Test\r\n");


    while(1)
    {
        __WFI();
    }

}


void ADC_IRQHandler(void)
{
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET)
    {
        LED_G_OFF();
        printf("%d\r\n", ADC_GetConversionValue(ADC1));     // ADC 결과 읽어 출력
    }
}




/*
    TIM3 인터럽트는 ADC 시작 시점을 확인하기 위해 사용한다.
    이 인터럽트를 사용하지 않더라도 ADC는 정상적으로 수행된다.

    TIM3 인터럽트에서 ADC가 시작되고 동시에 LED_G가 켜진다.
    ADC가 종료되면 ADC 인터럽트가 발생되고, ADC 인터럽트 핸들러에서 LED_G를 끈다.
    오실로스코프를 이용해 LED_G를 관찰하면 ADC 수행에 약 600ns 정도 시간이 걸리는 것을 확인할 수 있다.
    ADC 수행 시간은 ADC Common의 prescaler와 ADC_RegularChannelConfig()를 이용해 설정 가능하다.
*/
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        LED_G_ON();
    }
}



