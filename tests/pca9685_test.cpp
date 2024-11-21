#include <iostream>
#include "pca9685.h"
#include <fcntl.h>     // For open
#include <unistd.h>    // For close, read, write

int main(){
	PCA9685 pca9685;
	
	if(!pca9685.initialize()){
		
	}
}
