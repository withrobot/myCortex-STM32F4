
/*
  ex09.4_I2C_myAHRS+

  myAHRS+ 모듈 인터페이스 예제.
  I2C 인터페이스를 이용해 myAHRS+로 부터 출력값을 얻어 UART로 출력한다.
  CPAL 라이브러리를 사용해 I2C를 인터페이스 한다.

  Example for myAHRS+ interface.
  Get myAHRS+ output data through I2C and print them on UART.
  Use CPAL library for I2C(by ST)
 */

#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "llio.h"
#include "cpal.h"
#include "cpal_i2c.h"
#include "cpal_conf.h"
#include "delay.h"

#include "myAHRS+.h"


// myAHRS+의 레지스터 맵과 동일한 모양으로 구성된 구조체
// 레지스터에서 읽은 값을 이 구조체로 형변환 하면 별도의 비트 연산이 필요없이 바로 센서 값을 얻을 수 있다.
// 또한 한번의 i2c 통신의 overhead도 줄일 수 있다.
// This structure used for data type casting.
// myAHRS+ registers has little endian format, so simple type casting can do parsing job
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
} ahrs_sensor_t;

typedef struct
{
    short roll;
    short pitch;
    short yaw;
} ahrs_euler_t;

typedef struct
{
    short x;
    short y;
    short z;
    short w;
} ahrs_quaternian_t;

static CPAL_TransferTypeDef buff;
static unsigned char i2c_buff[sizeof(ahrs_sensor_t)];


int main()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;




    LED_Init();
    LED_R_ON();


    // Initialize CPAL
    CPAL_I2C_StructInit(&I2C1_DevStructure);
    I2C1_DevStructure.CPAL_Dev = CPAL_I2C1;
    I2C1_DevStructure.pCPAL_I2C_Struct->I2C_ClockSpeed = 400000;
    I2C1_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_DMA;
    I2C1_DevStructure.wCPAL_Options = 0;

    CPAL_I2C_Init(&I2C1_DevStructure);


    // Delay after reset.
    // myAHRS+'s reset is tied with myCortex-STM32F4-EXP.
    // myAHRS+ takes about 1.5s for boot-up.
    delay_ms(3000);



    // Interrupt input(PE2) configuration
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);



    LLIO_Init(115200);
    printf("\r\nmyAHRS+ I2C example\r\n");




    // myAHRS의 WHO_AM_I 레지스터 값을 읽어 출력한다.
    // read 결과값은 등록된 user callback을 통해 받아온다.
    // Read WHO_AM_I register.
    // CPAL operations are asynchronous. When read is done, User callback is invoked
    buff.pbBuffer = i2c_buff;
    buff.wAddr1 = AHRS_SLAVE_ADDRESS << 1;
    buff.wAddr2 = AHRS_REG_WHO_AM_I;
    buff.wNumData = 1;

    I2C1_DevStructure.pCPAL_TransferRx = &buff;
    printf("\r\n\r\nread : %d\r\n", CPAL_I2C_Read(&I2C1_DevStructure));


    while(1)
    {
        __WFI();    // 인터럽트가 발생할 때 까지 이곳에서 대기. Wait for Interrupt from myAHRS+
    }

}






// CPAL timeout error callback
uint32_t CPAL_TIMEOUT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
    printf("\r\nCPAL timeout error\r\n");

    return CPAL_PASS;
}



// CPAL read transfer complete user callback
void CPAL_I2C_RXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
    unsigned char reg;
    ahrs_sensor_t *sensor;
    short *temp_read;
    ahrs_euler_t *euler;
    ahrs_quaternian_t *q;

    float acc_x, acc_y, acc_z;
    float gyro_x, gyro_y, gyro_z;
    float magnet_x, magnet_y, magnet_z;
    float temp;
    float roll, pitch, yaw;
    float q_x, q_y, q_z, q_w;



//    printf("\r\nread TC CB. len = %d\r\n", pDevInitStruct->pCPAL_TransferRx->wNumData);

    reg = pDevInitStruct->pCPAL_TransferRx->wAddr2;
    if (reg == AHRS_REG_WHO_AM_I)
    {
        // 0xB1이 출력되어야 정상이다.
        // Must be 0xB1
        printf("WHO AM I : 0x%02X\r\n", pDevInitStruct->pCPAL_TransferRx->pbBuffer[0]);
    }
    else if (reg == AHRS_REG_ACC_X_LOW)
    {
        // raw sensor data
        sensor = (ahrs_sensor_t *) pDevInitStruct->pCPAL_TransferRx->pbBuffer;



        acc_x = (float)sensor->acc_x * 16.0 / 32767.;
        acc_y = (float)sensor->acc_y * 16.0 / 32767.;
        acc_z = (float)sensor->acc_z * 16.0 / 32767.;

        gyro_x = (float)sensor->gyro_x * 2000. / 32767.;
        gyro_y = (float)sensor->gyro_y * 2000. / 32767.;
        gyro_z = (float)sensor->gyro_z * 2000. / 32767.;

        magnet_x = (float)sensor->magnet_x * 0.3;
        magnet_y = (float)sensor->magnet_y * 0.3;
        magnet_z = (float)sensor->magnet_z * 0.3;

        printf("\r\n");
        printf("ACC[g]:%.2f,%.2f,%.2f\r\n", acc_x, acc_y, acc_z);
        printf("GYRO[deg/s]:%.2f,%.2f,%.2f\r\n", gyro_x, gyro_y, gyro_z);
        printf("MAG[uT]:%.2f,%.2f,%.2f\r\n", magnet_x, magnet_y, magnet_z);


        buff.pbBuffer = i2c_buff;
        buff.wAddr1 = AHRS_SLAVE_ADDRESS << 1;
        buff.wAddr2 = AHRS_REG_CAL_ACC_X_LOW;
        buff.wNumData = sizeof(ahrs_sensor_t);
        I2C1_DevStructure.pCPAL_TransferRx = &buff;
        CPAL_I2C_Read(&I2C1_DevStructure);
    }
    else if (reg == AHRS_REG_CAL_ACC_X_LOW)
    {
        // calibrated sensor data
        sensor = (ahrs_sensor_t *) pDevInitStruct->pCPAL_TransferRx->pbBuffer;



        acc_x = (float)sensor->acc_x * 16.0 / 32767.;
        acc_y = (float)sensor->acc_y * 16.0 / 32767.;
        acc_z = (float)sensor->acc_z * 16.0 / 32767.;

        gyro_x = (float)sensor->gyro_x * 2000. / 32767.;
        gyro_y = (float)sensor->gyro_y * 2000. / 32767.;
        gyro_z = (float)sensor->gyro_z * 2000. / 32767.;

        magnet_x = (float)sensor->magnet_x * 0.3;
        magnet_y = (float)sensor->magnet_y * 0.3;
        magnet_z = (float)sensor->magnet_z * 0.3;

        printf("CAL ACC[g]:%.2f,%.2f,%.2f\r\n", acc_x, acc_y, acc_z);
        printf("CAL GYRO[deg/s]:%.2f,%.2f,%.2f\r\n", gyro_x, gyro_y, gyro_z);
        printf("CAL MAG[uT]:%.2f,%.2f,%.2f\r\n", magnet_x, magnet_y, magnet_z);


        buff.pbBuffer = i2c_buff;
        buff.wAddr1 = AHRS_SLAVE_ADDRESS << 1;
        buff.wAddr2 = AHRS_REG_TEMP_CAL_LOW;
        buff.wNumData = sizeof(ahrs_sensor_t);
        I2C1_DevStructure.pCPAL_TransferRx = &buff;
        CPAL_I2C_Read(&I2C1_DevStructure);
    }
    else if (reg == AHRS_REG_TEMP_CAL_LOW)
    {
        // temperature
        temp_read = (short*) pDevInitStruct->pCPAL_TransferRx->pbBuffer;

        temp = (float)*temp_read * 200. / 32767.;


        printf("Temperature[degC]:%.2f\r\n", temp);


        buff.pbBuffer = i2c_buff;
        buff.wAddr1 = AHRS_SLAVE_ADDRESS << 1;
        buff.wAddr2 = AHRS_REG_ROLL_LOW;
        buff.wNumData = sizeof(ahrs_sensor_t);
        I2C1_DevStructure.pCPAL_TransferRx = &buff;
        CPAL_I2C_Read(&I2C1_DevStructure);
    }
    else if (reg == AHRS_REG_ROLL_LOW)
    {
        // Euler angle
        euler = (ahrs_euler_t*) pDevInitStruct->pCPAL_TransferRx->pbBuffer;

        roll = (float)euler->roll * 180. / 32767.;
        pitch = (float)euler->pitch * 180. / 32767.;
        yaw = (float)euler->yaw * 180. / 32767.;


        printf("R/P/Y[deg]:%.2f,%.2f,%.2f\r\n", roll, pitch, yaw);


        buff.pbBuffer = i2c_buff;
        buff.wAddr1 = AHRS_SLAVE_ADDRESS << 1;
        buff.wAddr2 = AHRS_REG_QUATERNIAN_X_LOW;
        buff.wNumData = sizeof(ahrs_sensor_t);
        I2C1_DevStructure.pCPAL_TransferRx = &buff;
        CPAL_I2C_Read(&I2C1_DevStructure);
    }
    else if (reg == AHRS_REG_QUATERNIAN_X_LOW)
    {
        // Quaternian
        q = (ahrs_quaternian_t*) pDevInitStruct->pCPAL_TransferRx->pbBuffer;

        q_x = (float)q->x / 32767.;
        q_y = (float)q->y / 32767.;
        q_z = (float)q->z / 32767.;
        q_w = (float)q->w / 32767.;


        printf("Quaternian:%.4f,%.4f,%.4f,%.4f\r\n", q_x, q_y, q_z, q_w);


        LED_G_TOGGLE();

    }




}



// CPAL error callback
void CPAL_I2C_ERR_UserCallback(CPAL_DevTypeDef pDevInstance, uint32_t DeviceError)
{
    printf("\r\nCPAL Error. %d\r\n", DeviceError);
}


static int int_cnt;

void EXTI2_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line2);

        // myAHRS+ has 100Hz interrupt output.
        if (int_cnt++ % 10 == 0)
        {
            // read request for raw sensor data(2*9 = 18 byte)
            buff.pbBuffer = i2c_buff;
            buff.wAddr1 = AHRS_SLAVE_ADDRESS << 1;
            buff.wAddr2 = AHRS_REG_ACC_X_LOW;
            buff.wNumData = sizeof(ahrs_sensor_t);
            I2C1_DevStructure.pCPAL_TransferRx = &buff;
            CPAL_I2C_Read(&I2C1_DevStructure);
        }
    }
}




