// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Libraries ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "servo.h"
#include "pca9685.h"
#include <cmath> // For round()
#include <algorithm>  // For std::clamp
#include <iostream>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Constructor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructor: Creates and initializes object
Servo::Servo(PCA9685* pca9685, uint8_t pcaChannel, uint8_t pwmFreq, 
			uint16_t minPulse, uint16_t maxPulse, float maxAngle, float rotationSpeed, float stepFreq)
		  : pca(pca9685), pcaChannel(pcaChannel), pwmFreq(pwmFreq)
		  , minPulse(minPulse), maxPulse(maxPulse), maxAngle(maxAngle)
          , rotationSpeed(rotationSpeed), running(false){
    
    // Preprocessing for angle calculations based on servo motor characteristics
    angleToPwmSlope = ((maxPulse - minPulse) / maxAngle);
    stepSize = 1000000.0f / (4096.0 * pwmFreq);
    stepPeriod = 1000.0f / stepFreq; // In ms
    
    // Switches channel off
    disable();
    
    // Sets the onTime to 0
    pca -> setOnTime(pcaChannel, 0x0000);
    
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Thread Method ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Thread used to control movement toward the target position
void Servo::velocityControlThread(){
    while(currentAngle != targetAngle){

        // Takes a step towards the target
        step();

        // Pause this thread for (stepPeriod)ms 
         std::this_thread::sleep_for(std::chrono::milliseconds(stepPeriod));

    }
    running = false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Helper Methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Takes one step towards the target position by calculating the delta time
void Servo::step(){

    // Calculates the amount of time passed
    auto endTime = std::chrono::steady_clock::now();
    auto deltaTime = endTime - startTime;
    startTime = endTime;

    // Calculates new angle based on direction
    if(clockwise){
        currentAngle = std::clamp(currentAngle + std::chrono::duration_cast<std::chrono::duration<float>>(deltaTime).count() * rotationSpeed,0.0f,targetAngle);
    }
    else{
        currentAngle = std::clamp(currentAngle - std::chrono::duration_cast<std::chrono::duration<float>>(deltaTime).count() * rotationSpeed,targetAngle,maxAngle);
    }

    // Sets the angle
    this->setPosition(currentAngle);

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Servo Control ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Sets the angle of the servo motor in degrees (Private)
void Servo::setPosition(float angle){

	// Clamp angle value
	angle = std::clamp(angle, 0.0f, maxAngle);

    std::cout << "Setting angle to " << angle << std::endl;
	
	// Map angle to pulseWidth
	float pulseWidth = angleToPwmSlope * angle + minPulse;
	
	// Calculate offTime
    uint16_t offTime = static_cast<uint16_t>(round(pulseWidth / stepSize));
    
    // Sets signal PWM signal up
    pca -> setOffTime(pcaChannel, offTime);
    
}

// Moves the servo position smoothly towards the input angle (Public)
void Servo::moveToPosition(float angle){
    
    // This prevents two threads from attempting to control the servo at once.
    if(running){
        printf("Thread is already running, please wait until it's finished");
        return;
    }

    // Sets global
    targetAngle = angle;
    running = true;
    startTime = std::chrono::steady_clock::now();
    if(targetAngle < currentAngle){ // CCW
        clockwise = false;
    }
    else{ // CW
        clockwise = true;
    }

    // Creates thread
    std::thread t(&Servo::velocityControlThread, this);

    t.detach();
    


}

// Sets the speed of the servo motor in radians/second
void Servo::setSpeed(float speed){
	rotationSpeed = speed;
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
