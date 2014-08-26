
#include "fatfs_sdc_spi.h"


#define ENABLE_SPI_PERIPH()                                                     \
                do {                                                            \
                    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);       \
                    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);       \
                    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);        \
                } while(0)
#define SPI_CLK_PIN             GPIO_Pin_10
#define SPI_CLK_PINSOURCE       GPIO_PinSource10
#define SPI_MISO_PIN            GPIO_Pin_11
#define SPI_MISO_PINSOURCE      GPIO_PinSource11
#define SPI_MOSI_PIN            GPIO_Pin_12
#define SPI_MOSI_PINSOURCE      GPIO_PinSource12
#define SPI_GPIO                GPIOC
#define SPI_GPIO_AF             GPIO_AF_SPI3
#define SPIn                    SPI3

#define ENABLE_CS_PERIPH()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define SD_CS_PIN               GPIO_Pin_9
#define SD_CS_GPIO              GPIOC

#define SD_CS_ASSERT()          do {                                            \
                                        SD_CS_GPIO->BSRRH = SD_CS_PIN;          \
                                        SD_LED_ON();                            \
                                } while(0)
#define SD_CS_DEASSERT()        do {                                            \
                                        SD_CS_GPIO->BSRRL = SD_CS_PIN;          \
                                        SD_LED_OFF();                           \
                                } while(0)

#define ENABLE_SD_LED_PERIPH()  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define SD_LED_PIN              GPIO_Pin_11
#define SD_LED_GPIO             GPIOB

#define SD_LED_ON()
#define SD_LED_OFF()


#define ENABLE_SD_INSERT_PERIPH()  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define SD_INSERT_PIN           GPIO_Pin_15
#define SD_INSERT_GPIO          GPIOB

#define SD_EXIST()              (GPIO_ReadInputDataBit(SD_INSERT_GPIO, SD_INSERT_PIN) == Bit_RESET)
#define SD_NOT_EXIST()          (GPIO_ReadInputDataBit(SD_INSERT_GPIO, SD_INSERT_PIN) == Bit_SET)


#define ENABLE_SD_ENABLE_PERIPH()  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define SD_ENABLE_PIN           GPIO_Pin_9
#define SD_ENABLE_GPIO          GPIOB

#define SD_ENABLE()
#define SD_DISABLE()




static unsigned long timeOut;
static unsigned long xfersPrMsec = 1;



/*****************************************************************************/

static unsigned char WaitReady(void)
{
  unsigned char res;
  unsigned long retryCount;

  /* Wait for ready in timeout of 500ms */
  retryCount = 500 * xfersPrMsec;
  do
    res = MICROSD_XferSpi(0xff);
  while ((res != 0xFF) && --retryCount);

  return res;
}



void MICROSD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;


    /* Enable SPI and GPIO clocks */
    ENABLE_SPI_PERIPH();

    /* Configure SPI pins: SCK, MISO and MOSI */
    GPIO_PinAFConfig(SPI_GPIO, SPI_CLK_PINSOURCE, SPI_GPIO_AF);
    GPIO_PinAFConfig(SPI_GPIO, SPI_MISO_PINSOURCE, SPI_GPIO_AF);
    GPIO_PinAFConfig(SPI_GPIO, SPI_MOSI_PINSOURCE, SPI_GPIO_AF);

    GPIO_InitStructure.GPIO_Pin = SPI_CLK_PIN | SPI_MOSI_PIN | SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_InitStructure);



    ENABLE_CS_PERIPH();

    GPIO_InitStructure.GPIO_Pin = SD_CS_PIN;                    // CS
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(SD_CS_GPIO, &GPIO_InitStructure);

    SD_CS_DEASSERT();


#if FEATURE_SD_LED
    ENABLE_SD_LED_PERIPH();

    GPIO_InitStructure.GPIO_Pin = SD_LED_PIN;                   // LED
    GPIO_Init(SD_LED_GPIO, &GPIO_InitStructure);

    SD_LED_OFF();
#endif

#if FEATURE_SD_ENABLE
    ENABLE_SD_ENABLE_PERIPH();                                  // SD Enable. load switch

    GPIO_InitStructure.GPIO_Pin = SD_ENABLE_PIN;
    GPIO_Init(SD_ENABLE_GPIO, &GPIO_InitStructure);

    SD_DISABLE();
#endif


    ENABLE_SD_INSERT_PERIPH();

    GPIO_InitStructure.GPIO_Pin = SD_INSERT_PIN;                // SD Insert
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(SD_INSERT_GPIO, &GPIO_InitStructure);






    /* SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPIn, &SPI_InitStructure);

    /* Enable SPI1  */
    SPI_Cmd(SPIn, ENABLE);
}


void MICROSD_PowerOn(void)
{
    SD_ENABLE();

}


void MICROSD_PowerOff(void)
{
    /* Wait for micro SD card ready */
    SD_CS_ASSERT();
    SD_CS_DEASSERT();

    SD_DISABLE();
}




/*
    SPI3은 APB1에 연결되어 있으며 PCLK1 = 42MHz이다.
*/
void MICROSD_SpiClkSlow(void)
{
    SPI_InitTypeDef SPI_InitStructure;

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;       // 42MHz / 128 = 336kHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPIn, &SPI_InitStructure);

    xfersPrMsec = 336000 / 8000;
}

void MICROSD_SpiClkFast(void)
{
    SPI_InitTypeDef SPI_InitStructure;

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;       // 42MHz / 4 = 10.5MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPIn, &SPI_InitStructure);

    xfersPrMsec = 10500000 / 8000;
}


unsigned char MICROSD_XferSpi(unsigned char data)
{
    while (SPI_I2S_GetFlagStatus(SPIn, SPI_I2S_FLAG_TXE) == RESET);

    SPI_I2S_SendData(SPIn, data);

    while(SPI_I2S_GetFlagStatus(SPIn, SPI_I2S_FLAG_RXNE) == RESET);

    return SPI_I2S_ReceiveData(SPIn);
}

int MICROSD_BlockTx(const unsigned char *buff, unsigned char token)
{
    unsigned char resp;
    unsigned long bc = 512;

    if (WaitReady() != 0xFF)
        return 0;

    MICROSD_XferSpi(token);         /* Xmit a token */
    if (token != 0xFD)
    {                               /* Not StopTran token */
        do
        {
            /* Xmit the 512 byte data block to the MMC */
            MICROSD_XferSpi(*buff++);
            MICROSD_XferSpi(*buff++);
            MICROSD_XferSpi(*buff++);
            MICROSD_XferSpi(*buff++);
            bc -= 4;
        } while (bc);

        MICROSD_XferSpi(0xFF);        /* CRC (Dummy) */
        MICROSD_XferSpi(0xFF);
        resp = MICROSD_XferSpi(0xff); /* Receive a data response */
        if ((resp & 0x1F) != 0x05)    /* If not accepted, return with error */
            return 0;
    }
    return 1;
}

int MICROSD_BlockRx(unsigned char *buff, unsigned long btr)
{
    unsigned char token;
    unsigned long retryCount;

    /* Wait for data packet in timeout of 100ms */
    retryCount = 100 * xfersPrMsec;
    do
        token = MICROSD_XferSpi(0xff);
    while ((token == 0xFF) && --retryCount);

    if (token != 0xFE)
        /* Invalid data token */
        return 0;

    if ( timeOut >= btr + 2 )
        timeOut -= btr + 2;
    else
        timeOut = 0;


    do
    {
        *buff++ = MICROSD_XferSpi(0xff);
        *buff++ = MICROSD_XferSpi(0xff);
        *buff++ = MICROSD_XferSpi(0xff);
        *buff++ = MICROSD_XferSpi(0xff);
    } while(btr -= 4);

    MICROSD_XferSpi(0xff);                        /* Discard CRC */
    MICROSD_XferSpi(0xff);

    return 1;     /* Return with success */
}

unsigned char MICROSD_SendCmd(unsigned char cmd, DWORD arg)
{
    unsigned char  n, res;
    unsigned long retryCount;

    if (cmd & 0x80)
    { /* ACMD<n> is the command sequense of CMD55-CMD<n> */
        cmd &= 0x7F;
        res  = MICROSD_SendCmd(CMD55, 0);
        if (res > 1) return res;
    }

    /* Select the card and wait for ready */
    SD_CS_DEASSERT();
    if (!MICROSD_Select())
      return 0xFF;

    /* Send command packet */
    MICROSD_XferSpi(0x40 | cmd);            /* Start + Command index */
    MICROSD_XferSpi((unsigned char)(arg >> 24));  /* Argument[31..24] */
    MICROSD_XferSpi((unsigned char)(arg >> 16));  /* Argument[23..16] */
    MICROSD_XferSpi((unsigned char)(arg >> 8));   /* Argument[15..8] */
    MICROSD_XferSpi((unsigned char) arg);         /* Argument[7..0] */
    n = 0x01;                               /* Dummy CRC + Stop */
    if (cmd == CMD0)
        n = 0x95;                             /* Valid CRC for CMD0(0) */
    if (cmd == CMD8)
        n = 0x87;                             /* Valid CRC for CMD8(0x1AA) */
    MICROSD_XferSpi(n);

    /* Receive command response */
    if (cmd == CMD12)
        MICROSD_XferSpi(0xff);                /* Skip a stuff byte when stop reading */
    retryCount = 10;                        /* Wait for a valid response in timeout of 10 attempts */
    do
        res = MICROSD_XferSpi(0xff);
    while ((res & 0x80) && --retryCount);

    return res;             /* Return with the response value */
}

void MICROSD_TimeOutSet(unsigned long msec)
{
  timeOut = xfersPrMsec * msec;
}

unsigned char MICROSD_TimeOutElapsed(void)
{
    return (timeOut == 0) ? 1 : 0;
}


int MICROSD_Select(void)
{
    SD_CS_ASSERT();
    if (WaitReady() != 0xff)
    {
        SD_CS_DEASSERT();
        return 0;
    }
    return 1;
}

void MICROSD_Deselect(void)
{
    SD_CS_DEASSERT();
}



