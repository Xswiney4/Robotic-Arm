#include "as5600.h"
#include "i2c.h"

#include <chrono> // For time in ms
#include <thread> // For sleeping
#include <iostream>
#include <iomanip>

// I2C Config
const std::string I2C_DIRECTORY = "/dev/i2c-1"; // Define I2C directory

// AS5600 Config
const uint16_t AS5600_CONFIG = 0x0000;           // Config of AS5600

int main() {

    // Object Building
    I2C i2c(I2C_DIRECTORY);                  // Create I2C object
    AS5600 as5600(&i2c, AS5600_CONFIG);      // AS5600 Object

    uint16_t step;
    float angle;

    while(true){
        step = as5600.getStep();
        angle = as5600.getAngle();

        std::cout << "Step: " << step << " Angle: " << std::fixed << std::setprecision(2) << angle << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

}