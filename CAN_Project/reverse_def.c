//reverse_def.c
#include <lpc21xx.h>
#include "LED_BLINK.h"
#include "delay.h"
#include "reverse_def.h"

/*
    HC-SR04 holds ECHO high for ~38 ms when nothing is in range.
    Limit the wait to 30 ms of REAL time using Timer0 (1 us per tick,
    T0PR = 14 at PCLK = 15 MHz), instead of counting loop iterations
    whose duration depends on CPU speed / optimisation level.
*/
#define ECHO_MAX_US       30000U
#define OUT_OF_RANGE_CM   999U

void Ultrasonic_Init(void)
{
    IODIR0 |= (TRIG);
    IODIR0 &= ~(ECHO);
}

/*
    Returns distance in cm.

        0   -> sensor never started an echo (not connected / not
               responding).  The application treats 0 as STOP
               (fail-safe).
        999 -> echo never ended = nothing within range (clear path).
               The application treats this as SAFE.
*/
unsigned int Ultrasonic_Trigger(void)
{
    unsigned int time = 0;
    unsigned int count = 0;

    /* Generate 10 us Trigger Pulse */
    IOSET0 = TRIG;
    delay_US(10);
    IOCLR0 = TRIG;

    /* Wait for Echo HIGH (sensor responding) */
    while((IOPIN0 & ECHO) == 0)
    {
        if(++count > TIMEOUT)
            return 0;
    }

    /* Start Timer */
    timer_us();

    /* Wait for Echo LOW, limited by real elapsed time */
    while(IOPIN0 & ECHO)
    {
        if(T0TC > ECHO_MAX_US)
        {
            T0TCR = 0;
            return OUT_OF_RANGE_CM;
        }
    }

    /* Stop Timer */
    time = T0TC;
    T0TCR = 0;

    return (time / 59);
}

void Indicator_Output(unsigned char indicator)
{
    switch(indicator)
    {
        case IND_LEFT:
            Blink_left();
            break;

        case IND_RIGHT:
            Blink_right();
            break;

        case IND_OFF:
            IOSET0 = (1<<4)|(1<<5)|(1<<6)|(1<<7);
            break;
    }
}
