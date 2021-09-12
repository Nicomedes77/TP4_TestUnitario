#ifndef i2c_H_
#define i2c_H_

#include <stdint.h>
#include <stdbool.h>

bool i2c_Init( int i2c_add, unsigned char frecClock );
bool wiringPiI2CWriteReg8(int i2cdevice,unsigned char writeD_0,unsigned char writeD_1);
int wiringPiI2CReadReg8(int i2cdevice,unsigned char registerAddress);

#endif 