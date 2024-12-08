#include "as5600.h"
#include "i2c.h"

#include <chrono> // For time in ms
#include <thread> // For sleeping
#include <cstdint> // for uint16_t
#include <iostream>
#include <iomanip>

// I2C Config
const std::string I2C_DIRECTORY = "/dev/i2c-1"; // Define I2C directory

// ~~ AS5600 Config ~~

// Power Mode
// 00 = NOM, 01 = LPM1, 10 = LPM2, 11 = LPM3
uint8_t PM = 0b00;

// Hystersis
// 00 = OFF, 01 = 1 LSB, 10 = 2 LSBs, 11 = 3 LSBs
uint8_t HYST = 0b11;

/* Output Stage
 00 = analog (full range from 0% to 100% between GND and VDD, 01 = analog
(reduced range from 10% to 90% between GND and VDD, 10 = digital PWM */
uint8_t OUTS = 0b00;

// PWM Frequency
// 00 = 115 Hz; 01 = 230 Hz; 10 = 460 Hz; 11 = 920 Hz
uint8_t PWMF = 0b00;

// Slow Filter
// 00 = 16x ; 01 = 8x; 10 = 4x; 11 = 2x
uint8_t SF = 0b00;

/* Fast Filter Threshold
000 = slow filter only, 001 = 6 LSBs, 010 = 7 LSBs, 011 = 9 LSBs,100 = 18 LSBs, 101
= 21 LSBs, 110 = 24 LSBs, 111 = 10 LSBs*/
uint8_t FTH = 0b000;

// Watchdog
// 0 = OFF, 1 = ON
uint8_t WD = 0b0;

int main() {

    // Config Building
    uint16_t CONF = 0x0000;
    CONF |= PM;
    CONF |= HYST << 2;
    CONF |= OUTS << 4;
    CONF |= PWMF << 6;
    CONF |= SF << 8;
    CONF |= FTH << 10;
    CONF |= WD << 13;

    std::cout << "CONF: " << CONF << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    // Object Building
    I2C i2c(I2C_DIRECTORY);                  // Create I2C object
    AS5600 as5600(&i2c, CONF);      // AS5600 Object

    uint16_t step;
    float angle;

    while(true){
        step = as5600.getStep();
        angle = as5600.getAngle();

        std::cout << "Step: " << step << " Angle: " << std::fixed << std::setprecision(2) << angle << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

}