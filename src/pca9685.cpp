#include "pca9685.h"
#include <fcntl.h>     // For open
#include <unistd.h>    // For close, read, write
#include <linux/i2c-dev.h> // For I2C ioctl commands
#include <sys/ioctl.h>
#include <stdexcept>   // For std::runtime_error
#include <iostream>    // For std::cerr

// Constructor: Opens the I2C bus and sets the slave address
PCA9685::PCA9685(int i2cBus, int addr) : address(addr) {
    std::string busPath = "/dev/i2c-" + std::to_string(i2cBus);
    fileDescriptor = open(busPath.c_str(), O_RDWR);
    if (fileDescriptor < 0) {
        throw std::runtime_error("Failed to open I2C bus");
    }

    if (ioctl(fileDescriptor, I2C_SLAVE, address) < 0) {
        close(fileDescriptor);
        throw std::runtime_error("Failed to set I2C slave address");
    }

    // Initialization sequence
    setPWMFreq(50); // Default frequency
}

// Destructor: Closes the I2C file
PCA9685::~PCA9685() {
    close(fileDescriptor);
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
