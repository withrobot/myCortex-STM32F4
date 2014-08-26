
/*
  ex10.1_FatFs

  FatFs 테스트.
  SPI mode로 SD 카드 사용.

  SDXC 카드를 사용하는 경우 FAT32로 포맷해서 사용해야 한다.
 */

#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"
#include "ff.h"
#include "fatfs_sdc_spi.h"
#include "fatfs_diskio_spi.h"




static FATFS Fatfs;
static FIL g_file;
static DIR g_dir;
static FILINFO g_fi;





static int init_fs(void)
{
    MICROSD_Init();

    if (f_mount(0, &Fatfs) != FR_OK)
      return -1;
    return 0;
}






int main()
{
    char buff[64];
    FRESULT fret;
    unsigned int bw;
    int i;

    LED_Init();
    LED_R_ON();

    LLIO_Init(115200);
    printf("\r\nFatFs Test\r\n");

    init_fs();

    printf("1 : open \"test.txt\" file\r\n");
    printf("2 : write \"Hello\\r\\n\" to \"test.txt\" file\r\n");
    printf("3 : write 512 byte bulk data to \"test.txt\" file\r\n");
    printf("4 : close \"test.txt\" file\r\n");
    printf("5 : list all files with there size\r\n");

    while(1)
    {
        scanf("%s", buff);

        if (buff[0])
        {
            LED_G_TOGGLE();

            if (strcmp(buff, "1") == 0)             // file open
            {
                fret = f_open(&g_file, "test.txt", FA_WRITE | FA_CREATE_ALWAYS);

                printf("Open test.txt return : %d\r\n", fret);
            }
            else if (strcmp(buff, "2") == 0)        // Write
            {
                sprintf(buff, "Hello\r\n");
                fret = f_write(&g_file, buff, strlen(buff), &bw);
                printf("Write \"Hello\" to test.txt result : %d\r\n", fret);
            }
            else if (strcmp(buff, "3") == 0)        // Write Bulk
            {
                sprintf(buff, "01234567");
                for (i = 0; i < 512; i += 8)
                {
                    fret = f_write(&g_file, buff, strlen(buff), &bw);
                    printf("W : %d,%d\r\n", fret, i);
                }
            }
            else if (strcmp(buff, "4") == 0)        // File close
            {
                fret = f_close(&g_file);
                printf("Close test.txt result : %d\r\n", fret);
            }
            else if (strcmp(buff, "5") == 0)        // Dir
            {
                fret = f_opendir(&g_dir, "/");
                printf("Open dir result : %d\r\n", fret);

                while(1)
                {
                    fret = f_readdir(&g_dir, &g_fi);
                    if (fret != FR_OK || g_fi.fname[0] == 0)    // error or end of file info
                        break;

                    printf("  %-14s %d\r\n", g_fi.fname, g_fi.fsize);
                }
            }


        }
    }

}

