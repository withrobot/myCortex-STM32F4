/*
        assert.c

    assert 함수 구현.

    USE_FULL_ASSERT : assert 사용여부 결정
    FEATURE_ASSERT_LED : assert 발생시 LED 1개를 빠르게 점멸하는 구현
    FEATURE_ASSERT_DBG : assert 발생시 코드 정보를 debug 채널로 출력하는 구현(TBD)

  Date          Who     Description
  ------------  ------  ----------------------------------------------
  2012-09-15    hwpark  최초구현
  2013-05-17    hwpark  STM32F10x와 STM32F4xx용 범용 코드로 정리
  2013-10-28    hwpark  LED는 led.h 파일을 사용하도록 수정
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#if TARGET_STM32F10X
#include "stm32f10x.h"
#elif TARGET_STM32F4XX
#include "stm32f4xx.h"
#else
#error Target MCU is not defined.
#endif





#if FEATURE_ASSERT_LED || FEATURE_ASSERT_DBG

#include "led.h"


#if TARGET_STM32F10X

#if FEATURE_ASSERT_DBG
#define ENABLE_DBG_UART_PERIPH()            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE)
#define DBG_GPIO_USART                      GPIOA

#define DBG_GPIO_USART_Rx_Pin               GPIO_Pin_10
#define DBG_GPIO_USART_Tx_Pin               GPIO_Pin_9

#define DBG_UARTn                           USART1
#define DBG_UART_BAUDRATE                   115200
#endif  // FEATURE_ASSERT_DBG


#elif TARGET_STM32F4XX

// UART 출력 관련 peripheral
#if FEATURE_ASSERT_DBG
#define DBG_UART_GPIO_PERIPH                RCC_AHB1Periph_GPIOA
#define DBG_UART_GPIO                       GPIOA
#define DBG_UART_TX_PIN                     GPIO_Pin_9
#define DBG_UART_RX_PIN                     GPIO_Pin_10
#define DBG_UART_TX_PIN_SOURCE              GPIO_PinSource9
#define DBG_UART_RX_PIN_SOURCE              GPIO_PinSource10

#define DBG_UART_PERIPH                     RCC_APB2Periph_USART1
#define DBG_UARTn                           USART1
#define DBG_UART_BAUDRATE                   115200

#endif       // FEATURE_ASSERT_DBG

#endif       // TARGET





#if FEATURE_ASSERT_DBG
#define ASSERT_LOG_MSG_LEN              256

static void assert_printf(const char *str, ...)
{
    va_list vaArgP;
    char buff[ASSERT_LOG_MSG_LEN];
    char *c;

    //
    // Start the varargs processing.
    //
    va_start(vaArgP, str);

    //
    // Call vsnprintf to perform the conversion.
    //
    vsnprintf(buff, ASSERT_LOG_MSG_LEN, str, vaArgP);

    //
    // End the varargs processing.
    //
    va_end(vaArgP);

    c = buff;
    while(*c)
    {
        while(USART_GetFlagStatus(DBG_UARTn, USART_FLAG_TC) == RESET);
        USART_SendData(DBG_UARTn, (uint16_t)*c);

        c++;
    }
}
#endif          // FEATURE_ASSERT_DBG





/*
    ASSERT 발생시 호출되는 함수.
    file name, line number 등을 UART로 출력하거나 LED를 켜거나 등의 용도로 사용 가능하다.
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    // assert 발생 시 적색 LED를 점멸시키는 assert 구현.
    GPIO_InitTypeDef GPIO_InitStructure;
    volatile int i;
    int cnt;
#if FEATURE_ASSERT_DBG
    USART_InitTypeDef USART_InitStructure;
#endif

    __disable_irq();

    // LED
    LED_R_INIT();
#if TARGET_STM32F10X

#if FEATURE_ASSERT_DBG
    ENABLE_DBG_UART_PERIPH();

    GPIO_InitStructure.GPIO_Pin   = DBG_GPIO_USART_Tx_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(DBG_GPIO_USART, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = DBG_GPIO_USART_Rx_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DBG_GPIO_USART, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate   = DBG_UART_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;
    USART_InitStructure.USART_Parity     = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl
                                         = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(DBG_UARTn, &USART_InitStructure);

    USART_Cmd(DBG_UARTn, ENABLE);
#endif

#elif TARGET_STM32F4XX

#if FEATURE_ASSERT_DBG
    // Enable USART GPIO
    RCC_AHB1PeriphClockCmd(DBG_UART_GPIO_PERIPH, ENABLE);

    // GPIO AF mapping
    GPIO_PinAFConfig(DBG_UART_GPIO, DBG_UART_TX_PIN_SOURCE, GPIO_AF_USART1);
    GPIO_PinAFConfig(DBG_UART_GPIO, DBG_UART_RX_PIN_SOURCE, GPIO_AF_USART1);

    // Configure GPIOB as alternate function, push-pull
    GPIO_InitStructure.GPIO_Pin   = DBG_UART_TX_PIN | DBG_UART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(DBG_UART_GPIO, &GPIO_InitStructure);

    // Enable USART
    RCC_APB2PeriphClockCmd(DBG_UART_PERIPH, ENABLE);

    USART_InitStructure.USART_BaudRate = DBG_UART_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(DBG_UARTn, &USART_InitStructure);
    USART_Cmd(DBG_UARTn, ENABLE);
#endif          // FEATURE_ASSERT_DBG
#endif          // TARGET


    SystemCoreClockUpdate();
    cnt = SystemCoreClock / 120;

    while(1)
    {
        LED_R_OFF();
        for(i = 0; i < cnt * 3; i++);
        LED_R_ON();
        for(i = 0; i < cnt; i++);


#if FEATURE_ASSERT_DBG
        assert_printf("\r\n\r\n[ASSERT] %s:%d\r\n", file, line);
#endif
    }

}

#else   // FEATURE_ASSERT_LED || FEATURE_ASSERT_DBG
void assert_failed(uint8_t* file, uint32_t line)
{
    // 아무것도 하지 않는 assert 구현.
    __disable_irq();

    /* Infinite loop */
    while (1)
    {
    }
}
#endif  // FEATURE_ASSERT_LED || FEATURE_ASSERT_DBG



