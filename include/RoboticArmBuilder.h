#ifndef ARM_BUILDER_H
#define ARM_BUILDER_H

#include "i2c.h"
#include "pca9685.h"
#include "servo.h"
#include "config.h"

#include <string>
#include <cstdint>  // For uint8_t

// Position/Orientation Structures
struct Position{
    float x, y, z;
};
struct Orientation{
    float pitch, yaw, roll;
};

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
    Position targetPosition;
    Orientation targetOrientation;

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

    // Start Params
    void initStartVector(); // Sets starting postion/orientation
    
    // Arm Control
    void setAngle(uint8_t motor, float angle, bool wait = true); // Sets a given motor to an angle
    void setEE(Position position, Orientation orientation);

    // Set Arm Characterists
    void setEndSpeed(float speed);  // Sets the target speed of the end effector.

};

#endif
