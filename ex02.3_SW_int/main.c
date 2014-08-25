
/*
 * ex02.3_SW_int
 * GPIO interrupt를 이용해 스위치입력을 받아 LED를 제어하는 예제
 */

#include "stm32f4xx.h"
#include "led.h"
#include "switch.h"

int main()
{
    LED_Init();

    SW_INT_INIT();

    while(1)
    {
        __WFI();
    }

}

// SW1 : PA0(EXTI0) Interrupt Handler
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(SW_1_EXTI_LINE) != RESET)
    {
        LED_R_TOGGLE();
        EXTI_ClearITPendingBit(SW_1_EXTI_LINE);
    }
}

// SW2 : PB10(EXTI10) Interrupt Handler
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(SW_2_EXTI_LINE) != RESET)
    {
        LED_G_TOGGLE();
        EXTI_ClearITPendingBit(SW_2_EXTI_LINE);
    }
}



