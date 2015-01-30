//*****************************************************************************
//
// myAHRS+.h - Definitions for myAHRS+ module.
//
// Copyright (c) 2003-2014 Withrobot, Inc.  All rights reserved.
//
// Software License Agreement
//
// Withrobot, Inc.(Withrobot) is supplying this software for use solely and
// exclusively on Withrobot's products.
//
// The software is owned by Withrobot and/or its suppliers, and is protected
// under applicable copyright laws.  All rights are reserved.
// Any use in violation of the foregoing restrictions may subject the user
// to criminal sanctions under applicable laws, as well as to civil liability
// for the breach of the terms and conditions of this license.
//
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// WITHROBOT SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
// OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of myCortex-STM32F4 series examples.
//*****************************************************************************

#ifndef __MYAHRS_PLUS_H__
#define __MYAHRS_PLUS_H__


#define AHRS_SLAVE_ADDRESS              0x20


#define AHRS_REG_WHO_AM_I               0x01
#define AHRS_REG_REV_ID_MAJOR           0x02
#define AHRS_REG_REV_ID_MINOR           0x03
#define AHRS_REG_STATUS                 0x04


#define AHRS_REG_ACC_X_LOW              0x10
#define AHRS_REG_ACC_X_HIGH             0x11
#define AHRS_REG_ACC_Y_LOW              0x12
#define AHRS_REG_ACC_Y_HIGH             0x13
#define AHRS_REG_ACC_Z_LOW              0x14
#define AHRS_REG_ACC_Z_HIGH             0x15
#define AHRS_REG_GYRO_X_LOW             0x16
#define AHRS_REG_GYRO_X_HIGH            0x17
#define AHRS_REG_GYRO_Y_LOW             0x18
#define AHRS_REG_GYRO_Y_HIGH            0x19
#define AHRS_REG_GYRO_Z_LOW             0x1A
#define AHRS_REG_GYRO_Z_HIGH            0x1B
#define AHRS_REG_MAGNET_X_LOW           0x1C
#define AHRS_REG_MAGNET_X_HIGH          0x1D
#define AHRS_REG_MAGNET_Y_LOW           0x1E
#define AHRS_REG_MAGNET_Y_HIGH          0x1F
#define AHRS_REG_MAGNET_Z_LOW           0x20
#define AHRS_REG_MAGNET_Z_HIGH          0x21

#define AHRS_REG_CAL_ACC_X_LOW          0x22
#define AHRS_REG_CAL_ACC_X_HIGH         0x23
#define AHRS_REG_CAL_ACC_Y_LOW          0x24
#define AHRS_REG_CAL_ACC_Y_HIGH         0x25
#define AHRS_REG_CAL_ACC_Z_LOW          0x26
#define AHRS_REG_CAL_ACC_Z_HIGH         0x27
#define AHRS_REG_CAL_GYRO_X_LOW         0x28
#define AHRS_REG_CAL_GYRO_X_HIGH        0x29
#define AHRS_REG_CAL_GYRO_Y_LOW         0x2A
#define AHRS_REG_CAL_GYRO_Y_HIGH        0x2B
#define AHRS_REG_CAL_GYRO_Z_LOW         0x2C
#define AHRS_REG_CAL_GYRO_Z_HIGH        0x2D
#define AHRS_REG_CAL_MAGNET_X_LOW       0x2E
#define AHRS_REG_CAL_MAGNET_X_HIGH      0x2F
#define AHRS_REG_CAL_MAGNET_Y_LOW       0x30
#define AHRS_REG_CAL_MAGNET_Y_HIGH      0x31
#define AHRS_REG_CAL_MAGNET_Z_LOW       0x32
#define AHRS_REG_CAL_MAGNET_Z_HIGH      0x33

#define AHRS_REG_TEMP_CAL_LOW           0x34
#define AHRS_REG_TEMP_CAL_HIGH          0x35

#define AHRS_REG_ROLL_LOW               0x36
#define AHRS_REG_ROLL_HIGH              0x37
#define AHRS_REG_PITCH_LOW              0x38
#define AHRS_REG_PITCH_HIGH             0x39
#define AHRS_REG_YAW_LOW                0x3A
#define AHRS_REG_YAW_HIGH               0x3B

#define AHRS_REG_QUATERNIAN_X_LOW       0x3c
#define AHRS_REG_QUATERNIAN_X_HIGH      0x3d
#define AHRS_REG_QUATERNIAN_Y_LOW       0x3e
#define AHRS_REG_QUATERNIAN_Y_HIGH      0x3f
#define AHRS_REG_QUATERNIAN_Z_LOW       0x40
#define AHRS_REG_QUATERNIAN_Z_HIGH      0x41
#define AHRS_REG_QUATERNIAN_W_LOW       0x42
#define AHRS_REG_QUATERNIAN_W_HIGH      0x43


// WHO_AM_I register value
#define AHRS_WHO_AM_I                   0xB1

// bit mask for STATUS register
#define AHRS_STATUS_READY               0x80



#endif /*__MYAHRS_PLUS_H__*/
