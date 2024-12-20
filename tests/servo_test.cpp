#include "pca9685.h"
#include "i2c.h"
#include "servo.h"

#include <string>
#include <cstdint>  // For uint8_t and system
#include <unistd.h>  // For sleep()
#include <iostream>
#include <ostream>

const uint8_t PCA9685_ADDR = 0x40;     		    // Define PCA9685 address with correct type
const std::string I2C_DIRECTORY = "/dev/i2c-1"; // Define I2C directory

const uint8_t PWM_FREQ = 50; // hz
const uint16_t SERVO_1_MIN_PULSE = 540; // microseconds
const uint16_t SERVO_1_MAX_PULSE = 2665; // microseconds
const float SERVO_1_MAX_ANGLE = 262.793; // degrees
const float SERVO_1_DEFAULT_ANGLE = 130; // degrees

const uint16_t SERVO_2_MIN_PULSE = 535; // microseconds
const uint16_t SERVO_2_MAX_PULSE = 2655; // microseconds
const float SERVO_2_MAX_ANGLE = 263.848; // degrees
const float SERVO_2_DEFAULT_ANGLE = 130; // degrees

const float SERVO_SPEED =  45; // degrees/second
const float SERVO_UPDATE_RESOLUTION = 5; // Updates/degree


int main() {
    I2C i2c(I2C_DIRECTORY);                  // Create I2C object
    PCA9685 pca9685(&i2c, PCA9685_ADDR);     // Pass pointer to I2C object and address
    pca9685.setPWMFrequency(PWM_FREQ);

	// Sets up Servo parameters
	ServoParams servo1Params = {&pca9685, 0, SERVO_1_MIN_PULSE, SERVO_1_MAX_PULSE
							 , SERVO_1_MAX_ANGLE, SERVO_1_DEFAULT_ANGLE, SERVO_SPEED, SERVO_UPDATE_RESOLUTION};

	ServoParams servo2Params = {&pca9685, 1, SERVO_2_MIN_PULSE, SERVO_2_MAX_PULSE
							 , SERVO_2_MAX_ANGLE, SERVO_2_DEFAULT_ANGLE, SERVO_SPEED, SERVO_UPDATE_RESOLUTION};
    
	Servo servo1(servo1Params);
	Servo servo2(servo2Params);


	servo1.moveToPosition(0).join();
	servo1.setSpeed(10);
	servo1.moveToPosition(260).join();
	servo1.setSpeed(90);
	servo1.moveToPosition(0).join();

	std::cout << "Terminating Program" << std::endl;
    
    return 0;
}
