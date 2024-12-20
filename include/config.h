#ifndef ARM_CONFIG_H
#define ARM_CONFIG_H

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Robotic Arm Config ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Robotic Arm Physical Parameters
#define D_6 55.0 // Length of link 6
#define D_1 9.5 // Offset along Z1
#define A_2 39.0 // Length of link 2
#define A_3 150.0 // Length of link 3

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Device Parameters ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// I2C Params
#define I2C_DIRECTORY "/dev/i2c-1"

// PCA9865 Parms
#define PCA9685_SLAVE_ADDR 0x40  // Slave address
#define PCA9685_FREQ 50    // hz

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Servo Parameters ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Global Servo Params
#define SERVO_UPDATE_RESOLUTION 5 // updates/degree (servo smoothness)
#define SERVO_SPEED 90 // deg/sec

// Joint 1 Servo Params
#define J1S_CHANNEL 0
#define J1S_MIN_PULSE 540 // microseconds
#define J1S_MAX_PULSE 2665 // microseconds
#define J1S_MAX_ANGLE 262.793 // degrees
#define J1S_DEF_ANGLE 130 // degrees

// Joint 2 Servo Params
#define J2S_CHANNEL 1
#define J2S_MIN_PULSE 535 // microseconds
#define J2S_MAX_PULSE 2655 // microseconds
#define J2S_MAX_ANGLE 263.848 // degrees
#define J2S_DEF_ANGLE 131 // degrees

// Joint 3 Servo Params
#define J3S_CHANNEL 4
#define J3S_MIN_PULSE 400 // microseconds
#define J3S_MAX_PULSE 2795 // microseconds
#define J3S_MAX_ANGLE 296.367 // degrees
#define J3S_DEF_ANGLE 136 // degrees

// Joint 4 Servo Params
#define J4S_CHANNEL 5
#define J4S_MIN_PULSE 400 // microseconds
#define J4S_MAX_PULSE 2790 // microseconds
#define J4S_MAX_ANGLE 295.4 // degrees
#define J4S_DEF_ANGLE 140 // degrees

// Joint 5 Servo Params
#define J5S_CHANNEL 6
#define J5S_MIN_PULSE 395 // microseconds
#define J5S_MAX_PULSE 2780 // microseconds
#define J5S_MAX_ANGLE 296.719 // degrees
#define J5S_DEF_ANGLE 134 // degrees

// Joint 6 Servo Params
#define J6S_CHANNEL 8
#define J6S_MIN_PULSE 1000 // microseconds
#define J6S_MAX_PULSE 2000 // microseconds
#define J6S_MAX_ANGLE 79.8926 // degrees
#define J6S_DEF_ANGLE 40 // degrees


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif