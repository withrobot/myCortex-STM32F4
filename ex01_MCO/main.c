
/*
 * ex01_MCO
 * MCO1 pin(PA8)을 통해 system master clock / 2 (84MHz)을 출력.
 */

#include "stm32f4xx.h"

int main()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // Configure MCO1 (PA8) as alternate function, push-pull
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure MCO1 output to System Clock output / 2
    RCC_MCO1Config(RCC_MCO1Source_PLLCLK, RCC_MCO1Div_2);

}

