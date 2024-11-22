#include "i2c.h"
#include <fcntl.h>          // For open() function
#include <unistd.h>    // For close, read, write
#include <sys/ioctl.h>      // For ioctl() function
#include <linux/i2c-dev.h>  // For I2C_SLAVE and other I2C constants
#include <stdexcept>        // For exceptions like std::runtime_error
#include <vector>
#include <string>

// Constructor
I2C::I2C(const std::string& busPath): busPath(busPath), i2cBus(-1), activeSlave(0x00){
	
	// Open the I2C bus 
	i2cBus = open(busPath.c_str(), O_RDWR);
	if (i2cBus < 0){
		throw std::runtime_error("Failed to open I2C bus");
	}
}

// Destructor
I2C::~I2C(){
	if(i2cBus >= 0){
		close(i2cBus);
	}
}

// Configures slave for reading/writing
bool I2C::setSlave(uint8_t slave){
	
	// Returns if the slave is already active
	if(isActive(slave)){
		return true;
	}
	// Sets slave
	if(ioctl(i2cBus, I2C_SLAVE, slave) < 0) {
		throw std::runtime_error("Failed to set I2C slave address to " + std::to_string(slave));
		return false;
	}
	activeSlave = slave;
	return true;
}

// Returns true if slave is active
bool I2C::isActive(uint8_t slave){
	if(slave == activeSlave){
		return true;
	}else{
		return false;
	}
}

// Checks if slave has been registered
bool I2C::validateSlave(uint8_t slave){
	for(int i = 0; i < slaves.size(); i++){
		if(slaves[i] == slave){
			return true;
		}
	}
	return false;
}

// Registers slave into database
bool I2C::registerSlave(uint8_t addr){
	if(!validateSlave(addr) && pingSlave(addr)){
		slaves.push_back(addr);
		return true;
	}
	return false;
}

// Pings slave at given address by reading from a known register
bool I2C::pingSlave(uint8_t addr){
    setSlave(addr);
    
    uint8_t dummyByte = 0x00;
    if (::read(i2cBus, &dummyByte, 1) != 1) {
        return false;  // If read fails, the slave is not responding
    }
    return true;  // Return true if the slave responds
}

// Reads information from slave
std::vector<uint8_t> I2C::read(uint8_t addr, int numBytes) {
	
	// Ensures the slave has been registered
	if(!validateSlave(addr)){
		throw std::runtime_error("Slave is not registered to I2C: " + std::to_string(addr));
	}
    setSlave(addr); // Sets the slave as active
    
    std::vector<uint8_t> buffer(numBytes); // Sets the buffer to hold the number of bytes

    // Perform the read operation
    if (::read(i2cBus, buffer.data(), numBytes) != numBytes) {
        throw std::runtime_error("Failed to read from I2C device");
    }

    return buffer; // Return the buffer after successful read
}

// Writes information to slave
bool I2C::write(uint8_t addr, uint8_t* buffer, int numBytes){
	
	// Ensures the slave has been registered
	if(!validateSlave(addr)){
		throw std::runtime_error("Slave is not registered to I2C: " + std::to_string(addr));
	}
	setSlave(addr); // Sets the slave as active
	
	if(::write(i2cBus, buffer, numBytes) != numBytes){
		throw std::runtime_error("Failed to write to I2C device");
		return false;
	}
	return true;
}
