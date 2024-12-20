// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Libraries ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "RoboticArmBuilder.h"
#include "config.h"

#include <vector>
#include <cmath>
#include <iostream>
#include <unistd.h> // For sleep()
#include <thread>
#include <stdexcept>   // For std::runtime_error


// Static constant values
const float RoboticArmBuilder::DEG_TO_RAD = M_PI / 180.0;
const float RoboticArmBuilder::RAD_TO_DEG = 180.0 / M_PI;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Constructor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructor: Creates and initializes object
RoboticArmBuilder::RoboticArmBuilder(){

    targetPitch = -1.0;
    targetYaw = -1.0;
    targetRoll = -1.0;
    targetX = -1.0;
    targetY = -1.0;
    targetZ = -1.0;

    // I2C Construction
    i2c = new I2C(I2C_DIRECTORY);

    // PCA9685 Construction
    pca = new PCA9685(i2c, PCA9685_SLAVE_ADDR);

    // Constructing Servo Parameters
    ServoParams j1sParams = {pca, J1S_CHANNEL, J1S_MIN_PULSE, J1S_MAX_PULSE
							 , J1S_MAX_ANGLE, J1S_DEF_ANGLE, SERVO_SPEED, SERVO_UPDATE_RESOLUTION};
    ServoParams j2sParams = {pca, J2S_CHANNEL, J2S_MIN_PULSE, J2S_MAX_PULSE
							 , J2S_MAX_ANGLE, J2S_DEF_ANGLE, SERVO_SPEED, SERVO_UPDATE_RESOLUTION};
    ServoParams j3sParams = {pca, J3S_CHANNEL, J3S_MIN_PULSE, J3S_MAX_PULSE
							 , J3S_MAX_ANGLE, J3S_DEF_ANGLE, SERVO_SPEED, SERVO_UPDATE_RESOLUTION};
    ServoParams j4sParams = {pca, J4S_CHANNEL, J4S_MIN_PULSE, J4S_MAX_PULSE
							 , J4S_MAX_ANGLE, J4S_DEF_ANGLE, SERVO_SPEED, SERVO_UPDATE_RESOLUTION};
    ServoParams j5sParams = {pca, J5S_CHANNEL, J5S_MIN_PULSE, J5S_MAX_PULSE
							 , J5S_MAX_ANGLE, J5S_DEF_ANGLE, SERVO_SPEED, SERVO_UPDATE_RESOLUTION};
    ServoParams j6sParams = {pca, J6S_CHANNEL, J6S_MIN_PULSE, J6S_MAX_PULSE
							 , J6S_MAX_ANGLE, J6S_DEF_ANGLE, SERVO_SPEED, SERVO_UPDATE_RESOLUTION};

    // Create Servo objects
    servos[0] = new Servo(j1sParams);
    servos[1] = new Servo(j2sParams);
    servos[2] = new Servo(j3sParams);
    servos[3] = new Servo(j4sParams);
    servos[4] = new Servo(j5sParams);
    servos[5] = new Servo(j6sParams);


}

// Deconstructor: Cleans up objects, sets arm to default position
RoboticArmBuilder::~RoboticArmBuilder(){
    sleep(1);
    for (int i = 0; i < 6; i++){
        delete servos[i];
    }
    delete pca;
    delete i2c;
    sleep(1);

}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Validation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Ensure the angle specified is within the limits of the motor
bool RoboticArmBuilder::validateAngle(uint8_t motor, float angle){
    return servos[motor]->isAngleValid(angle);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Helper Methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Converts radians to degrees
float RoboticArmBuilder::radToDeg(float rad){
    return rad * RAD_TO_DEG;
}

// Converts degrees to radians
float RoboticArmBuilder::degToRad(float deg){
    return deg * DEG_TO_RAD;
}

// Calculates and updates joint angles based on target position/orientation variables
void RoboticArmBuilder::updateJoints(){

    // ~~ Calculate RDesired ~~
    // Precompute cosines and sines
    float cYaw = cos(degToRad(targetYaw));
    float sYaw = sin(degToRad(targetYaw));
    float cPitch = cos(degToRad(targetPitch));
    float sPitch = sin(degToRad(targetPitch));
    //float cRoll = cos(degToRad(targetRoll));
    //float sRoll = sin(degToRad(targetRoll));

    // Z-axis of the end effector
    std::vector<float> zEffector = {
        cYaw * sPitch,
        sYaw * sPitch,
        cPitch
    };

    // X-axis of the end effector
    std::vector<float> xEffector = {
        cYaw * cPitch,
        sYaw * cPitch,
        -sPitch
    };

    // Y-axis of the end effector
    std::vector<float> yEffector = {
        zEffector[1] * xEffector[2] - zEffector[2] * xEffector[1],
        zEffector[2] * xEffector[0] - zEffector[0] * xEffector[2],
        zEffector[0] * xEffector[1] - zEffector[1] * xEffector[0]
    };

    // Assemble RDesired matrix using vectors
    std::vector<std::vector<float>> RDesired(3, std::vector<float>(3));
    RDesired[0] = xEffector;
    RDesired[1] = yEffector;
    RDesired[2] = zEffector;

    // ~~ Calculate wrist center ~~

    float wcX = targetX - D_6 * RDesired[0][2];
    float wcY = targetY - D_6 * RDesired[1][2];
    float wcZ = targetZ - D_6 * RDesired[2][2] - D_1;

    // ~~ Calculate Theta 1, 2, and 3 ~~

    // Theta_1
    float theta1 = atan2(wcY, wcX);

    // r is the projection onto the XY plane
    float r = sqrt(wcX * wcX + wcY * wcY);

    // Distance from joint 2 to wrist center
    float s = sqrt(r * r + wcZ * wcZ);

    // Using the law of cosines for theta3
    float cosTheta3 = (s * s - A_2 * A_2 - A_3 * A_3) / (2 * A_2 * A_3);
    float theta3 = atan2(sqrt(1 - cosTheta3 * cosTheta3), cosTheta3); // elbow up

    // Theta2
    float alpha = atan2(wcZ, r); // angle to wrist center
    float beta = atan2(A_3 * sin(theta3), A_2 + A_3 * cos(theta3));
    float theta2 = alpha - beta;

    // ~~ Calculate Theta 4, 5, and 6 ~~
    
    float theta4 = atan2(RDesired[1][2], RDesired[0][2]);
    float theta5 = atan2(sqrt(RDesired[0][2]*RDesired[0][2] + RDesired[1][2]*RDesired[1][2]), RDesired[2][2]);
    float theta6 = atan2(RDesired[1][1], RDesired[1][0]);

    std::cout << "Theta 1: " << radToDeg(theta1) << std::endl;
    std::cout << "Theta 2: " << radToDeg(theta2) << std::endl;
    std::cout << "Theta 3: " << radToDeg(theta3) << std::endl;
    std::cout << "Theta 4: " << radToDeg(theta4) << std::endl;
    std::cout << "Theta 5: " << radToDeg(theta5) << std::endl;
    std::cout << "Theta 6: " << radToDeg(theta6) << std::endl;
    //std::cout << "Wrist Center Position: (" << wcX << ", " << wcY << ", " << wcZ << ")" << std::endl;

    std::thread t1 = servos[0]->moveToPosition(radToDeg(theta1) + J1S_DEF_ANGLE);
    std::thread t2 = servos[1]->moveToPosition(radToDeg(theta2) + 90.0 + J2S_DEF_ANGLE);
    std::thread t3 = servos[2]->moveToPosition(radToDeg(theta3) + J3S_DEF_ANGLE);
    std::thread t4 = servos[3]->moveToPosition(radToDeg(theta4) + J4S_DEF_ANGLE);
    std::thread t5 = servos[4]->moveToPosition(radToDeg(theta5) + J5S_DEF_ANGLE);
    servos[5]->moveToPosition(radToDeg(theta6) + J6S_DEF_ANGLE).join();

    if(t1.joinable()){t1.join();} else{t1.detach();}
    if(t2.joinable()){t2.join();} else{t2.detach();}
    if(t3.joinable()){t3.join();} else{t3.detach();}
    if(t4.joinable()){t4.join();} else{t4.detach();}
    if(t5.joinable()){t5.join();} else{t5.detach();}

}

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
    
    // Set global variables
    targetPitch = pitch;
    targetYaw = yaw;
    targetRoll = roll;

    if (targetX == -1.0){
        std::cout << "Target position undefined, orientation defined" << std::endl;
        return;
    }

    // Update Joints
    updateJoints();
}

/* Uses inverse kinematics to calculate and set the motors so that the end effector
 * touches the given x, y, and z position.
 */
void RoboticArmBuilder::setEndPosition(float x, float y, float z, bool wait){
    
    if (targetYaw == -1.0){
        std::cout << "Please define orientation first" << std::endl;
        return;
    }

    // Set global variables
    targetX = x;
    targetY = y;
    targetZ = z;

    // Update Joints
    updateJoints();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Set Arm Characterists ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Sets the target speed of the end effector.
void RoboticArmBuilder::setEndSpeed(float speed){
    this->endSpeed = speed;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~