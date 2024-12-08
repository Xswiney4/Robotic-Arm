#include "pca9685.h"
#include "i2c.h"

#include <string>
#include <cstdint>  // For uint8_t and system
#include <unistd.h>  // For sleep()


const uint8_t PCA9685_ADDR = 0x40;     		    // Define PCA9685 address
const std::string I2C_DIRECTORY = "/dev/i2c-1"; // Define I2C directory

int main() {
    I2C i2c(I2C_DIRECTORY);                  // Create I2C object
    PCA9685 pca9685(&i2c, PCA9685_ADDR);     // Pass pointer to I2C object and address
    
    pca9685.switchOn(1);
    //pca9685.setPWM(1, 819, 3276); // 0 degrees
    pca9685.setPWM(1, 2666, 1429); // 1000 degrees
    
    while (true){
		sleep(1);
	}
}
