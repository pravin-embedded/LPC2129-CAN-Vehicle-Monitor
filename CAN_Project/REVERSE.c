/*
    REVERSE / INDICATOR NODE

    Hardware : indicator LEDs P0.4-P0.7 (active low)
               ultrasonic TRIG P0.16, ECHO P0.17
               CAN1 (RX P0.25)

    CAN (Data1 only, DLC = 1, Data2 = 0):
        RX 0x100 : indicator command  <- Main node  (1 = LEFT, 2 = RIGHT)
        TX 0x102 : reverse status     -> Main node  (0 = SAFE, 1 = WARNING, 2 = STOP)
*/

#include <lpc21xx.h>

#include "reverse_def.h"
#include "can.h"
#include "can_ids.h"
#include "can_defines.h"
#include "delay.h"
#include "LED_BLINK.h"
#include "types.h"


int main(void)
{
    u32 distance;
    u8  status;
    u8  cmd;

    CAN_Frame RxMsg;
    CAN_Frame TxMsg;


    /*
    ============================================================
    INITIALIZATION
    ============================================================
    */

    Ultrasonic_Init();

    LED_Init();

    Init_CAN1();

    /*
        Timer0 prescaler for a 1 us tick.
        PCLK = 15 MHz  ->  (14 + 1) / 15 MHz = 1 us
        (the old value 11 assumed 12 MHz and made distances read 23 % high)
    */
    T0PR = 14;


    while(1)
    {
        /*
        ============================================================
        1. RECEIVE INDICATOR COMMAND FROM MAIN
           Empty the whole receive buffer (the Fuel node's 0x101
           frames also arrive here and are simply ignored).
        ============================================================
        */

        while(C1GSR & RBS_BIT_READ)
        {
            CAN1_Rx(&RxMsg);

            if(RxMsg.ID == CAN_ID_INDICATOR)
            {
                cmd = (u8)(RxMsg.Data1 & 0xFF);

                if(cmd == IND_LEFT)
                {
                    Blink_left();
                }
                else if(cmd == IND_RIGHT)
                {
                    Blink_right();
                }
            }
        }

        CAN1_RecoverOverrun();


        /*
        ============================================================
        2. ULTRASONIC / REVERSE STATUS
           (a timeout returns 0 -> STOP : fail-safe behaviour)
        ============================================================
        */

        distance = Ultrasonic_Trigger();

        if(distance > 50)
        {
            status = SAFE;
        }
        else if(distance > 20)
        {
            status = WARNING;
        }
        else
        {
            status = STOP;
        }


        /*
        ============================================================
        3. SEND STATUS TO MAIN
        ============================================================
        */

        TxMsg.ID      = CAN_ID_REVERSE;
        TxMsg.vbf.RTR = 0;
        TxMsg.vbf.DLC = 1;
        TxMsg.Data1   = status;
        TxMsg.Data2   = 0;

        CAN1_Tx(TxMsg);


        /*
            Limit the status rate to ~10 frames/s.
            Also respects the HC-SR04 minimum of ~60 ms between
            measurements, and stops flooding the Main node.
        */
        delay_MS(100);
    }
}
