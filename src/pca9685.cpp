/*
#include "pca9685.h"
#include <fcntl.h>     // For open
#include <unistd.h>    // For close, read, write
#include <linux/i2c-dev.h> // For I2C ioctl commands
#include <sys/ioctl.h>
#include <stdexcept>   // For std::runtime_error
#include <iostream>    // For std::cerr

// Constructor: Saves i2c information and sets slave address
PCA9685::PCA9685(int* i2cBusPtr, int addr){
    
    // Validate i2cBus pointer
    if (i2cBusPtr == nullptr) {
        throw std::invalid_argument("I2C bus pointer is null");
    }
    
    this -> i2cBus = i2cBus;
    this -> address = addr;
    
    // Sets i2c to communicate ONLY with this slave address
    if (ioctl(*i2cBusPtr, I2C_SLAVE, addr) < 0) {
        throw std::runtime_error("Failed to set I2C slave address");
    }

}

// Writes a byte to a register
void PCA9685::writeRegister(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    if (write(fileDescriptor, buffer, 2) != 2) {
        throw std::runtime_error("Failed to write to I2C device");
    }
}

// Reads a byte from a register
uint8_t PCA9685::readRegister(uint8_t reg) {
    if (write(fileDescriptor, &reg, 1) != 1) {
        throw std::runtime_error("Failed to write register address to I2C device");
    }

    uint8_t value;
    if (read(fileDescriptor, &value, 1) != 1) {
        throw std::runtime_error("Failed to read from I2C device");
    }

    return value;
}

// Sets the PWM frequency
void PCA9685::setPWMFreq(float frequency) {
    uint8_t prescale = static_cast<uint8_t>(25000000.0 / (4096.0 * frequency) - 1.0);
    writeRegister(0x00, 0x10); // Enter sleep mode
    writeRegister(0xFE, prescale); // Set prescaler
    writeRegister(0x00, 0x80); // Restart
}

// Sets PWM for a specific channel
void PCA9685::setPWM(int channel, int on, int off) {
    writeRegister(0x06 + 4 * channel, on & 0xFF);
    writeRegister(0x07 + 4 * channel, on >> 8);
    writeRegister(0x08 + 4 * channel, off & 0xFF);
    writeRegister(0x09 + 4 * channel, off >> 8);
}
*/
