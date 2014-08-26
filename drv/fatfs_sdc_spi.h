
#ifndef __FATFS_SDC_SPI_H__
#define __FATFS_SDC_SPI_H__

#include "stm32f4xx.h"
#include "integer.h"

/* Definitions for MMC/SDC command */
#define CMD0      (0)         /**< GO_IDLE_STATE */
#define CMD1      (1)         /**< SEND_OP_COND */
#define ACMD41    (41 | 0x80) /**< SEND_OP_COND (SDC) */
#define CMD8      (8)         /**< SEND_IF_COND */
#define CMD9      (9)         /**< SEND_CSD */
#define CMD10     (10)        /**< SEND_CID */
#define CMD12     (12)        /**< STOP_TRANSMISSION */
#define ACMD13    (13 | 0x80) /**< SD_STATUS (SDC) */
#define CMD16     (16)        /**< SET_BLOCKLEN */
#define CMD17     (17)        /**< READ_SINGLE_BLOCK */
#define CMD18     (18)        /**< READ_MULTIPLE_BLOCK */
#define CMD23     (23)        /**< SET_BLOCK_COUNT */
#define ACMD23    (23 | 0x80) /**< SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24     (24)        /**< WRITE_BLOCK */
#define CMD25     (25)        /**< WRITE_MULTIPLE_BLOCK */
#define CMD41     (41)        /**< SEND_OP_COND (ACMD) */
#define CMD55     (55)        /**< APP_CMD */
#define CMD58     (58)        /**< READ_OCR */



void MICROSD_Init(void);

void MICROSD_PowerOn(void);
void MICROSD_PowerOff(void);

void MICROSD_SpiClkSlow(void);
void MICROSD_SpiClkFast(void);

unsigned char MICROSD_XferSpi(unsigned char data);
int MICROSD_BlockRx(unsigned char *buff, unsigned long btr);
int MICROSD_BlockTx(const unsigned char *buff, unsigned char token);

unsigned char MICROSD_SendCmd(unsigned char cmd, DWORD arg);


void MICROSD_TimeOutSet(unsigned long msec);
unsigned char MICROSD_TimeOutElapsed(void);

int MICROSD_Select(void);
void MICROSD_Deselect(void);


#endif // __FATFS_SDC_SPI_H__

