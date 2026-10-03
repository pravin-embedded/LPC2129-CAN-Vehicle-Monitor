//LED_BLINK.c
#include <lpc21xx.h>
#include "delay.h"
#include "LED_BLINK.h"

/* Indicator LEDs on P0.4 - P0.7, ACTIVE LOW:
   IOCLR0 = LED ON, IOSET0 = LED OFF                */

void LED_Init(void)
{
    IODIR0 |= (1<<4) | (1<<5) | (1<<6) | (1<<7);
    IOSET0  = (1<<4) | (1<<5) | (1<<6) | (1<<7);    /* all OFF */
}

void Blink_right(void)
{
    int i;
    for(i=7; i>3; i--)
    {
        IOCLR0 = 1<<i;
        delay_MS(100);
        IOSET0 = 1<<i;
    }
}

void Blink_left(void)
{
    int i;
    for(i=4; i<8; i++)
    {
        IOCLR0 = 1<<i;
        delay_MS(100);
        IOSET0 = 1<<i;
    }
}

/* Reset and start Timer0 (1 us per tick when T0PR = 14 at PCLK = 15 MHz) */
void timer_us(void)
{
    T0TCR = 0x02;       /* reset counter + prescaler */
    T0TCR = 0x01;       /* enable                    */
}
