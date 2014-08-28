
/*
  ex05.2_FPU

  FPU 사용 여부에 따른 연산 속도 차이 확인

   - FPU 켜기 : EWARM Project->Options->General Options->Target tab->FPU = VFPv4
   - FPU 끄기 : EWARM Project->Options->General Options->Target tab->FPU = None
 */

#include <stdio.h>
#include <math.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"
#include "delay.h"

static volatile unsigned long tick_count;



static void TimerCount_Start(void);
static uint32_t TimerCount_Stop(uint32_t Value);




int main()
{
    volatile float Result;
    float Data1=1.0f, Data2=2.0f ;
    unsigned long nb_cycles = 0x00;
    int i;

    LED_Init();
    LED_R_ON();
    LED_G_ON();

    LLIO_Init(115200);

#if (__FPU_USED == 1)
    printf("\r\nUsing FPU\r\n");
#else
    printf("\r\nWithout FPU\r\n");
#endif


    while(1)
    {
        delay_ms(500);

        LED_G_ON();
        TimerCount_Start(); /* Start SYSTick counter @ HCLK rate(168MHz) */

        /* Initialze float variables */
        Data1 = 1.0 ;
        Data2 = 2.0;

        /* Start a set of mathematical operations */
        for (i = 0; i < 100; i++)
        {
            Result = Data1 * Data2 + Data1 / Data2;
            Data1 = Data2;
            Data2 = Result;
        }

        LED_G_OFF();
        nb_cycles = TimerCount_Stop(nb_cycles); /* Return SYSTick Counter value */

        printf("CPU time = %d\r\n", nb_cycles);

        nb_cycles = 0x00; /* Reset for the next time through loop */
    }
}


void SysTick_Handler(void)
{
    LED_G_TOGGLE();

    tick_count++;
}


/**
  * @brief  Reloads the 24-bit SYStick timer to start counting from 0.
  * @param  None
  * @retval None
  */
static void TimerCount_Start(void)
{

  SysTick->LOAD  =  0xFFFFFF  ;	/* set reload register */
  SysTick->VAL  =  0  ;		/* Clear Counter */
  SysTick->CTRL  =  0x5 ;	/* Enable Counting*/
}


/**
  * @brief  Stop the Systick counter and Retrieve the # CPU clocks elapsed.
  * @param  The parameter to hold the Systick counter value.
  * @retval the number of Systick counts.
  */
static uint32_t TimerCount_Stop(uint32_t Value)
{
  Value = SysTick->VAL;  /* Load the SysTick Counter Value */
  SysTick->CTRL  =0;	/* Disable Counting */
  Value = 0xFFFFFF - Value;/* Capture Counts in CPU Cycles*/

  return Value;
}

