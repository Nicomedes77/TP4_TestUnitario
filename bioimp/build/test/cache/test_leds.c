#include "src/leds.h"
#include "/var/lib/gems/2.5.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"


static uint16_t puerto;



void setUp(void)

{

    LedsInit(&puerto);

}

void tearDown(void)

{



}





void test_LedsOffAfterCreate(void)

{

    uint16_t puerto = 0xFFFF;

    LedsInit(&puerto);

    UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0X0000)), (UNITY_INT)(UNITY_UINT16)((puerto)), (

   ((void *)0)

   ), (UNITY_UINT)(20), UNITY_DISPLAY_STYLE_UINT16);



}





void test_prender_un_led(void)

{

        const int led = 2;

        LedsTurnOn(led);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((1 << (led - 1))), (UNITY_INT)(UNITY_UINT16)((puerto)), (

       ((void *)0)

       ), (UNITY_UINT)(29), UNITY_DISPLAY_STYLE_UINT16);

}





void test_apagar_un_led(void)

{

        const int led = 2;

        LedsTurnOn(led);

        LedsTurnOff(led);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((puerto)), (

       ((void *)0)

       ), (UNITY_UINT)(38), UNITY_DISPLAY_STYLE_UINT16);

}





void test_prender_y_apagar_varios_leds(void)

{

        const int led = 2;

        LedsTurnOn(led);

        LedsTurnOn(5);

        LedsTurnOff(5);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((1 << (led - 1))), (UNITY_INT)(UNITY_UINT16)((puerto)), (

       ((void *)0)

       ), (UNITY_UINT)(48), UNITY_DISPLAY_STYLE_UINT16);

}





void test_ledPrendidoOk(void)

{

        const int led = 2;

        LedsTurnOn(led);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((1 << (led - 1))), (UNITY_INT)(UNITY_UINT16)((puerto)), (

       ((void *)0)

       ), (UNITY_UINT)(56), UNITY_DISPLAY_STYLE_UINT16);

        LedsTurnOff(led);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((puerto)), (

       ((void *)0)

       ), (UNITY_UINT)(58), UNITY_DISPLAY_STYLE_UINT16);

}





void test_ledApagadoOk(void)

{

        const int led = 2;

        LedsTurnOff(led);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((puerto)), (

       ((void *)0)

       ), (UNITY_UINT)(66), UNITY_DISPLAY_STYLE_UINT16);

        LedsTurnOn(led);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((1 << (led - 1))), (UNITY_INT)(UNITY_UINT16)((puerto)), (

       ((void *)0)

       ), (UNITY_UINT)(68), UNITY_DISPLAY_STYLE_UINT16);

}





void test_allLedsOn_oneOp(void)

{

        const int led = 0x0000;

        LedsTurnOn(led);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((1 << (led - 1))), (UNITY_INT)(UNITY_UINT16)((puerto)), (

       ((void *)0)

       ), (UNITY_UINT)(76), UNITY_DISPLAY_STYLE_UINT16);

}





void test_allLedsOff_oneOp(void)

{

        const int led = 0xFFFF;

        LedsTurnOff(led);

        UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((puerto)), (

       ((void *)0)

       ), (UNITY_UINT)(84), UNITY_DISPLAY_STYLE_UINT16);

}
