//*****************************************************************************
//
// myAHRS.h - Definitions for myAHRS module.
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
// This is part of myCortex-LMXXXX series examples.
//*****************************************************************************

#ifndef __MYAHRS_H__
#define __MYAHRS_H__


#define AHRS_SLAVE_ADDRESS              0x1F


#define AHRS_REG_WHO_AM_I               0x01
#define AHRS_REG_REV_ID                 0x02
#define AHRS_REG_STATUS                 0x03
#define AHRS_REG_CONTROL                0x10
#define AHRS_REG_SENSOR_ID              0x12


#define AHRS_REG_ACC_X_LOW              0x60
#define AHRS_REG_ACC_X_HIGH             0x61
#define AHRS_REG_ACC_Y_LOW              0x62
#define AHRS_REG_ACC_Y_HIGH             0x63
#define AHRS_REG_ACC_Z_LOW              0x64
#define AHRS_REG_ACC_Z_HIGH             0x65
#define AHRS_REG_GYRO_X_LOW             0x66
#define AHRS_REG_GYRO_X_HIGH            0x67
#define AHRS_REG_GYRO_Y_LOW             0x68
#define AHRS_REG_GYRO_Y_HIGH            0x69
#define AHRS_REG_GYRO_Z_LOW             0x6A
#define AHRS_REG_GYRO_Z_HIGH            0x6B
#define AHRS_REG_MAGNET_X_LOW           0x6C
#define AHRS_REG_MAGNET_X_HIGH          0x6D
#define AHRS_REG_MAGNET_Y_LOW           0x6E
#define AHRS_REG_MAGNET_Y_HIGH          0x6F
#define AHRS_REG_MAGNET_Z_LOW           0x70
#define AHRS_REG_MAGNET_Z_HIGH          0x71
#define AHRS_REG_ROLL_LOW               0x78
#define AHRS_REG_ROLL_HIGH              0x79
#define AHRS_REG_PITCH_LOW              0x7A
#define AHRS_REG_PITCH_HIGH             0x7B
#define AHRS_REG_YAW_LOW                0x7C
#define AHRS_REG_YAW_HIGH               0x7D
#define AHRS_REG_TEMP_LOW               0x86
#define AHRS_REG_TEMP_HIGH              0x87



// WHO_AM_I register value
#define AHRS_WHO_AM_I                   0xB1

// bit mask for STATUS register
#define AHRS_STATUS_READY               0x80
#define AHRS_STATUS_MAGNET_ERROR        0x08
#define AHRS_STATUS_TEMP_ERROR          0x04
#define AHRS_STATUS_GYRO_ERROR          0x02
#define AHRS_STATUS_ACCEL_ERROR         0x01


// bit mask for CONTROL register
#define AHRS_CONTROL_RESET              0x80
#define AHRS_CONTROL_RSTFACT            0x40
#define AHRS_CONTROL_SAVE               0x10




#endif /*__MYAHRS_H__*/
