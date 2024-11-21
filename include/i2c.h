#ifndef i2c_H
#define i2c_H

#include <string>
#include <vector>

class I2C
{
private:
    std::string busPath; // I2C bus path
    int i2cBus; // I2C file descriptor
    uint8_t activeSlave; // Currently active slave
    std::vector<uint8_t> slaves; // Database of all slaves registered on I2C bus
    
    bool setSlave(uint8_t slave); // Configures slave for reading/writing
    bool isActive(uint8_t slave); // Returns true if slave is active
    bool validateSlave(uint8_t slave); // Checks if slave has been registered

public:
    I2C(const std::string& busPath); // Constructor
    ~I2C(); // Destructor
	
    bool registerSlave(uint8_t addr); // Registers slave into database
    bool pingSlave(uint8_t addr); // Pings slave at given address, returns true if we get a response
    
    std::vector<uint8_t> read(uint8_t addr, int numBytes);
    bool write(uint8_t addr, uint8_t* buffer, int numBytes);
    
};

#endif
