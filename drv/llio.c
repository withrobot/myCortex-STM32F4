/*
 * Low-level library support
 * standard in/out
 */

#include <stdio.h>
#include <stddef.h>
#include "stm32f4xx.h"
#include <yfuns.h>
#include "llio.h"


#define FEATURE_LLIO_UART


#define RCC_AHB1Periph_GPIO_USART       RCC_AHB1Periph_GPIOA

#define GPIO_USART                      GPIOA

#define GPIO_USART_Rx_Pin               GPIO_Pin_10
#define GPIO_USART_Rx_PinSource         GPIO_PinSource10
#define GPIO_USART_Tx_Pin               GPIO_Pin_9
#define GPIO_USART_Tx_PinSource         GPIO_PinSource9
#define GPIO_AF_USART                   GPIO_AF_USART1

#define RCC_APB2Periph_USART            RCC_APB2Periph_USART1
#define UARTn                           USART1


void LLIO_Init(unsigned long baudrate)
{
#ifdef FEATURE_LLIO_UART
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOA clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIO_USART, ENABLE);

    /* Enable UARTn clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART, ENABLE);

    // UART configuration ...

    // GPIO AF config for UART1
    GPIO_PinAFConfig(GPIOA, GPIO_USART_Tx_PinSource, GPIO_AF_USART);
    GPIO_PinAFConfig(GPIOA, GPIO_USART_Rx_PinSource, GPIO_AF_USART);


    /* Configure USARTx_Tx/USARTx_Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_USART_Tx_Pin | GPIO_USART_Rx_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_USART, &GPIO_InitStructure);


    /* USARTx configuration ------------------------------------------------------*/
    USART_InitStructure.USART_BaudRate   = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;
    USART_InitStructure.USART_Parity     = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl
                                         = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;

    /* Configure the USARTx */
    USART_Init(UARTn, &USART_InitStructure);

    /* Enable the UARTn */
    USART_Cmd(UARTn, ENABLE);
#endif
}


#ifdef FEATURE_LLIO_UART
#pragma module_name = "?__write"
static int MyLowLevelPutchar(int ch);
static int MyLowLevelGetchar();
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
  size_t nChars = 0;

  if (buffer == 0)
  {
    /*
     * This means that we should flush internal buffers.  Since we
     * don't we just return.  (Remember, "handle" == -1 means that all
     * handles should be flushed.)
     */
    return 0;
  }

  /* This template only writes to "standard out" and "standard err",
   * for all other file handles it returns failure. */
  if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size != 0; --size)
  {
    if (MyLowLevelPutchar(*buffer++) < 0)
    {
      return _LLIO_ERROR;
    }

    ++nChars;
  }

  return nChars;
}

static int MyLowLevelPutchar(int ch)
{
    USART_SendData(UARTn, (u8) ch);

    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(UARTn, USART_FLAG_TXE) == RESET);

    return ch;
}

//#pragma module_name = "?__read"
size_t __read(int handle, unsigned char * buffer, size_t size)
{
  int nChars = 0;

  /* This template only reads from "standard in", for all other file
   * handles it returns failure. */
  if (handle != _LLIO_STDIN)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size > 0; --size)
  {
    int c = MyLowLevelGetchar();
    if (c < 0)
      break;

    *buffer++ = c;
    ++nChars;
  }

  return nChars;
}

static int MyLowLevelGetchar()
{
    while (USART_GetFlagStatus(UARTn, USART_FLAG_RXNE) == RESET);
    return USART_ReceiveData(UARTn);
}
#else
#pragma module_name = "?__write"
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    return 0;
}
size_t __read(int handle, unsigned char * buffer, size_t size)
{
    return 0;
}
#endif




