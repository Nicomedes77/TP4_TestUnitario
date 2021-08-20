/*
El chip AD5933 es un analizador
de impedancias integrado que permite
tomar mediciones del tipo bipolar, en 
un espectro de 100 Hz a 100 kHz.
*/

#include "unity.h"
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
    float val = AD5933_GetTemperature(status);
    TEST_ASSERT_EQUAL_FLOAT(660.5938,val);
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
    unsigned char direccion = 0x43;
    unsigned char numero = 23;
    resultado = AD5933_GetRegisterValue(direccion,numero);
    TEST_ASSERT_EQUAL_INT64 (5932177859674593369, resultado);
}

/* testeo que evalua que el calculo de ganancia devuelva un valor double */
void test_calculoFactorGanancia(void)
{
    unsigned long calibracion = 1000;
    char frecuenciaFuncion = 8;
    double factorGanancia;
    factorGanancia = AD5933_CalculateGainFactor(calibracion,frecuenciaFuncion);
    TEST_ASSERT_EQUAL_UINT32(0,factorGanancia);
}

/* testeo la puesta en Stand By */
void test_ponerStandBy(void)
{
    bool val = AD5933_SetToStandBy();
    TEST_ASSERT_TRUE(val);
}