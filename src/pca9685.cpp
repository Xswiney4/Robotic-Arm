// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Libraries ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "pca9685.h"
#include "i2c.h"
#include <stdexcept>   // For std::runtime_error
#include <vector>
#include <cmath> // For round()
#include <iostream>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Constructor/Destructor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructor: Creates and initializes object
PCA9685::PCA9685(I2C* i2cPtr, uint8_t addr, uint8_t prescaler) : i2c(i2cPtr), address(addr){
    
    // Attempts to register PCA9685 into i2c object, returns error if it fails
    if(!i2c->registerSlave(address)){
        throw std::runtime_error("Failed to add PCA9685 to i2c");
    }
    
    // Preprocessing for PWM calculations
    stepSize = calculateStepSize(prescaler);

    // Set the prescaler to the default value and switch all channels off
    setPrescaler(prescaler);
    allOff();
}

// Destructor
PCA9685::~PCA9685(){
    allOff();
    sleep();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Helper methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Writes a value to a register
void PCA9685::writeReg(uint8_t reg, uint8_t value){
    // Creates buffer
    uint8_t buffer[2] = {reg, value};
    
    // Sends two bytes to PCA containing register and value to update, throws error if failure
    if(!i2c -> write(address, buffer, 2)){
        throw std::runtime_error("Failed to write value to register");
    }
}

// Reads a value from a register
uint8_t PCA9685::readReg(uint8_t reg){
    
    // Sends one bytes to PCA containing register to set for reading, throws error if failure
    if(!i2c -> write(address, &reg, 1)){
        throw std::runtime_error("Failed to setup register for reading");
    }
    
    std::vector<uint8_t> buffer = i2c -> read(address, 1);
    return buffer[0];
    
}

// Modifies specific bits in a register without overwriting the entire register.
void PCA9685::modifyReg(uint8_t reg, uint8_t mask, uint8_t value){
    // Get register contents
    uint8_t prevByte = readReg(reg);
    
    // Clear the bits specified by mask, then set those values.
    uint8_t modifiedByte = (prevByte & ~mask) | (value & mask);
    
    // Write the modifed byte to that register.
    writeReg(reg, modifiedByte);
}

// Sets Prescaler
void PCA9685::setPrescaler(uint8_t value){
    
    validatePrescaler(value);

    stepSize = calculateStepSize(value);
    sleep();
    
    writeReg(PRESCALE_REG, value);
    
    wake();
}

/* Calculates the register for a channel
 * Use a 0 or 1 for selecting the OFF/ON register and LOW/HIGH byte
 * Example: getRegister(5,1,0) returns the address for LED5_ON_L -> 0x1A
*/
uint8_t PCA9685::getRegister(uint8_t channel, uint8_t on, uint8_t high){
    
    validateChannel(channel);
    
    return 0x08 + (channel * 4) - (2 * on) + high;
    
}

float PCA9685::calculateStepSize(uint8_t prescaler){
    return (static_cast<float>(prescaler) + 1.0f) / 25.0f;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Input validation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Validates value is between 3 and 255
void PCA9685::validatePrescaler(uint8_t value){
    if(value > 2 && value < 256){
        return;
    }
    throw std::runtime_error("Failed to validate Prescaler");
}

// Validates value is between 0.0 and 100.0
void PCA9685::validateDuty(float duty){
    if(duty >= 0.0 && duty <= 100.0){
        return;
    }
    throw std::runtime_error("Failed to validate Duty Cycle");
}

// Validates value is between 0 and 15
void PCA9685::validateChannel(uint8_t channel){
    if(channel >= 0 && channel < 16){
        return;
    }
    throw std::runtime_error("Failed to validate Channel");
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Global Controls ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Sets Prescaler given frequency in Hz
void PCA9685::setPWMFrequency(int freq){
    setPrescaler(uint8_t(round(25000000.0/(4096.0 * freq)) - 1));
}

// Switches ALL_LED_OFF - off (Effectively turning on ALL channels)
void PCA9685::allOn(){
    modifyReg(ALL_LED_OFF_H, 0x10, 0x00);
}

// Switches ALL_LED_OFF - on (Effectively turning off ALL channels)
void PCA9685::allOff(){
    modifyReg(ALL_LED_OFF_H, 0x10, 0x10);
}

// Puts the PCA9685 into sleep mode
void PCA9685::sleep(){
    modifyReg(MODE1_REG, 0x10, 0x10);
}

// Takes the PCA9685 out of sleep mode
void PCA9685::wake(){
    modifyReg(MODE1_REG, 0x10, 0x00);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~ Channel Controls ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Switches LEDn full OFF - off (Effectively turning on the channel)
void PCA9685::switchOn(uint8_t channel){
    uint8_t reg = getRegister(channel, 0, 1);
    modifyReg(reg, 0x10, 0x00);
}

// Switches LEDn full OFF - on (Effectively turning off the channel)
void PCA9685::switchOff(uint8_t channel){
    uint8_t reg = getRegister(channel, 0, 1);
    modifyReg(reg, 0x10, 0x10);
}

// Sets PWM on/off times (more precise)
void PCA9685::setPWM(uint8_t channel, uint16_t onTime, uint16_t offTime){
    
    // Get Register Addresses
    uint8_t onLowReg = getRegister(channel, 1, 0);
    uint8_t onHighReg = onLowReg + 1;
    uint8_t offLowReg = onLowReg + 2;
    uint8_t offHighReg = onLowReg + 3;
    
    // Seperate input into bytes
    uint8_t onLowByte = onTime & 0x00FF;
    uint8_t onHighByte = (onTime >> 8) & 0x0F;
    uint8_t offLowByte = offTime & 0x00FF;
    uint8_t offHighByte = (offTime >> 8) & 0x0F;
    
    // Write values to registers
    writeReg(onLowReg,onLowByte);
    writeReg(onHighReg,onHighByte);
    writeReg(offLowReg,offLowByte);
    writeReg(offHighReg,offHighByte);
    
}

// Sets PWM based on desired pulseWidth
void PCA9685::setPulseWidth(uint8_t channel, float pulseWidth){
    
    // Calculates offTime
    uint16_t offTime = static_cast<uint16_t>(round(pulseWidth / stepSize));

    // Sets offtime for channel
    setOffTime(channel, offTime);
    
}

// Sets Duty cycle of channel (less precise)
void PCA9685::setDuty(uint8_t channel, float duty){
    
    uint16_t onTime = (duty * 4095) / 100;
    uint16_t offTime = 4095 - onTime;
    
    setPWM(channel, onTime, offTime);
    
}

// Sets ONLY the offTime
void PCA9685::setOffTime(uint8_t channel, uint16_t offTime){
    
    // Get Register Addresses
    uint8_t offLowReg = getRegister(channel, 0, 0);
    uint8_t offHighReg = offLowReg + 1;
    
    // Seperate input into bytes
    uint8_t offLowByte = offTime & 0x00FF;
    uint8_t offHighByte = (offTime >> 8) & 0x0F;
    
    // Write values to registers
    writeReg(offLowReg,offLowByte);
    writeReg(offHighReg,offHighByte);
}

// Sets ONLY the onTime
void PCA9685::setOnTime(uint8_t channel, uint16_t onTime){
    
    // Get Register Addresses
    uint8_t onLowReg = getRegister(channel, 1, 0);
    uint8_t onHighReg = onLowReg + 1;
    
    // Seperate input into bytes
    uint8_t onLowByte = onTime & 0x00FF;
    uint8_t onHighByte = (onTime >> 8) & 0x0F;
    
    // Write values to registers
    writeReg(onLowReg,onLowByte);
    writeReg(onHighReg,onHighByte);
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
