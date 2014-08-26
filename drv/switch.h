#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "stm32f4xx.h"


#define SW_1_PERIPH             RCC_AHB1Periph_GPIOA
#define SW_1_GPIO_PORT          GPIOA
#define SW_1_GPIO_PIN           GPIO_Pin_0
#define SW_1_EXTI_IRQn          EXTI0_IRQn
#define SW_1_EXTI_PORTSOURCE    EXTI_PortSourceGPIOA
#define SW_1_EXTI_PINSOURCE     EXTI_PinSource0
#define SW_1_EXTI_LINE          EXTI_Line0

#define SW_2_PERIPH             RCC_AHB1Periph_GPIOB
#define SW_2_GPIO_PORT          GPIOB
#define SW_2_GPIO_PIN           GPIO_Pin_10
#define SW_2_EXTI_IRQn          EXTI15_10_IRQn
#define SW_2_EXTI_PORTSOURCE    EXTI_PortSourceGPIOB
#define SW_2_EXTI_PINSOURCE     EXTI_PinSource10
#define SW_2_EXTI_LINE          EXTI_Line10

#define SW_1_INIT()             SW_X_INIT(1)
#define SW_2_INIT()             SW_X_INIT(2)

#define SW_1_INT_INIT()         SW_X_INT_INIT(1)
#define SW_2_INT_INIT()         SW_X_INT_INIT(2)

#define SW_INIT()                                                               \
        do {                                                                    \
            SW_1_INIT();                                                        \
            SW_2_INIT();                                                        \
        } while(0)

#define SW_INT_INIT()                                                           \
        do {                                                                    \
            SW_1_INT_INIT();                                                    \
            SW_2_INT_INIT();                                                    \
        } while(0)


#define SW_X_INIT(X)                                                            \
        do {                                                                    \
            GPIO_InitTypeDef GPIO_InitStructure;                                \
            RCC_AHB1PeriphClockCmd(SW_##X##_PERIPH, ENABLE);                    \
            GPIO_InitStructure.GPIO_Pin   = SW_##X##_GPIO_PIN;                  \
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;                       \
            GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;                       \
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                      \
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;                    \
            GPIO_Init(SW_##X##_GPIO_PORT, &GPIO_InitStructure);                 \
        } while(0)

#define SW_X_INT_INIT(X)                                                        \
        do {                                                                    \
             EXTI_InitTypeDef EXTI_InitStructure;                               \
            NVIC_InitTypeDef NVIC_InitStructure;                                \
            RCC_AHB1PeriphClockCmd(SW_##X##_PERIPH, ENABLE);                    \
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);              \
            SYSCFG_EXTILineConfig(SW_##X##_EXTI_PORTSOURCE, SW_##X##_EXTI_PINSOURCE);\
            NVIC_InitStructure.NVIC_IRQChannel = SW_##X##_EXTI_IRQn;            \
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;        \
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;               \
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     \
            NVIC_Init(&NVIC_InitStructure);                                     \
            EXTI_InitStructure.EXTI_Line = SW_##X##_EXTI_LINE;                  \
            EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                 \
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;             \
            EXTI_InitStructure.EXTI_LineCmd = ENABLE;                           \
            EXTI_Init(&EXTI_InitStructure);                                     \
            SW_X_INIT(##X##);                                                   \
        } while(0)

#define SW_1_IS_PUSHED()        SW_X_IS_PUSHED(1)
#define SW_2_IS_PUSHED()        SW_X_IS_PUSHED(2)
#define SW_X_IS_PUSHED(X)       (GPIO_ReadInputDataBit(SW_##X##_GPIO_PORT, SW_##X##_GPIO_PIN) == Bit_RESET)

#endif /* __SWITCH_H__ */
