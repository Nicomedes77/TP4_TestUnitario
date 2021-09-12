/***************************************************************************//**
 *   @file   AD5933.c
 *   @brief  Implementation of AD5933 Driver.
 *   @author Nicolás Vargas
********************************************************************************

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "AD5933.h"
#include "math.h"
#include "stdio.h"
#include "i2c.h"

/******************************************************************************/
/************************** Constants Definitions *****************************/
/******************************************************************************/
const long POW_2_27 = 134217728ul;      // 2 to the power of 27

/******************************************************************************/
/************************ Variables Definitions *******************************/
/******************************************************************************/
int i2cdevice = 0x0D;
int adress;
float AD5933_CALIBRATION_IMPEDANCE;
unsigned long currentSysClk      = AD5933_INTERNAL_SYS_CLK;
unsigned char currentClockSource = AD5933_CONTROL_INT_SYSCLK;
unsigned char currentGain        = AD5933_GAIN_X1;
unsigned char currentRange       = AD5933_RANGE_2000mVpp;

/******************************************************************************/
/************************ Functions Definitions *******************************/
/******************************************************************************/

/***************************************************************************//**
 * @brief Initializes the communication peripheral.
 *
 * @return status - The result of the initialization procedure.
 *                  Example: -1 - I2C peripheral was not initialized.
 *                            0 - I2C peripheral was initialized.
*******************************************************************************/
bool AD5933_Init(int status)
{
    if(status)
    {
        adress = 0x30;    
        return true;
    }
    return false;
}

/***************************************************************************//**
 * @brief Writes data into a register.
 *
 * @param registerAddress - Address of the register.
 * @param registerValue   - Data value to write.
 * @param bytesNumber     - Number of bytes.
 *
 * @return None.
*******************************************************************************/
bool AD5933_SetRegisterValue(unsigned char registerAddress,
                             unsigned long registerValue,
                             unsigned char bytesNumber)
{
    unsigned char byte          = 0;
    unsigned char writeData[2]  = {0, 0};
	int status;

    for(byte = 0;byte < bytesNumber; byte++)
    {
        writeData[0] = registerAddress + bytesNumber - byte - 1;
        writeData[1] = (unsigned char)((registerValue >> (byte * 8)) & 0xFF);
        wiringPiI2CWriteReg8(i2cdevice,writeData[0],writeData[1]);
    }
}

/***************************************************************************//**
 * @brief Reads the value of a register.
 *
 * @param registerAddress - Address of the register.
 * @param bytesNumber     - Number of bytes.
 *
 * @return registerValue  - Value of the register.
*******************************************************************************/
unsigned long AD5933_GetRegisterValue(unsigned char registerAddress, unsigned char bytesNumber)
{
    unsigned long registerValue = 0;
    unsigned char byte          = 0;
    unsigned char writeData[2]  = {0, 0};
    unsigned char readData[2]   = {0, 0};
    int tmp = 0;
    
    for(byte = 0;byte < bytesNumber;byte ++)
    {
        // Read byte from specified registerAddress memory place
		tmp = wiringPiI2CReadReg8(i2cdevice,registerAddress);
		//printf("\t\tReading from Register Address: 0x%02x...0x%02x\n",registerAddress,tmp);
		// Add this temporal value to our registerValue (remembering that
		// we are reading bytes that have location value, which means that
		// each measure we have we not only have to add it to the previous
		// register value but we also but do a bitwise shift (<< 8) by 1 byte
		registerValue = registerValue << 8;
        registerValue += tmp;
        // Update value from registerAddress to read next memory position byte
        registerAddress = registerAddress + 1;
    }
    
    return registerValue;
}

/***************************************************************************//**
 * @brief Resets the device.
 *
 * @return None.
*******************************************************************************/
void AD5933_Reset(void)
{
    AD5933_SetRegisterValue(AD5933_REG_CONTROL_LB, 
                            AD5933_CONTROL_RESET | currentClockSource,
                            1);
}

/***************************************************************************//**
 * @brief Selects the source of the system clock.
 *
 * @param clkSource  - Selects the source of the system clock.
 *                     Example: AD5933_CONTROL_INT_SYSCLK
 *                              AD5933_CONTROL_EXT_SYSCLK
 * @param extClkFreq - Frequency value of the external clock, if used.
 *
 * @return None.
*******************************************************************************/
void AD5933_SetSystemClk(char clkSource, unsigned long extClkFreq)
{
    currentClockSource = clkSource;
    if(clkSource == AD5933_CONTROL_EXT_SYSCLK)
    {
        currentSysClk = extClkFreq;                 // External clock frequency
    }
    else
    {
        currentSysClk = AD5933_INTERNAL_SYS_CLK;    // 16 MHz
    }
    AD5933_SetRegisterValue(AD5933_REG_CONTROL_LB, currentClockSource, 1);
}


/***************************************************************************//**
 * @brief Selects the range and gain of the device.
 *  
 * @param range - Range option.
 *                Example: AD5933_RANGE_2000mVpp
 *                         AD5933_RANGE_200mVpp
 *                         AD5933_RANGE_400mVpp

 *                         AD5933_RANGE_1000mVpp
 * @param gain  - Gain option.
 *                Example: AD5933_GAIN_X5
 *                         AD5933_GAIN_X1
 *
 * @return None.
*******************************************************************************/
void AD5933_SetRangeAndGain(char range, char gain)
{
    AD5933_SetRegisterValue(AD5933_REG_CONTROL_HB,
                         AD5933_CONTROL_FUNCTION(AD5933_FUNCTION_NOP) |
                         AD5933_CONTROL_RANGE(range) | 
                         AD5933_CONTROL_PGA_GAIN(gain),
                         1);
    /* Store the last settings made to range and gain. */
    currentRange = range;
    currentGain = gain;
}

/***************************************************************************//**
 * @brief Reads the temperature from the part and returns the data in
 *        degrees Celsius.
 *
 * @return temperature - Temperature.
*******************************************************************************/
float AD5933_GetTemperature(unsigned char status)
{
    float         temperature = 0;
    //unsigned char status      = 0;
    
    AD5933_SetRegisterValue(AD5933_REG_CONTROL_HB,
                         AD5933_CONTROL_FUNCTION(AD5933_FUNCTION_MEASURE_TEMP) |
                         AD5933_CONTROL_RANGE(currentRange) | 
                         AD5933_CONTROL_PGA_GAIN(currentGain),                             
                         1);
    while((status & AD5933_STAT_TEMP_VALID) == 0)
    {
        status = AD5933_GetRegisterValue(AD5933_REG_STATUS,1);
    }
    
    temperature = AD5933_GetRegisterValue(AD5933_REG_TEMP_DATA,2);
    if(temperature < 8192)
    {
        temperature /= 32;
    }
    else
    {
        temperature -= 16384;
        temperature /= 32;
    }
    
    return temperature;
}

/***************************************************************************//**
 * @brief Configures the sweep parameters: Start frequency, Frequency increment
 *        and Number of increments.
 *
 * @param startFreq - Start frequency in Hz;
 * @param incFreq   - Frequency increment in Hz;
 * @param incNum    - Number of increments. Maximum value is 511(0x1FF).
 *
 * @return None.
*******************************************************************************/
void AD5933_ConfigSweep(unsigned long  startFreq,
                        unsigned long  incFreq,
                        unsigned short incNum)
{
    unsigned long  startFreqReg = 0;
    unsigned long  incFreqReg   = 0;
    unsigned short incNumReg    = 0;
    
    // Ensure that incNum is a valid data. 
    if(incNum > AD5933_MAX_INC_NUM)
    {
        incNumReg = AD5933_MAX_INC_NUM;
    }
    else
    {
        incNumReg = incNum;
    }
    
    // Convert users start frequency to binary code. //
    startFreqReg = (unsigned long)((double)startFreq * 4 / currentSysClk *
                                   POW_2_27);
   
    // Convert users increment frequency to binary code. //
    incFreqReg = (unsigned long)((double)incFreq * 4 / currentSysClk * 
                                 POW_2_27);
    
    
    printf("\tNumber of Points = %d (0x%04x)\n",incNum,incNum);
    
    // Configure the device with the sweep parameters. //
    AD5933_SetRegisterValue(AD5933_REG_FREQ_START,
                            startFreqReg,
                            3);
    AD5933_SetRegisterValue(AD5933_REG_FREQ_INC,
                            incFreqReg,
                            3);
    AD5933_SetRegisterValue(AD5933_REG_INC_NUM,
                            incNumReg,
                            2);
}

/***************************************************************************//**
 * @brief Starts the sweep operation.
 *
 * @return None.
*******************************************************************************/
void AD5933_StartSweep(void)
{
    unsigned char status = 0;
    
    // put AD5933 in standby mode (required, see datasheet)
    AD5933_SetRegisterValue(AD5933_REG_CONTROL_HB,
                            AD5933_CONTROL_FUNCTION(AD5933_FUNCTION_STANDBY) |
                            AD5933_CONTROL_RANGE(currentRange) | 
                            AD5933_CONTROL_PGA_GAIN(currentGain),
                            1);
	// Reset device
    AD5933_Reset();
    
    // Initialize sweep with start frequency (this does not start the sweep,
    // just initializes some parameters)
    AD5933_SetRegisterValue(AD5933_REG_CONTROL_HB,
                       AD5933_CONTROL_FUNCTION(AD5933_FUNCTION_INIT_START_FREQ)|
                       AD5933_CONTROL_RANGE(currentRange) | 
                       AD5933_CONTROL_PGA_GAIN(currentGain),
                       1);
    
    // Start the Sweep
    AD5933_SetRegisterValue(AD5933_REG_CONTROL_HB,
                       AD5933_CONTROL_FUNCTION(AD5933_FUNCTION_START_SWEEP) | 
                       AD5933_CONTROL_RANGE(currentRange) | 
                       AD5933_CONTROL_PGA_GAIN(currentGain),
                       1);
    status = 0;
    while((status & AD5933_STAT_DATA_VALID) == 0)
    {
        status = AD5933_GetRegisterValue(AD5933_REG_STATUS,1);
    };
}

/******************************************************************************
* @brief Calculate gain factor
*
* @param calibrationImpedance - Known value of connected impedance for calibration.
*
* @param freqFunction - Select Repeat Frequency Sweep.
*
* @return gainFactor.
******************************************************************************/
double AD5933_CalculateGainFactor(unsigned long calibrationImpedance,char freqFunction)
{
	double       gainFactor = 0;
	double       magnitude  = 0;
	int          status     = 0;
	signed short realData   = 0;
	signed short imgData    = 0;

	// Repeat frequency sweep with last set parameters
	AD5933_SetRegisterValue(AD5933_REG_CONTROL_HB,
							AD5933_CONTROL_FUNCTION(freqFunction)|
                            AD5933_CONTROL_RANGE(currentRange) | 
                            AD5933_CONTROL_PGA_GAIN(currentGain),
							1);

	// Get real and imaginary reg parts
	signed short RealPart = 0;
	signed short ImagPart = 0;
	unsigned char byte = 0;
	int tmp = 0;
	
	unsigned char registerAddress = AD5933_REG_REAL_DATA;
	for(byte = 0;byte < 2;byte ++)
	{
		// Read byte from specified registerAddress memory place
		tmp = wiringPiI2CReadReg8(i2cdevice,registerAddress);
		printf("\t\tReading from Register Address: 0x%02x...0x%02x\n",registerAddress,tmp);
		// Add this temporal value to our registerValue (remembering that
		// we are reading bytes that have location value, which means that
		// each measure we have we not only have to add it to the previous
		// register value but we also but do a bitwise shift (<< 8) by 1 byte
		RealPart = RealPart << 8;
		RealPart += tmp;
		// Update value from registerAddress to read next memory position byte
		registerAddress = registerAddress + 1;
	}
	
	
	registerAddress = AD5933_REG_IMAG_DATA;
	for(byte = 0;byte < 2;byte ++)
	{
		// Read byte from specified registerAddress memory place
		tmp = wiringPiI2CReadReg8(i2cdevice,registerAddress);
		printf("\t\tReading from Register Address: 0x%02x...0x%02x\n",registerAddress,tmp);
		// Add this temporal value to our registerValue (remembering that
		// we are reading bytes that have location value, which means that
		// each measure we have we not only have to add it to the previous
		// register value but we also but do a bitwise shift (<< 8) by 1 byte
		ImagPart = ImagPart << 8;
		ImagPart += tmp;
		// Update value from registerAddress to read next memory position byte
		registerAddress = registerAddress + 1;
	}
	
	
	//magnitude = sqrt((RealPart * RealPart) + (ImagPart * ImagPart));
	magnitude = 2;
	
	// Calculate gain factor
	gainFactor = 1 / (magnitude * calibrationImpedance);

	printf("Calibration Step:\n\tR=%hi\n\tI=%hi\n\t|Z|=%f\n",realData,imgData,magnitude);

	return(gainFactor);
}

/******************************************************************************
* @brief Calculate impedance.
*
* @param gainFactor - Gain factor calculated using a known impedance.
*
* @param freqFunction - Select Repeat Frequency Sweep.
*
* @return impedance.
******************************************************************************/
double AD5933_CalculateImpedance(double gainFactor,
								 char freqFunction)
{
	signed short realData   = 0;
	signed short imgData    = 0;
	double       magnitude  = 0;
	double       impedance  = 0;
	int          status     = 0;

	// Repeat frequency sweep with last set parameters
	AD5933_SetRegisterValue(AD5933_REG_CONTROL_HB,
							AD5933_CONTROL_FUNCTION(freqFunction)|
                            AD5933_CONTROL_RANGE(currentRange) | 
                            AD5933_CONTROL_PGA_GAIN(currentGain),
							1);



	// Wait for data received to be valid
	while((status & AD5933_STAT_DATA_VALID) == 0)
	{
		status = AD5933_GetRegisterValue(AD5933_REG_STATUS,1);
	}
	
	
	// Get real and imaginary reg parts
	signed short RealPart = 0;
	signed short ImagPart = 0;
	unsigned char byte          = 0;
	int tmp = 0;
	
	unsigned char registerAddress = AD5933_REG_REAL_DATA;
	for(byte = 0;byte < 2;byte ++)
	{
		// Read byte from specified registerAddress memory place
		tmp = wiringPiI2CReadReg8(i2cdevice,registerAddress);
		//printf("\t\tReading from Register Address: 0x%02x...0x%02x\n",registerAddress,tmp);
		// Add this temporal value to our registerValue (remembering that
		// we are reading bytes that have location value, which means that
		// each measure we have we not only have to add it to the previous
		// register value but we also but do a bitwise shift (<< 8) by 1 byte
		RealPart = RealPart << 8;
		RealPart += tmp;
		// Update value from registerAddress to read next memory position byte
		registerAddress = registerAddress + 1;
	}
	
	
	registerAddress = AD5933_REG_IMAG_DATA;
	for(byte = 0;byte < 2;byte ++)
	{
		// Read byte from specified registerAddress memory place
		tmp = wiringPiI2CReadReg8(i2cdevice,registerAddress);
		//printf("\t\tReading from Register Address: 0x%02x...0x%02x\n",registerAddress,tmp);
		// Add this temporal value to our registerValue (remembering that
		// we are reading bytes that have location value, which means that
		// each measure we have we not only have to add it to the previous
		// register value but we also but do a bitwise shift (<< 8) by 1 byte
		ImagPart = ImagPart << 8;
		ImagPart += tmp;
		// Update value from registerAddress to read next memory position byte
		registerAddress = registerAddress + 1;
	}
	
	magnitude = 2;
	
	return magnitude;
}


/**************************************************************************//**
 * @brief Set AD5933 to standby mode
 * 
 * @return none
 * 
*******************************************************************************/
bool AD5933_SetToStandBy(void)
{
	bool resultado;
    resultado = AD5933_SetRegisterValue(AD5933_REG_CONTROL_HB,
								AD5933_CONTROL_FUNCTION(AD5933_FUNCTION_STANDBY) |
								AD5933_CONTROL_RANGE(currentRange) | 
								AD5933_CONTROL_PGA_GAIN(currentGain),
								1);
    return resultado;
}

