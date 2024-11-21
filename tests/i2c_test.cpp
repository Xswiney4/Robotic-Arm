#include "i2c.h"
#include <iostream>
#include <ostream>

#define PCA9685_ADDR 0x40

int main(){
	I2C i2c("/dev/i2c-1");
	
	
	if (i2c.pingSlave(PCA9685_ADDR)) {
        std::cout << "Slave is present!" << std::endl;
    } else {
        std::cout << "Failed to ping slave!" << std::endl;
    }
    
    return 0;
}
