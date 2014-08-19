/*
  LED driver
  2013 withrobot Inc.

  - Device initialization
  - LED On/Off control

  * For use with STM32F4xx

  Revision history
  ----------------------------------------------------------------------
  Data          who     Description.
  2012.05.05    hwpark  Initial code writing.
  2012.06.16    hwpark  port HyGPS+ prototype. From KarGPS
  2013.10.28    hwpark  JTAG pin(PA15, PB4)에 LED 사용하는 코드 추가
  ----------------------------------------------------------------------
 */

#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"

#define LED_R_PERIPH            RCC_AHB1Periph_GPIOA
#define LED_R_PORT              GPIOA
#define LED_R_PIN               GPIO_Pin_15

#define LED_G_PERIPH            RCC_AHB1Periph_GPIOB
#define LED_G_PORT              GPIOB
#define LED_G_PIN               GPIO_Pin_4



#define LED_R_INIT()            LED_X_INIT(R)
#define LED_R_ON()              LED_X_ON(R)
#define LED_R_OFF()             LED_X_OFF(R)
#define LED_R_IS_ON()           LED_X_IS_ON(R)
#define LED_R_TOGGLE()          LED_X_TOGGLE(R)

#define LED_G_INIT()            LED_X_INIT(G)
#define LED_G_ON()              LED_X_ON(G)
#define LED_G_OFF()             LED_X_OFF(G)
#define LED_G_IS_ON()           LED_X_IS_ON(G)
#define LED_G_TOGGLE()          LED_X_TOGGLE(G)




#define LED_X_INIT(X)                                                           \
        do {                                                                    \
            GPIO_InitTypeDef GPIO_InitStructure;                                \
            RCC_AHB1PeriphClockCmd(LED_##X##_PERIPH, ENABLE);                   \
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_CAN1);            \
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_CAN1);             \
            GPIO_InitStructure.GPIO_Pin   = LED_##X##_PIN;                      \
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;                      \
            GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;                   \
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                      \
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                   \
            GPIO_Init(LED_##X##_PORT, &GPIO_InitStructure);                     \
            LED_X_OFF(##X##);                                                   \
        } while (0)


#define LED_X_ON(X)             LED_##X##_PORT->BSRRH = LED_##X##_PIN
#define LED_X_OFF(X)            LED_##X##_PORT->BSRRL = LED_##X##_PIN
#define LED_X_IS_ON(X)          !(LED_##X##_PORT->ODR & LED_##X##_PIN)
#define LED_X_TOGGLE(X)         LED_##X##_PORT->ODR ^= LED_##X##_PIN




void LED_Init(void);


#endif /* __LED_H__ */


