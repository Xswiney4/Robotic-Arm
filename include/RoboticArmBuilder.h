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

    
    // Real-time Characterists
    float endSpeed;     // Speed of the end effector

    // Arm Target Position/Orientation
    float targetPitch;
    float targetYaw;
    float targetRoll;
    float targetX;
    float targetY;
    float targetZ;

    // Robotic Variables
    // <DH TABLE>
    // <TRANSFORMATION MATRIX>
    // <JACOBIAN MATRIX>

    // Validation
    bool validateAngle(uint8_t motor, float angle); // Ensure the angle specified is within the limits of the motor

    // Conversion Factors
    static const float DEG_TO_RAD;  // Degrees to radians conversion factor
    static const float RAD_TO_DEG;  // Radians to degrees conversion factor

    // Helper Methods
    float radToDeg(float rad); // Converts radians to degrees
    float degToRad(float deg); // Converts degrees to radians
    void updateJoints(); // Calculates and updates joint angles based on target position/orientation variables

public:
	// Constructor / Destructor
    RoboticArmBuilder(); // Builds arm
    ~RoboticArmBuilder();                      // Resets arm back to default position
    
    // Arm Control
    void setAngle(uint8_t motor, float angle, bool wait = true); // Sets a given motor to an angle
    void setOrientation(float pitch, float yaw, float roll, bool wait = true);      // Sets pitch, yaw, and roll
    void setEndPosition(float x, float y, float z, bool wait = true);               // Sets end effector position

    // Set Arm Characterists
    void setEndSpeed(float speed);  // Sets the target speed of the end effector.

};

#endif
