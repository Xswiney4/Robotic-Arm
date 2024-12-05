/*
~~ Servo Calibration ~~

This calibrator measures the actual angle of a servo motor as we move throughout the PWM range. 
The following happens when run:
- Servo is set to the midpoint between the specified range
- PWM pulse is incrementally moved downward, monitoring the angle along the way.
- Once the angle no longer changes, we move back into the working range, and then move in finer increments
  until the motor no longer moves. The PWM signal before it stopped is recorded as the new MIN_PULSE
- AS5600 MUST BE ZERO'D AT NEW MIN_PULSE
- Then the motor moves to the other end of the range, performing the same steps as before to determine
  the other end of the PWM range; MAX_PULSE
calibratePwm():
- pwmOffset and pwmMultiplier are calculated using the new min and max pulses
calibrateError():
- The servo starts at one end of the SPECIFIED range and works it's way towards the other end of the SPECIFIED
  range, taking samples of the actual angle, and then comparing it to the supposed angle, to get the error.
- Then we create an error function, and then pull the dcOffset and dcMultiplier out of that.



*/

// Libraries
#include "pca9685.h"
#include "as5600.h"
#include "i2c.h"
#include "servo.h"

#include <cstdint>  // For uint8_t and system

// I2C Config
const std::string I2C_DIRECTORY = "/dev/i2c-1"; // Define I2C directory

// PCA9685 Config
const uint8_t PCA9685_ADDR = 0x40;  // Define PCA9685 address
const uint8_t PCA9685_FREQ = 50;    // hz
const uint8_t CHANNEL = 0;          // Channel the servo will be plugged into

// Servo Config ~~~~~ ADJUST BEFORE CALIBRATING
const uint16_t SERVO_MIN_PULSE = 500;     // microseconds
const uint16_t SERVO_MAX_PULSE = 2500;    // microseconds
const float SERVO_MAX_ANGLE = 270;        // degrees
const float SERVO_DEFAULT_ANGLE = 135;    // degrees
const float SERVO_SPEED =  45;            // degrees/second
const float SERVO_STEP_FREQ = 50;         // hz

// AS5600 Config
const uint16_t AS5600_CONFIG = 0x0000;           // Config of AS5600


// Method Headers
void calibrateEnds();   // 
void calibratePwm();    // Calculates pwmOffset and pwmMultiplier. This creates a log file servo_calibration_PWM.txt
void calibrateError();  // Calculates dcOffset and dcMultiplier. This creates a log file



int main() {

    // Object Building
    I2C i2c(I2C_DIRECTORY);                  // Create I2C object
    PCA9685 pca9685(&i2c, PCA9685_ADDR);     // Pass pointer to I2C object and address
    pca9685.setPWMFrequency(PCA9685_FREQ);   // Set output frequency of PWM signal

    ServoParams servoParams = {&pca9685, CHANNEL, SERVO_MIN_PULSE, SERVO_MAX_PULSE
							 , SERVO_MAX_ANGLE, SERVO_DEFAULT_ANGLE, SERVO_SPEED, SERVO_STEP_FREQ};
    
    Servo servo(servoParams);
    
    return 0;
}