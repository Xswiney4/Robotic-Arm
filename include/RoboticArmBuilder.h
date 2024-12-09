#ifndef ARM_BUILDER_H
#define ARM_BUILDER_H

#include "i2c.h"
#include "pca9685.h"
#include "servo.h"
#include "ik_solver.h"
#include "config.h"

#include <string>
#include <cstdint>  // For uint8_t

class RoboticArmBuilder
{
private:
	// Objects
    I2C* i2c;            // I2C Object
    PCA9685* pca; 		// PCA object
    
    Servo* servos[6]; // Array containing pointers to all servos
    Servo* j1s;      // Servo controlling Joint 1
    Servo* j2s;      // Servo controlling Joint 2
    Servo* j3s;      // Servo controlling Joint 3
    Servo* j4s;      // Servo controlling Joint 4
    Servo* j5s;      // Servo controlling Joint 5
    Servo* j6s;      // Servo controlling Joint 6
    
    // Real-time Characterists
    float endSpeed;     // Speed of the end effector

    // Robotic Variables
    // <DH TABLE>
    // <TRANSFORMATION MATRIX>
    // <JACOBIAN MATRIX>

    // Validation
    void validateAngle(uint8_t motor, float angle); // Ensure the angle specified is within the limits of the motor

    // Helper Methods

public:
	// Constructor / Destructor
    RoboticArmBuilder(); // Builds arm
    ~RoboticArmBuilder();                      // Resets arm back to default position
    
    // Arm Control
    void setAngle(uint8_t motor, float angle);                  // Sets a given motor to an angle
    void setOrientation(float pitch, float yaw, float roll);    // Sets pitch, yaw, and roll
    void setEndPosition(float x, float y, float z);             // Sets end effector position

    // Set Arm Characterists
    void setEndSpeed(float speed);  // Sets the target speed of the end effector.

};

#endif
