#ifndef AS5600_H
#define AS5600_H

#include "i2c.h"
#include <cstdint> // For uint8_t

// Slave Address
#define AS5600_ADDRESS 0x36

// Register Definitions
#define REG_RAW_ANGLE_MSB 0x0C
#define REG_RAW_ANGLE_LSB 0x0D
#define REG_ANGLE_MSB 0x0E
#define REG_ANGLE_LSB 0x0F

#define REG_CONF_MSB 0x07
#define REG_CONF_LSB 0x08

#define REG_ZMCO 0x01
#define REG_ZPOS_MSB 0x01
#define REG_ZPOS_LSB 0x02
#define REG_MPOS_MSB 0x03
#define REG_MPOS_LSB 0x04

#define REG_AGC 0x1A
#define REG_MAGNET_STATUS 0x0B
#define REG_MAGNITUDE_MSB 0X1B
#define REG_MAGNITUDE_LSB 0x1C

class AS5600
{
private:
    // Private Variables
    I2C *i2c;        // Pointer to I2C object
    uint8_t address; // I2C slave address

    // Config
    void initConfig(uint16_t config);

    // Helper Methods
    void writeReg(uint8_t reg, uint8_t value); 						// Writes a value to a register
    uint8_t readReg(uint8_t reg); 									// Reads a value from a register
    void modifyReg(uint8_t reg, uint8_t mask, uint8_t value); 		// Modifies specific bits in a register without overwriting the entire register.

public:
    // Constructor/Destructor
    AS5600(I2C *i2cPtr, uint16_t config);

    // Status
    void checkMagnet(); // Check the status of the magnet, throws error if magnet is not found

    // Zero
    void zero(); // Sets the current of angle of the encoder to the 0 position

    // Angle Reading
    uint16_t getStep(); // Returns the rotational step of the encoder (0 - 4095)
    uint16_t getRawStep(); // Returns the rotational step of the encoder (0 - 4095)
    float getAngle();   // Returns the angle of the encoder
};

#endif
