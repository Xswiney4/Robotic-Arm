#ifndef PCA9685_H
#define PCA9685_H

#include <string> // For std::string

class PCA9685
{
private:
    int fileDescriptor; // I2C file descriptor
    int address;        // I2C slave address

    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);

public:
    PCA9685(int i2cBus, int address);
    ~PCA9685();

    void setPWM(int channel, int on, int off);
    void setPWMFreq(float frequency);
};

#endif
