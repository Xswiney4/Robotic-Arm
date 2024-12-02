#ifndef ARM_BUILDER_H
#define ARM_BUILDER_H

#include "i2c.h"
#include "pca9685.h"
#include "servo.h"
#include "ik_solver.h"

#include <string>
#include <cstdint>  // For uint8_t

class RoboticArmBuilder
{
private:
	// Objects
    I2C* i2c;            // I2C Object
    PCA9685* pca; 		// PCA object
    
    Servo* servo_1;      // Servo controlling Joint 1
    Servo* servo_2;      // Servo controlling Joint 2
    Servo* servo_3;      // Servo controlling Joint 3
    Servo* servo_4;      // Servo controlling Joint 4
    Servo* servo_5;      // Servo controlling Joint 5
    Servo* servo_6;      // Servo controlling Joint 6
    
    // Real-time Characterists
    float endSpeed;     // Speed of the end effector

    // Validation
    void validateAngle(uint8_t motor, float angle); // Ensure the angle specified is within the limits of the motor

    // Helper Methods

public:
	// Constructor / Destructor
    RoboticArmBuilder(std::string configPath); // Builds arm
    ~RoboticArmBuilder();                      // Resets arm back to default position
    
    // Arm Control
    void setAngle(uint8_t motor, float angle);                  // Sets a given motor to an angle
    void setOrientation(float pitch, float yaw, float roll);    // Sets pitch, yaw, and roll
    void setEndPosition(float x, float y, float z);             // Sets end effector position

    // Set Arm Characterists
    void setEndSpeed(float speed);  // Sets the target speed of the end effector.

};

#endif
