#include "src/AD5933.h"
#include "/var/lib/gems/2.5.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"






















void setUp(void)

{



}

void tearDown(void)

{



}





void test_AD5933_tomaTemperatura(void)

{

    unsigned char status = 0;

    float val = AD5933_GetTemperature(status);

    UnityAssertFloatsWithin((UNITY_FLOAT)((UNITY_FLOAT)((660.5938)) * (UNITY_FLOAT)(0.00001f)), (UNITY_FLOAT)((UNITY_FLOAT)((660.5938))), (UNITY_FLOAT)((UNITY_FLOAT)((val))), ((

   ((void *)0)

   )), (UNITY_UINT)((UNITY_UINT)(28)));

}





void test_AD5933_Init(void)

{

    int inicio = 1;

    

   _Bool 

        res = AD5933_Init(inicio);

    do {if ((res)) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(36)));}} while(0);

}





void test_AD5933_NO_Init(void)

{

    int inicio = 0;

    

   _Bool 

        res = AD5933_Init(inicio);

    do {if (!(res)) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(44)));}} while(0);

}





void test_lecturaRegistro(void)

{

    unsigned long resultado;

    unsigned char direccion = 0x43;

    unsigned char numero = 23;

    resultado = AD5933_GetRegisterValue(direccion,numero);

    UnityAssertEqualNumber((UNITY_INT)((5932177859674593369)), (UNITY_INT)((resultado)), (

   ((void *)0)

   ), (UNITY_UINT)(54), UNITY_DISPLAY_STYLE_INT64);

}





void test_calculoFactorGanancia(void)

{

    unsigned long calibracion = 1000;

    char frecuenciaFuncion = 8;

    double factorGanancia;

    factorGanancia = AD5933_CalculateGainFactor(calibracion,frecuenciaFuncion);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((0)), (UNITY_INT)(UNITY_UINT32)((factorGanancia)), (

   ((void *)0)

   ), (UNITY_UINT)(64), UNITY_DISPLAY_STYLE_UINT32);

}





void test_ponerStandBy(void)

{

    

   _Bool 

        val = AD5933_SetToStandBy();

    do {if ((val)) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(71)));}} while(0);

}
