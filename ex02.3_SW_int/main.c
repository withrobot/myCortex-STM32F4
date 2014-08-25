
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

// SW1 : PE0(EXTI0) Interrupt Handler
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        LED_R_TOGGLE();
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

// SW2 : PE1(EXTI1) Interrupt Handler
void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        LED_G_TOGGLE();
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}



