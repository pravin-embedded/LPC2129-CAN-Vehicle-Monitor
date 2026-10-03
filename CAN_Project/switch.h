#ifndef __SWITCH_H__
#define __SWITCH_H__

/*
    P0.1 -> EINT0 -> MODE
    P0.3 -> EINT1 -> LEFT  indicator
    P0.7 -> EINT2 -> RIGHT indicator

    The ISRs in switch.c update the globals "indicator" and "mode",
    which are defined in CAN_Main.c.
*/
void Switch_Init(void);

#endif
