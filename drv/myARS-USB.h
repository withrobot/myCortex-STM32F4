//*****************************************************************************
//
// myARS-USB.h - Definitions for myARS-USB module.
//
// Copyright (c) 2003-2013 Withrobot, Inc.  All rights reserved.
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

#ifndef __MYARS_USB_H__
#define __MYARS_USB_H__


#define ARS_SLAVE_ADDRESS               0x1C


#define ARS_REG_WHO_AM_I                0x12
#define ARS_REG_STATUS                  0x13
#define ARS_REG_REV_ID                  0x14
#define ARS_REG_CONTROL                 0x15

#define ARS_REG_ACC_X_OFFSET_LOW        0x1A
#define ARS_REG_ACC_X_OFFSET_HIGH       0x1B
#define ARS_REG_ACC_Y_OFFSET_LOW        0x1C
#define ARS_REG_ACC_Y_OFFSET_HIGH       0x1D
#define ARS_REG_ACC_Z_OFFSET_LOW        0x1E
#define ARS_REG_ACC_Z_OFFSET_HIGH       0x1F
#define ARS_REG_GYRO_X_OFFSET_LOW       0x20
#define ARS_REG_GYRO_X_OFFSET_HIGH      0x21
#define ARS_REG_GYRO_Y_OFFSET_LOW       0x22
#define ARS_REG_GYRO_Y_OFFSET_HIGH      0x23

#define ARS_REG_ACC_X_LOW               0x32
#define ARS_REG_ACC_X_HIGH              0x33
#define ARS_REG_ACC_Y_LOW               0x34
#define ARS_REG_ACC_Y_HIGH              0x35
#define ARS_REG_ACC_Z_LOW               0x36
#define ARS_REG_ACC_Z_HIGH              0x37
#define ARS_REG_GYRO_X_LOW              0x38
#define ARS_REG_GYRO_X_HIGH             0x39
#define ARS_REG_GYRO_Y_LOW              0x3A
#define ARS_REG_GYRO_Y_HIGH             0x3B
#define ARS_REG_TEMP_LOW                0x3C
#define ARS_REG_TEMP_HIGH               0x3D
#define ARS_REG_ROLL_LOW                0x3E
#define ARS_REG_ROLL_HIGH               0x3F
#define ARS_REG_PITCH_LOW               0x40
#define ARS_REG_PITCH_HIGH              0x41




// bit mask for STATUS register
#define ARS_STATUS_CAL                  0x20

// bit mask for CONTROL register
#define ARS_CONTROL_RST                 0x80
#define ARS_CONTROL_SAVE                0x40
#define ARS_CONTROL_RSTFCT              0x10
#define ARS_CONTROL_UART                0x08
#define ARS_CONTROL_USB                 0x04
#define ARS_CONTROL_CAL                 0x02
#define ARS_CONTROL_LED                 0x01



#endif /*__MYARS_USB_H__*/
