
/*
  ex08.1_DAC_1ch

  DAC peripheral 동작 확인.
  0~4095 사이의 숫자를 콘솔로 입력받아 DAC1 채널로 아날로그 출력 내보낸다.
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"

int main()
{
    char buff[64];
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitStructure;
    int val;

    LED_Init();
    LED_R_ON();


    /* GPIOA clock enable (to be used with DAC) */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    /* DAC Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);


    /* DAC channel 1 (DAC_OUT1 = PA.4) configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    DAC_SetChannel1Data(DAC_Align_12b_R, 0);        // 초기값 = 0
    DAC_Cmd(DAC_Channel_1, ENABLE);


    LLIO_Init(115200);
    printf("\r\nDAC Test\r\n");
    printf("\r\nEnter a value[0..4095]\r\n");

    while(1)
    {
        if (scanf("%d", &val) == 1)
        {
            if (val > 4095)
                val = 4095;
            if (val < 0)
                val = 0;

            printf("  %d\r\n", val);
            DAC_SetChannel1Data(DAC_Align_12b_R, val);

            printf("\r\nEnter a value[0..4095]\r\n");
        }
        else
            scanf("%s", buff);      // flush read FIFO
    }

}

