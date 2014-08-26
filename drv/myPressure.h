//*****************************************************************************
//
// myPressure.h - Definitions for myPressure module.
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

#ifndef __MYPRESSURE_H__
#define __MYPRESSURE_H__


#define PRESSURE_SLAVE_ADDRESS          0x1D


#define PRESSURE_REG_WHO_AM_I           0x01
#define PRESSURE_REG_REV_ID             0x02
#define PRESSURE_REG_STATUS             0x03
#define PRESSURE_REG_CONTROL            0x10
#define PRESSURE_REG_SENSOR_ID          0x12


#define PRESSURE_REG_TEMP_LOW           0x86
#define PRESSURE_REG_TEMP_HIGH          0x87
#define PRESSURE_REG_PRESSURE_0         0x88
#define PRESSURE_REG_PRESSURE_1         0x89
#define PRESSURE_REG_PRESSURE_2         0x8A
#define PRESSURE_REG_PRESSURE_3         0x8B
#define PRESSURE_REG_ALTITUDE_0         0x8C
#define PRESSURE_REG_ALTITUDE_1         0x8D
#define PRESSURE_REG_ALTITUDE_2         0x8E
#define PRESSURE_REG_ALTITUDE_3         0x8F




// WHO_AM_I register value
#define PRESSURE_WHO_AM_I               0xB1

// bit mask for STATUS register
#define PRESSURE_STATUS_READY           0x80
#define PRESSURE_STATUS_TEMP_ERROR      0x04
#define PRESSURE_STATUS_BAROMETER_ERROR 0x01


// bit mask for CONTROL register
#define PRESSURE_CONTROL_RESET          0x80
#define PRESSURE_CONTROL_RSTFACT        0x40
#define PRESSURE_CONTROL_SAVE           0x10




#endif /*__MYPRESSURE_H__*/
