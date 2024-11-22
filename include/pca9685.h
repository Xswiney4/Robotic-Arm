#ifndef PCA9685_H
#define PCA9685_H

#include "i2c.h"
#include <cstdint>  // For uint8_t

// Register Definitions
#define MODE1_REG 0x00
#define MODE2_REG 0x01

#define ALL_LED_ON_L 0xFA
#define ALL_LED_ON_H 0xFB
#define ALL_LED_OFF_L 0xFC
#define ALL_LED_OFF_H 0xFD
#define PRESCALE_REG 0xFE

class PCA9685
{
private:
	// Private Variables
    I2C* i2c; 		// Pointer to I2C object
    uint8_t address; 	// I2C slave address
    
    // Helper methods
    void writeReg(uint8_t reg, uint8_t value); 						// Writes a value to a register
    uint8_t readReg(uint8_t reg); 									// Reads a value from a register
    void modifyReg(uint8_t reg, uint8_t mask, uint8_t value); 		// Modifies specific bits in a register without overwriting the entire register.
    void setPrescaler(uint8_t value); 								// Sets Prescaler Value
    uint8_t getRegister(uint8_t channel, uint8_t on, uint8_t high); // Calculates the register for a channel
    
    // Input validation (Throws error if invalid)
    void validatePrescaler(uint8_t value); 	// 3 - 255
    void validateDuty(float duty); 			// 0.0 - 100.0
    void validateChannel(uint8_t channel); 		// 0 - 15

public:
	// Constructor/Destructor
    PCA9685(I2C* i2cPtr, uint8_t addr, uint8_t prescaler = 0x1E); // Default prescaler is 50hz
    ~PCA9685();
    
    // Global Controls
    void setPWMFrequency(int freq); // Sets Prescaler given frequency in Hz
    void allOn(); 	// Switches all channel on
    void allOff(); 	// Switches all channels off
    
    void sleep(); 	// Puts the PCA9685 into sleep mode
    void wake(); 	// Takes the PCA9685 out of sleep mode
	
	// Channel Controls
	void switchOn(uint8_t channel); 	// Turns channel on
	void switchOff(uint8_t channel); 	// Turns channel off
	void setPWM(uint8_t channel, uint16_t onTime, uint16_t offTime); 	// Sets PWM on/off times (more precise)
    void setDuty(uint8_t channel, float duty); 							// Sets Duty cycle of channel (less precise)
    void setOffTime(uint8_t channel, uint16_t offTime);					// Sets ONLY the offTime
    void setOnTime(uint8_t channel, uint16_t onTime);					// Sets ONLY the onTime
    
};

#endif
