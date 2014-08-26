
/*
  ex09.2_I2C_myAHRS

  myAHRS 모듈 인터페이스 예제.
  I2C 인터페이스를 이용해 myAHRS로 부터 출력값을 얻어 UART로 출력한다.
  CPAL 라이브러리를 사용해 I2C를 인터페이스 한다.
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"
#include "cpal.h"
#include "cpal_i2c.h"
#include "cpal_conf.h"

#include "myAHRS.h"


// myAHRS의 레지스터 맵과 동일한 모양으로 구성된 구조체
// 레지스터에서 읽은 값을 이 구조체로 형변환 하면 별도의 비트 연산이 필요없이 바로 센서 값을 얻을 수 있다.
// 또한 한번의 i2c 통신의 overhead도 줄일 수 있다.
typedef struct
{
    short acc_x;
    short acc_y;
    short acc_z;
    short gyro_x;
    short gyro_y;
    short gyro_z;
    short magnet_x;
    short magnet_y;
    short magnet_z;
    unsigned char dummy[6];     // magnet_z_high 레지스터와 roll_low 레지스터 사이의 6 바이트 만큼 빈 공간 삽입
    short roll;
    short pitch;
    short yaw;
} ahrs_t;



static CPAL_TransferTypeDef buff;
static unsigned char i2c_buff[32];


int main()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;




    LED_Init();
    LED_R_ON();


    CPAL_I2C_StructInit(&I2C1_DevStructure);
    I2C1_DevStructure.CPAL_Dev = CPAL_I2C1;
    I2C1_DevStructure.pCPAL_I2C_Struct->I2C_ClockSpeed = 400000;
    I2C1_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_DMA;
    I2C1_DevStructure.wCPAL_Options = 0;

    CPAL_I2C_Init(&I2C1_DevStructure);




    // Interrupt input(PB5) configuration
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);



    LLIO_Init(115200);
    printf("\r\nmyAHRS I2C example\r\n");




    // myAHRS의 WHO_AM_I 레지스터 값을 읽어 출력한다.
    // read 결과값은 등록된 user callback을 통해 받아온다.
    buff.pbBuffer = i2c_buff;
    buff.wAddr1 = AHRS_SLAVE_ADDRESS << 1;
    buff.wAddr2 = AHRS_REG_WHO_AM_I;
    buff.wNumData = 1;

    I2C1_DevStructure.pCPAL_TransferRx = &buff;
    printf("\r\n\r\nread : %d\r\n", CPAL_I2C_Read(&I2C1_DevStructure));


    while(1)
    {
        __WFI();    // 인터럽트가 발생할 때 까지 이곳에서 대기
    }

}







uint32_t CPAL_TIMEOUT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
    printf("\r\nCPAL timeout error\r\n");

    return CPAL_PASS;
}



void CPAL_I2C_RXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
    unsigned char reg;
    ahrs_t *ahrs;

    float acc_x, acc_y, acc_z;
    float gyro_x, gyro_y, gyro_z;
    float magnet_x, magnet_y, magnet_z;
    float roll, pitch, yaw;



    printf("\r\nread TC CB. len = %d\r\n", pDevInitStruct->pCPAL_TransferRx->wNumData);

    reg = pDevInitStruct->pCPAL_TransferRx->wAddr2;
    if (reg == AHRS_REG_WHO_AM_I)
    {
        // 0xB1이 출력되어야 정상이다.
        printf("WHO AM I : 0x%02X\r\n", pDevInitStruct->pCPAL_TransferRx->pbBuffer[0]);
    }
    else if (reg == AHRS_REG_ACC_X_LOW)
    {
        ahrs = (ahrs_t *) pDevInitStruct->pCPAL_TransferRx->pbBuffer;



        acc_x = (float)ahrs->acc_x * 16.0 / 32767.;
        acc_y = (float)ahrs->acc_y * 16.0 / 32767.;
        acc_z = (float)ahrs->acc_z * 16.0 / 32767.;

        gyro_x = (float)ahrs->gyro_x * 2048. / 32767.;
        gyro_y = (float)ahrs->gyro_y * 2048. / 32767.;
        gyro_z = (float)ahrs->gyro_z * 2048. / 32767.;

        magnet_x = (float)ahrs->magnet_x * 8.0 / 32767.;
        magnet_y = (float)ahrs->magnet_y * 8.0 / 32767.;
        magnet_z = (float)ahrs->magnet_z * 8.0 / 32767.;

        roll = (float)ahrs->roll * 0.01;
        pitch = (float)ahrs->pitch * 0.01;
        yaw = (float)ahrs->yaw * 0.01;

        printf("ACC[m/s/s]:%.2f,%.2f,%.2f\r\n", acc_x, acc_y, acc_z);
        printf("GYRO[deg/s]:%.2f,%.2f,%.2f\r\n", gyro_x, gyro_y, gyro_z);
        printf("MAG[Gauss]:%.2f,%.2f,%.2f\r\n", magnet_x, magnet_y, magnet_z);
        printf("R/P[deg]:%.2f,%.2f,%.2f\r\n\r\n", roll, pitch, yaw);


        LED_G_TOGGLE();

    }


}

void CPAL_I2C_ERR_UserCallback(CPAL_DevTypeDef pDevInstance, uint32_t DeviceError)
{
    printf("\r\nCPAL Error. %d\r\n", DeviceError);
}


static int int_cnt;

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line5);

        if (int_cnt++ % 10 == 0)
        {
            buff.pbBuffer = i2c_buff;
            buff.wAddr1 = AHRS_SLAVE_ADDRESS << 1;
            buff.wAddr2 = AHRS_REG_ACC_X_LOW;
            buff.wNumData = sizeof(ahrs_t);
            I2C1_DevStructure.pCPAL_TransferRx = &buff;
            CPAL_I2C_Read(&I2C1_DevStructure);
        }
    }
}




