
/*
  ex04.4_stdio

  USART를 이용해 printf, scanf 구현
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"

int main()
{
    int n;
    char buff[128];

    LED_Init();

    LLIO_Init(115200);


    printf("\r\nstdio example\r\n");
    printf("\r\nEnter any number...\r\n");

    while(1)
    {
        if (scanf("%d", &n) == 1)
        {
            printf("%d is entered.\r\n", n);

            printf("\r\nEnter any number...\r\n");
        }
        else
            scanf("%s", buff);      // flush read FIFO

        LED_G_TOGGLE();
    }

}


