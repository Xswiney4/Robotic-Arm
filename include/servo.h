#ifndef SERVO_H
#define SERVO_H

#include "pca9685.h"
#include <cstdint>  // For uint8_t
#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>

// Parameters
struct ServoParams{

    // PCA9685 Params
    PCA9685* pca9685;
    uint8_t pcaChannel;
    uint8_t pcaPwmFreq;

    // Servo Characteristics
    uint16_t minPulse;
    uint16_t maxPulse;
    float maxAngle;
    float defaultAngle;

    // Rotation Characteristics
    float rotationSpeed;
    float rotationStepFreq;
};

class Servo
{
private:
	// Private Variables
    PCA9685* pca; 		// Pointer to PCA object
    uint8_t pcaChannel; // Channel on PCA
    uint8_t pcaPwmFreq; 	// Frequency the PCA is running at
    static std::mutex pcaMutex; // Mutex for accessing the PCA
    
    // Servo Characteristic Variables
    float maxAngle; // Max angle
    uint16_t minPulse; // Low pulse width value in microseconds
    uint16_t maxPulse; // High pulse width value in microseconds
    float defaultAngle; // Angle the motor will start at, and move to when deconstructed
    uint16_t rotationStepPeriod; // Period the velocity steps trigger at in milliseconds
    
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
	// Constructor / Destructor
    Servo(const ServoParams& params);
    ~Servo();

    // Servo Control (Public)
    std::thread moveToPosition(float angle); // In degrees
    void setSpeed(float speed);		// In radians/second
    
    void disable(); // Disables servo motor
    void enable(); // Enables servo motor

};

#endif
