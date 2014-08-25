
/*
  ex06.5_PWM_dimming

  ADC를 통해 가변저항의 값을 읽고 이를 PWM으로 출력하여 LED 밝기를 제어하는 예제.
  ADC->TIM 간의 데이터 흐름은 DMA를 사용한다.
  PWM을 생성하는 타이머의 update 이벤트를 ADC 트리거로 사용한다.
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"


// ADC 트리거용 타이머와 PWM 생성용 타이머 모두 TIM3를 사용한다.
// 따라서 ADC 수행 주기와 PWM 주파수가 동일하다
#define TIMER_PRESCALER_FREQ        1000000
#define PWM_FREQ                    1000                // PWM 반복 주기 1ms = 1kHz
#define PWM_WIDTH                   (TIMER_PRESCALER_FREQ / PWM_FREQ)

#define DUTY_IDLE                   (PWM_WIDTH / 2)     // 50% duty
#define DUTY_MAX                    (PWM_WIDTH - 1)


#define ADC1_DR_Address     0x4001204C      // ADC1 data register
#define TIM3_CCR1_Address   0x40000434      // TIM3 Compare1 register


int main()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    uint16_t PrescalerValue;

    LED_Init();
    LED_R_ON();

    LLIO_Init(115200);
    printf("\r\nPWM dimming\r\n");


    /* Enable ADC, GPIO, Timer, DMA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);


    /* DMA2 Channel0(ADC) configuration */
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;   // source
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)TIM3_CCR1_Address;    // destination
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);



    /* TIM3_CH1(PB4-LED) configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);



    /* ADC Common Init */
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;     // ADC clock = PCLK2/prescaler. ADC freq = typical 30MHz, Max 36MHz. PCLK2 = 168/2=84MHz.
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);


    /* ADC channel 10 (PC0) configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
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


    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);





    /* Compute the prescaler value */
    SystemCoreClockUpdate();
    PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ) - 1;           // timer base counter에 1MHz 입력

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = PWM_WIDTH - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // TIM3 update 이벤트에서 트리거 발생하도록 설정 -> ADC trigger로 사용
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);


    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = DUTY_IDLE;          // default 50%
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);


    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);





    while(1)
    {
        __WFI();
    }

}




