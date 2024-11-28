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
const uint16_t SERVO_1_MIN_PULSE = 500; // microseconds
const uint16_t SERVO_1_MAX_PULSE = 2500; // microseconds
const float SERVO_1_MAX_ANGLE = 270; // degrees

const float SERVO_1_SPEED =  90; // degrees/second
const float SERVO_1_STEP_FREQ = 50; // Hz


int main() {
    I2C i2c(I2C_DIRECTORY);                  // Create I2C object
    PCA9685 pca9685(&i2c, PCA9685_ADDR);     // Pass pointer to I2C object and address
    pca9685.setPWMFrequency(PWM_FREQ);
    
	Servo servo1(&pca9685, 1, PWM_FREQ, SERVO_1_MIN_PULSE, SERVO_1_MAX_PULSE, SERVO_1_MAX_ANGLE, SERVO_1_SPEED, SERVO_1_STEP_FREQ);
	
	servo1.enable();
	
	std::cout << "Setting angle to 270..." << std::endl;
	servo1.moveToPosition(270);
	sleep(10);
	std::cout << "Setting angle to 270..." << std::endl;
	servo1.moveToPosition(100);
	sleep(10);
	std::cout << "Setting angle to 270..." << std::endl;
	servo1.moveToPosition(163.3);
    
    while (true){
		sleep(1);
	}
}
