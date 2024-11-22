#ifndef SERVO_H
#define SERVO_H

#include "pca9685.h"
#include <cstdint>  // For uint8_t

class Servo
{
private:
	// Private Variables
    PCA9685* pca; 		// Pointer to PCA object
    uint8_t pcaChannel; // Channel on PCA
    uint8_t pwmFreq; 	// Frequency the PCA is running at
    
    // Servo Characteristic Variables
    float maxAngle; // Max angle
    uint16_t minPulse; // Low pulse width value in microseconds
    uint16_t maxPulse; // High pulse width value in microseconds
    
    // Preprocessed variables
    float angleToPwmSlope; // Preprocessed slop for calculating pulse width
    float stepSize; // Preprocessed step size for calculating on/off times
    
    // Real-Time Characteristics
    float targetAngle;
    float rotationSpeed;
    
    // Helper methods
    

public:
	// Constructor
    Servo(PCA9685* pca9685, uint8_t pcaChannel, uint8_t pwmFreq ,uint16_t minPulse, uint16_t maxPulse, float maxAngle);
    
    // Servo Control
    void setAngleDeg(float angle);	// In degrees
    void setSpeed(float speed);		// In radians/second
    
    void disable(); // Disables servo motor
    void enable(); // Enables servo motor
    
};

#endif
