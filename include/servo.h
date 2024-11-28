#ifndef SERVO_H
#define SERVO_H

#include "pca9685.h"
#include <cstdint>  // For uint8_t
#include <atomic>
#include <thread>
#include <chrono>

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
    uint16_t stepPeriod; // Period the velocity steps trigger at in milliseconds
    
    // Preprocessed variables
    float angleToPwmSlope; // Preprocessed slop for calculating pulse width
    float stepSize; // Preprocessed step size for calculating on/off times
    
    // Real-Time Characteristics
    float targetAngle;
    float currentAngle;
    float rotationSpeed; // Degrees / Second
    
    // Velocity Control
    bool running;
    bool clockwise; // True if moving clockwise
    std::chrono::time_point<std::chrono::steady_clock> startTime;

    void velocityControlThread();

    // Helper Methods
    void step(); // Takes a step towards the target position

    // Servo Control (Private)
    void setPosition(float angle);	// In degrees

public:
	// Constructor
    Servo(PCA9685* pca9685, uint8_t pcaChannel, uint8_t pwmFreq ,uint16_t minPulse, uint16_t maxPulse, float maxAngle, float rotationSpeed, float stepFreq);
    
    // Servo Control (Public)
    void moveToPosition(float angle); // In degrees
    void setSpeed(float speed);		// In radians/second
    
    void disable(); // Disables servo motor
    void enable(); // Enables servo motor
    
};

#endif
