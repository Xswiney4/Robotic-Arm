#ifndef PCA9685_H
#define PCA9685_H

#include <string> // For std::string

class PCA9685
{
private:
    int* i2cBusPtr; // I2C file descriptor
    int address;        // I2C slave address

public:
    PCA9685(int* i2cBusPtr, int address); // Constructor
    ~PCA9685(); // Destructor
    
	bool initialize(int pwmFreq); // Initialize
	
    bool writeReg(uint8_t reg, uint8_t value);
    uint8_t readReg(uint8_t reg);
    
    setPWMFreq(int pwmFreq); // Sets PWM frequency
    
};

#endif
