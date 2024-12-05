#ifndef AS5600_H
#define AS5600_H

#include "i2c.h"
#include <cstdint>  // For uint8_t

// Register Definitions

class AS5600
{
private:
	// Private Variables
    I2C* i2c; 		// Pointer to I2C object
    uint8_t address; 	// I2C slave address
    
    // Helper methods
    
    // Input validation (Throws error if invalid)

public:
	// Constructor/Destructor
    AS5600(I2C* i2cPtr, uint8_t addr, uint8_t prescaler = 0x1E); // Default prescaler is 50hz
    ~AS5600();
    
};

#endif
