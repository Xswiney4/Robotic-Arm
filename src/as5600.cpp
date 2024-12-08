// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Libraries ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "as5600.h"
#include "i2c.h"

#include <string>
#include <stdexcept> // For std::runtime_error
#include <iostream>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Constructor/Destructor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructor: Creates and initializes object
AS5600::AS5600(I2C *i2cPtr, uint16_t config) : i2c(i2cPtr), address(AS5600_ADDRESS){

    // Attempts to register PCA9685 into i2c object, returns error if it fails
    if(!i2c->registerSlave(address)){
        throw std::runtime_error("Failed to add AS5600 to i2c");
    }

    initConfig(config);
    checkMagnet();
    zero();

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Config ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void AS5600::initConfig(uint16_t config){

    // Seperates config into bytes
    uint8_t msb = config >> 8;
    uint8_t lsb = config & 0x0011;

    // Write config to AS5600
    uint8_t buffer[3] = {REG_CONF_MSB, msb, lsb};
    if(!i2c -> write(address, buffer, 3)){
        throw std::runtime_error("Failed to write config to AS5600");
    }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Helper methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Writes a value to a register
void AS5600::writeReg(uint8_t reg, uint8_t value){
    // Creates buffer
    uint8_t buffer[2] = {reg, value};
    
    // Sends two bytes to PCA containing register and value to update, throws error if failure
    if(!i2c -> write(address, buffer, 2)){
        throw std::runtime_error("Failed to write value to register");
    }
}

// Reads a value from a register
uint8_t AS5600::readReg(uint8_t reg){
    
    // Sends one bytes to PCA containing register to set for reading, throws error if failure
    if(!i2c -> write(address, &reg, 1)){
        throw std::runtime_error("Failed to setup register for reading");
    }
    
    std::vector<uint8_t> buffer = i2c -> read(address, 1);
    return buffer[0];
    
}

// Modifies specific bits in a register without overwriting the entire register.
void AS5600::modifyReg(uint8_t reg, uint8_t mask, uint8_t value){
    // Get register contents
    uint8_t prevByte = readReg(reg);
    
    // Clear the bits specified by mask, then set those values.
    uint8_t modifiedByte = (prevByte & ~mask) | (value & mask);
    
    // Write the modifed byte to that register.
    writeReg(reg, modifiedByte);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Status ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Check the status of the magnet, throws error if magnet has issue
void AS5600::checkMagnet(){

    // Get status register from device
    uint8_t status = readReg(REG_MAGNET_STATUS);

    std::cout << "Status Register: " << std::to_string(status) << std::endl;

    // Seperate status indicators
    uint8_t md = (status >> 5) & 0x01;
    uint8_t ml = (status >> 4) & 0x01;
    uint8_t mh = (status >> 3) & 0x01;

    std::string error = "";
    // AGC minimum gain overflow, magnet too strong
    if (mh == 1){
        error += "AGC minimum gain overflow, magnet too strong\n";
    }
    // AGC maximum gain overflow, magnet too weak
    if (ml == 1){
        error += "AGC maximum gain overflow, magnet too weak\n";
    }
    // Magnet was not detected
    if (md == 0){
        error += "Magnet was NOT detected\n";
    }
    //error = "FORCED/n";
    // If an error was detected
    if (error != ""){

        uint8_t magnitudeLSB = readReg(REG_MAGNITUDE_MSB);
        uint8_t magnitudeMSB = readReg(REG_MAGNITUDE_LSB);
        uint16_t magnitude = (magnitudeMSB << 8) | magnitudeLSB;

        uint8_t agcVal = readReg(REG_AGC);

        error = "The following errors were detected:\n" + error + "AGC Value = " + std::to_string(agcVal) + "/255\n" + "The magnet magnitude = " + std::to_string(magnitude) + "/65,535";
        throw std::runtime_error(error);
    }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Zero ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Sets the current of angle of the encoder to the 0 position
void AS5600::zero(){

    // Get the raw angle values of the device
    uint8_t rawAngleMSB = readReg(REG_RAW_ANGLE_MSB);
    uint8_t rawAngleLSB = readReg(REG_RAW_ANGLE_LSB);

    // Write raw angles to ZPOS (Start position)
    writeReg(REG_ZPOS_MSB, rawAngleMSB);
    writeReg(REG_ZPOS_LSB, rawAngleLSB);

    // Write raw angles to MPOS (End position)
    //writeReg(REG_MPOS_MSB, rawAngleMSB - 1);
    //writeReg(REG_MPOS_LSB, rawAngleLSB - 1);

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Angle Reading ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Returns the rotational step of the encoder (0 - 4095)
uint16_t AS5600::getStep(){

    // Get angle value
    uint8_t angleMSB = readReg(REG_ANGLE_MSB);
    uint8_t angleLSB = readReg(REG_ANGLE_LSB);

    return (angleMSB << 8) | angleLSB;
}

// Returns the rotational step of the encoder (0 - 4095)
uint16_t AS5600::getRawStep(){

    // Get angle value
    uint8_t angleMSB = readReg(REG_RAW_ANGLE_MSB);
    uint8_t angleLSB = readReg(REG_RAW_ANGLE_LSB);

    return (angleMSB << 8) | angleLSB;
}

// Returns the angle of the encoder
float AS5600::getAngle(){

    // Converts steps into an angle
    return (static_cast<float>(getStep()) * 45.0f) / 512.0f; // 360 degrees / 4096 steps = 45/512

}