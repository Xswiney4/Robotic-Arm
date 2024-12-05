// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Libraries ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "servo.h"
#include <cmath> // For round()
#include <algorithm>  // For std::clamp
#include <iostream>
#include <unistd.h> // For sleep()

// Initialize static mutex
std::mutex Servo::pcaMutex;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Constructor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructor: Creates and initializes object
Servo::Servo(const ServoParams& params)
		  : pca(params.pca9685), pcaChannel(params.pcaChannel)
          , minPulse(params.minPulse), maxPulse(params.maxPulse)
          , maxAngle(params.maxAngle), defaultAngle(params.defaultAngle)
          , currentAngle(params.defaultAngle)
          , rotationSpeed(params.rotationSpeed), running(false){
    
    // Preprocessing for angle calculations based on servo motor characteristics
    angleToPwmSlope = ((maxPulse - minPulse) / maxAngle);
    rotationStepPeriod = 1000.0f / params.rotationStepFreq; // In ms
    
    // Switches channel off
    disable();
    
    // Sets the onTime to 0
    pca -> setOnTime(pcaChannel, 0x0000);
    
}

Servo::~Servo(){

    // Move to default position
    this->moveToPosition(defaultAngle).join();

}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Thread Method ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Thread used to control movement toward the target position
void Servo::velocityControlThread(){
    while(true){

        // This mutex prevents multiple threads from communicating with the PCA at the same time
        std::unique_lock<std::mutex> lock(pcaMutex);

        // Takes a step towards the target
        step();

        // Unlock mutex
        lock.unlock();

        // Checks to see if we've reached the target
        if(currentAngle == targetAngle){
            break;
        }
        else{
            // Pause this thread for (rotationStepPeriod)ms 
            std::this_thread::sleep_for(std::chrono::milliseconds(rotationStepPeriod));
        }
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

    std::cout << "pulseWidth: " << pulseWidth << std::endl;

    // Sets signal PWM signal up
    pca -> setPulseWidth(pcaChannel, pulseWidth);
    
}

/* Moves the servo position smoothly towards the input angle (Public)
 * The thread needs to be either joined or detached after running this function
 * moveToPosition().join() -> Start thread and wait until the motion is complete
 * moveToPostion().detach() -> Start thread and run in background
 */
std::thread Servo::moveToPosition(float angle){

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
    std::thread workingThread(&Servo::velocityControlThread, this);
    
    return workingThread;

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