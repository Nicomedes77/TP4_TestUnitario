/*
El chip AD5933 es un analizador
de impedancias integrado que permite
tomar mediciones del tipo bipolar, en 
un espectro de 100 Hz a 100 kHz.
*/

#include "unity.h"
#include "mock_i2c.h"
#include "AD5933.h"
#include "math.h"

//static uint16_t puerto;

void setUp(void)
{
    //  LedsInit(&puerto);
}
void tearDown(void)
{
    
}

/* testeo si el chip devuelve el valor de temperatura en float*/
void test_AD5933_tomaTemperatura(void)
{
    unsigned char status = 0;
    unsigned char writeData[2]  ={0x80, 0x91};
    int i2cdevice = 0x0D;
    int registerAddress_STATUS = 0x8F;
    int registerAddress_TEMP_DATA_1 = 0x92;
    int registerAddress_TEMP_DATA_2 = 0x93;

    // escritura registro BARRIDO
    wiringPiI2CWriteReg8_ExpectAndReturn(i2cdevice,writeData[0],writeData[1],true);
    // lectura registro STATUS
    wiringPiI2CReadReg8_ExpectAndReturn(i2cdevice,registerAddress_STATUS,1);
    // lectura registro TEMP_1
    wiringPiI2CReadReg8_ExpectAndReturn(i2cdevice,registerAddress_TEMP_DATA_1,1);
    // lectura registro TEMP_2
    wiringPiI2CReadReg8_ExpectAndReturn(i2cdevice,registerAddress_TEMP_DATA_2,1);
    float val = AD5933_GetTemperature(status);
    TEST_ASSERT_EQUAL_FLOAT(8.03125,val);
}

/* testeo si el chip se inicializa correctamente enviandole un 1 */
void test_AD5933_Init(void)
{
    int inicio = 1;
    bool res = AD5933_Init(inicio);
    TEST_ASSERT_TRUE(res);
}

/* testeo si el chip NO se inicializa correctamente enviandole un 0 */
void test_AD5933_NO_Init(void)
{
    int inicio = 0;
    bool res = AD5933_Init(inicio);
    TEST_ASSERT_FALSE(res);
}

/* testeo si el chip lee un registro */
void test_lecturaRegistro(void)
{
    unsigned long resultado;
    unsigned char i2cdevice = 0x0D;
    unsigned char numero = 1;
    int registerAddress = 0x0D;
    //lectura de registro
    wiringPiI2CReadReg8_ExpectAndReturn(i2cdevice,registerAddress,1);
    resultado = AD5933_GetRegisterValue(i2cdevice,numero);
    TEST_ASSERT_EQUAL_INT64 (1, resultado);
}

/* testeo que evalua que el calculo de ganancia devuelva un valor double */
void test_calculoFactorGanancia(void)
{
    unsigned long calibracion = 1000;
    char frecuenciaFuncion = 8;
    double factorGanancia;
    int i2cdevice = 0x0D;
    unsigned char registerAddress_BARRIDO = 0x8F;
    unsigned char registerAddress_R1 = 0x94;
    unsigned char registerAddress_R2 = 0x95;
    unsigned char registerAddress_R3 = 0x96;
    unsigned char registerAddress_R4 = 0x97;
    unsigned char writeData[2]  ={0x80, 0x81};

    // escritura registro BARRIDO
    wiringPiI2CWriteReg8_ExpectAndReturn(i2cdevice,writeData[0],writeData[1],true);
    // lectura registro parte_REAL1
    wiringPiI2CReadReg8_ExpectAndReturn(i2cdevice,registerAddress_R1,0);
    // lectura registro parte_REAL2
    wiringPiI2CReadReg8_ExpectAndReturn(i2cdevice,registerAddress_R2,0);
    // lectura registro parte_IMG1
    wiringPiI2CReadReg8_ExpectAndReturn(i2cdevice,registerAddress_R3,0);
    // lectura registro parte_IMG2
    wiringPiI2CReadReg8_ExpectAndReturn(i2cdevice,registerAddress_R4,0);

    factorGanancia = AD5933_CalculateGainFactor(calibracion,frecuenciaFuncion);
    TEST_ASSERT_EQUAL_UINT32(0,factorGanancia);
}

/* testeo la puesta en Stand By */
void test_ponerStandBy(void)
{
    int i2cdevice = 0x0D;
    unsigned char writeData[2]  ={0x80, 0xB1};
    // escritura registro STANDBY
    wiringPiI2CWriteReg8_ExpectAndReturn(i2cdevice,writeData[0],writeData[1],true);

    bool val = AD5933_SetToStandBy();
    TEST_ASSERT_TRUE(val);
}