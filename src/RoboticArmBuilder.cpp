// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Libraries ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "RoboticArmBuilder.h"

#include <stdexcept>   // For std::runtime_error

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Constructor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructor: Creates and initializes object
RoboticArmBuilder::RoboticArmBuilder(){

    // I2C Construction
    i2c = new I2C(I2C_DIRECTORY);

    // PCA9685 Construction
    pca = new PCA9685(i2c, PCA9685_SLAVE_ADDR);

    // Constructing Servo Parameters
    ServoParams j1sParams = {pca, J1S_CHANNEL, J1S_MIN_PULSE, J1S_MAX_PULSE
							 , J1S_MAX_ANGLE, J1S_DEF_ANGLE, SERVO_SPEED, SERVO_STEP_RATE};
    ServoParams j2sParams = {pca, J2S_CHANNEL, J2S_MIN_PULSE, J2S_MAX_PULSE
							 , J2S_MAX_ANGLE, J2S_DEF_ANGLE, SERVO_SPEED, SERVO_STEP_RATE};
    ServoParams j3sParams = {pca, J3S_CHANNEL, J3S_MIN_PULSE, J3S_MAX_PULSE
							 , J3S_MAX_ANGLE, J3S_DEF_ANGLE, SERVO_SPEED, SERVO_STEP_RATE};
    ServoParams j4sParams = {pca, J4S_CHANNEL, J4S_MIN_PULSE, J4S_MAX_PULSE
							 , J4S_MAX_ANGLE, J4S_DEF_ANGLE, SERVO_SPEED, SERVO_STEP_RATE};
    ServoParams j5sParams = {pca, J5S_CHANNEL, J5S_MIN_PULSE, J5S_MAX_PULSE
							 , J5S_MAX_ANGLE, J5S_DEF_ANGLE, SERVO_SPEED, SERVO_STEP_RATE};
    ServoParams j6sParams = {pca, J6S_CHANNEL, J6S_MIN_PULSE, J6S_MAX_PULSE
							 , J6S_MAX_ANGLE, J6S_DEF_ANGLE, SERVO_SPEED, SERVO_STEP_RATE};

    // Create Servo objects
    servos[0] = new Servo(j1sParams);
    servos[1] = new Servo(j2sParams);
    servos[2] = new Servo(j3sParams);
    servos[3] = new Servo(j4sParams);
    servos[4] = new Servo(j5sParams);
    servos[5] = new Servo(j6sParams);

    // Inverse Kinematics Init


}

// Deconstructor: Cleans up objects, sets arm to default position
RoboticArmBuilder::~RoboticArmBuilder(){
    for (int i = 0; i < 6; i++){
        delete servos[i];
    }
    delete pca;
    delete i2c;

}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Validation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Ensure the angle specified is within the limits of the motor
bool RoboticArmBuilder::validateAngle(uint8_t motor, float angle){
    return servos[motor]->isAngleValid(angle);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Helper Methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Arm Control ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Sets the angle of a given motor
void RoboticArmBuilder::setAngle(uint8_t motor, float angle, bool wait){
    
    // Throws error if angle is not in servo's range
    if(!validateAngle(motor, angle)){
        throw std::runtime_error("Angle is not within servo's range");
    }
    
    // Waits until the motors stops moving before moving on
    if(wait){
        servos[motor]->moveToPosition(angle).join();
    }
    // Moves on without waiting for the servo to stop
    else{
        servos[motor]->moveToPosition(angle).detach();
    }
}

// Sets the orientation of the last 3 joints in terms of pitch, yaw, and roll
void RoboticArmBuilder::setOrientation(float pitch, float yaw, float roll, bool wait){
    
    // Throws error if angle is not in servo's range
    if(!validateAngle(3, pitch)){
        throw std::runtime_error("Pitch is not within servo's range");
    }
    else if(!validateAngle(4, yaw)){
        throw std::runtime_error("Yaw is not within servo's range");
    }
    else if(!validateAngle(5, roll)){
        throw std::runtime_error("Roll is not within servo's range");
    }
    
    // Waits until the motors stops moving before moving on
    if(wait){
        servos[5]->moveToPosition(roll).detach();
        servos[4]->moveToPosition(yaw).detach();
        servos[3]->moveToPosition(pitch).join();
    }
    // Moves on without waiting for the servo to stop
    else{
        servos[5]->moveToPosition(roll).detach();
        servos[4]->moveToPosition(yaw).detach();
        servos[3]->moveToPosition(pitch).detach();
    }
}

/* Uses inverse kinematics to calculate and set the motors so that the end effector
 * touches the given x, y, and z position.
 */
void RoboticArmBuilder::setEndPosition(float x, float y, float z, bool wait){
    if(wait){

    }
    else{
        
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Set Arm Characterists ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Sets the target speed of the end effector.
void RoboticArmBuilder::setEndSpeed(float speed){
    this->endSpeed = speed;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~