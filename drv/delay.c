
#include "stm32f4xx.h"

/*
    기본 delay.
    이 함수는 수행되고 리턴할 때 까지 3 cycle 시간이 걸린다.
    compiler optimization에 영향 받지 않는다.
    system core clock이 변경되면 instruction cache의 prefatch latency가 달라지므로 이 함수의 수행 시간이 달라질 수 있다.
    현재 코드는168MHz용 구성.
*/
static void _delay_tick(unsigned long ulCount)
{
    __asm("    subs    r0, #1\n"
          "    bne.n   _delay_tick\n"
          "    bx      lr");
}





// micro second 단위로 delay
void delay_us(unsigned long us)
{
    _delay_tick(us * (168000000 / 3 / 1000000));
}





// mili second 단위로 delay
void delay_ms(unsigned long ms)
{
    delay_us(ms * 1000);
}


