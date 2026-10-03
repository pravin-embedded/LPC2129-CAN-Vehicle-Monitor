#include <lpc21xx.h>
#include "LED_BLINK.h"
#include "switch.h"

extern volatile unsigned char indicator;
extern volatile unsigned char mode;


/* P0.1 -> EINT0 -> MODE */
void EINT0_ISR_MODE(void) __irq
{
    if(mode == FORWARD)
        mode = REVERSE;
    else
        mode = FORWARD;

    EXTINT = (1 << 0);
    VICVectAddr = 0;
}


/* P0.3 -> EINT1 -> LEFT */
void EINT1_ISR_LEFT(void) __irq
{
    indicator = IND_LEFT;

    EXTINT = (1 << 1);
    VICVectAddr = 0;
}


/* P0.7 -> EINT2 -> RIGHT */
void EINT2_ISR_RIGHT(void) __irq
{
    indicator = IND_RIGHT;

    EXTINT = (1 << 2);
    VICVectAddr = 0;
}


void Switch_Init(void)
{
    /* P0.1 -> EINT0 */
    PINSEL0 &= ~(3 << 2);
    PINSEL0 |=  (3 << 2);

    /* P0.3 -> EINT1 */
    PINSEL0 &= ~(3 << 6);
    PINSEL0 |=  (3 << 6);

    /* P0.7 -> EINT2 */
    PINSEL0 &= ~(3 << 14);
    PINSEL0 |=  (3 << 14);


    /* Edge triggered */
    EXTMODE |= (1 << 0) | (1 << 1) | (1 << 2);

    /* Falling edge polarity */
    EXTPOLAR &= ~((1 << 0) | (1 << 1) | (1 << 2));

    /* Clear pending interrupts */
    EXTINT = (1 << 0) | (1 << 1) | (1 << 2);


    /* EINT0 -> VIC14 -> MODE */
    VICVectAddr0 = (unsigned int)EINT0_ISR_MODE;
    VICVectCntl0 = (1 << 5) | 14;


    /* EINT1 -> VIC15 -> LEFT */
    VICVectAddr1 = (unsigned int)EINT1_ISR_LEFT;
    VICVectCntl1 = (1 << 5) | 15;


    /* EINT2 -> VIC16 -> RIGHT */
    VICVectAddr2 = (unsigned int)EINT2_ISR_RIGHT;
    VICVectCntl2 = (1 << 5) | 16;


    /* Enable EINT0, EINT1, EINT2 */
    VICIntEnable |= (1 << 14) | (1 << 15) | (1 << 16);
}
