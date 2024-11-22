// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Libraries ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "servo.h"
#include "pca9685.h"
#include <cmath> // For round()
#include <algorithm>  // For std::clamp
#include <iostream>
#include <ostream>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Constructor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructor: Creates and initializes object
Servo::Servo(PCA9685* pca9685, uint8_t pcaChannel, uint8_t pwmFreq, 
			uint16_t minPulse, uint16_t maxPulse, float maxAngle)
		  : pca(pca9685), pcaChannel(pcaChannel), pwmFreq(pwmFreq)
		  , minPulse(minPulse), maxPulse(maxPulse), maxAngle(maxAngle){
    
    // Preprocessing for angle calculations based on servo motor characteristics
    angleToPwmSlope = ((maxPulse - minPulse) / maxAngle);
    stepSize = 1000000.0f / (4096.0 * pwmFreq);
    
    // Switches channel off
    disable();
    
    // Sets the onTime to 0
    pca -> setOnTime(pcaChannel, 0x0000);
    
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Helper methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Servo Control ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Sets the angle of the servo motor in degrees
void Servo::setAngleDeg(float angle){

	// Clamp angle value
	angle = std::clamp(angle, 0.0f, maxAngle);
	
	// Map angle to pulseWidth
	float pulseWidth = angleToPwmSlope * angle + minPulse;
	
	// Calculate offTime
    uint16_t offTime = static_cast<uint16_t>(round(pulseWidth / stepSize));
    
    // Sets signal PWM signal up
    pca -> setOffTime(pcaChannel, offTime);
    
}

// Sets the speed of the servo motor in radians/second
void Servo::setSpeed(float speed){
	
}

// Disables servo motor
void Servo::disable(){
	pca -> switchOff(pcaChannel);
}
// Enables servo motor
void Servo::enable(){
	pca -> switchOn(pcaChannel);
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
