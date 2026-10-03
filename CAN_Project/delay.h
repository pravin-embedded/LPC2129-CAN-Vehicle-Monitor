/*
    Simple busy-wait delays.

    Both routines are LOOP-COUNT BASED, not timer based. The iteration counts
    (* 12 and * 12000) were tuned for this project's clock configuration
    (CCLK = 60 MHz, PCLK = 15 MHz), so the real delay depends on:

      * the actual CCLK - a lower clock stretches every delay proportionally
      * the compiler optimisation level, since it changes how many CPU cycles
        one loop iteration takes

    Changing the optimisation setting in the Keil project, or building for a
    different clock, changes the timing of every delay_MS()/delay_US() call in
    the firmware (1-Wire bit timing, LCD strobe delays, frame pacing, and the
    ultrasonic trigger pulse). Re-measure before relying on them.
*/
#include "type.h"
void delay_US(u32 delayus);

void delay_MS(u32 delayms);
