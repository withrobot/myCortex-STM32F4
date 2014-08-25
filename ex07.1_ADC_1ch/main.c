
/*
  ex07.1_ADC_1ch

  ADC peripheral 동작 확인.
  가변 저항의 값을 읽어 전압과 저항값을 출력한다.
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"

int main()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    unsigned short adc_result;
    float volt;
    int resistor;

    LED_Init();
    LED_R_ON();



    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);



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
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);


    /* ADC1 regular channel10 configuration */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);


    LLIO_Init(115200);
    printf("\r\nADC Test\r\n");


    SystemCoreClockUpdate();
    if (SysTick_Config(SystemCoreClock / 20))   // 20Hz tick interrupt
    {
        assert_param(0);
    }

    while(1)
    {
        __WFI();

        ADC_SoftwareStartConv(ADC1);                            // ADC 시작

        if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)       // ADC 완료까지 대기
        {
            adc_result = ADC_GetConversionValue(ADC1);
            volt = (float)adc_result * 3.3 / 4096.;
            resistor = adc_result * 10000 / 4096;
            printf("%d,%.2f,%d\r\n", adc_result, volt, resistor);     // ADC 결과 읽어 출력
        }
    }

}

void SysTick_Handler(void)
{
    LED_G_TOGGLE();
}



