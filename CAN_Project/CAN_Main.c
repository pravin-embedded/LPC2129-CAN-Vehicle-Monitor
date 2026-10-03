/*
    MAIN NODE

    LCD (20x4):
        Row 1 : TEMP
        Row 2 : FUEL
        Row 3 : MODE
        Row 4 : REV status (only meaningful in REVERSE mode)

    CAN (Data1 only, DLC = 1, Data2 = 0):
        TX 0x100 : indicator command  -> Reverse node
        RX 0x101 : fuel percentage    <- Fuel node
        RX 0x102 : reverse status     <- Reverse node
*/

#include <lpc21xx.h>

#include "lcd.h"
#include "LED_BLINK.h"
#include "can.h"
#include "can_ids.h"
#include "can_defines.h"
#include "ds18b20.h"
#include "onewire.h"
#include "delay.h"
#include "types.h"
#include "switch.h"


/*
    Modified by the external interrupt routines in switch.c.
*/
volatile u8 indicator = IND_OFF;
volatile u8 mode      = FORWARD;


/*
    Latest values received over CAN.
*/
static u32 fuel           = 0;
static u8  reverse_status = SAFE;


/*
    ------------------------------------------------------------------
    Service_CAN()

    Empty the CAN receive buffer completely, then recover from a data
    overrun if one happened.  Called often (every ~10 ms) so frames from
    the Fuel and Reverse nodes are never left waiting in the controller.
    ------------------------------------------------------------------
*/
static void Service_CAN(void)
{
    CAN_Frame rx;

    while(C1GSR & RBS_BIT_READ)
    {
        CAN1_Rx(&rx);

        if(rx.ID == CAN_ID_FUEL)
        {
            fuel = rx.Data1 & 0xFF;
        }
        else if(rx.ID == CAN_ID_REVERSE)
        {
            reverse_status = (u8)(rx.Data1 & 0xFF);
        }
    }

    CAN1_RecoverOverrun();
}


/*
    ------------------------------------------------------------------
    Service_Indicator()

    FORWARD : send the pending LEFT/RIGHT command (once) to the Reverse
              node.
    REVERSE : indicator presses are ignored and discarded, so they do
              not fire later when the mode returns to FORWARD.
    ------------------------------------------------------------------
*/
static void Service_Indicator(void)
{
    CAN_Frame tx;
    u8 cmd;

    cmd = indicator;

    if(cmd == IND_OFF)
        return;

    indicator = IND_OFF;            /* consume the event */

    if(mode == FORWARD)
    {
        tx.ID      = CAN_ID_INDICATOR;
        tx.vbf.RTR = 0;
        tx.vbf.DLC = 1;
        tx.Data1   = cmd;
        tx.Data2   = 0;

        CAN1_Tx(tx);
    }
}


static void Service_Background(void)
{
    Service_CAN();
    Service_Indicator();
}


/*
    ------------------------------------------------------------------
    Temp_Read()

    Same sequence as DS18B20_ReadTemperature() in ds18b20.c
    (reset, skip ROM, convert, wait 750 ms, read scratchpad), but the
    750 ms wait is split into 10 ms slices so CAN reception and
    indicator commands keep being serviced during the conversion.

    Returns -1000.0 if the sensor does not answer.
    ------------------------------------------------------------------
*/
static f32 Temp_Read(void)
{
    unsigned char lsb;
    unsigned char msb;
    short raw;
    u8 i;

    if(!OneWire_Reset())
        return -1000.0f;

    OneWire_WriteByte(0xCC);        /* skip ROM        */
    OneWire_WriteByte(0x44);        /* convert T       */

    for(i = 0; i < 75; i++)         /* 75 x 10 ms = 750 ms */
    {
        delay_MS(10);
        Service_Background();
    }

    DS18B20_ReadRaw(&lsb, &msb);

    raw = (short)(((short)msb << 8) | lsb);

    return (f32)raw / 16.0f;
}


int main(void)
{
    f32 temp;

    LCD_Init();

    Switch_Init();

    Init_CAN1();


    while(1)
    {
        /*
        ============================================================
        1. ENGINE TEMPERATURE  (CAN + indicators serviced inside)
        ============================================================
        */

        temp = Temp_Read();

        if(temp < -999.0f)
        {
            LCD_Cmd(0x80);
            LCD_str("TEMP: SENSOR ERR ");
        }
        else
        {
            LCD_Cmd(0x80);
            LCD_str("TEMP:");

            LCD_Cmd(0x85);
            LCD_F32(temp);

            LCD_Char(176);
            LCD_Char('C');

            LCD_str("    ");
        }

        Service_Background();


        /*
        ============================================================
        2. LCD FUEL
        ============================================================
        */

        LCD_Cmd(0xC0);
        LCD_str("FUEL:     ");

        LCD_Cmd(0xC5);
        LCD_U32(fuel);
        LCD_Char('%');

        Service_Background();


        /*
        ============================================================
        3. LCD MODE
        ============================================================
        */

        LCD_Cmd(0x94);
        LCD_str("MODE:");

        if(mode == FORWARD)
        {
            LCD_str("FORWARD ");
        }
        else
        {
            LCD_str("REVERSE ");
        }

        Service_Background();


        /*
        ============================================================
        4. LCD REVERSE STATUS
        ============================================================
        */

        LCD_Cmd(0xD4);
        LCD_str("REV: ");

        if(mode == FORWARD)
        {
            LCD_str("-----   ");
        }
        else
        {
            switch(reverse_status)
            {
                case SAFE:
                    LCD_str("SAFE    ");
                    break;

                case WARNING:
                    LCD_str("WARNING ");
                    break;

                case STOP:
                    LCD_str("STOP    ");
                    break;

                default:
                    LCD_str("-----   ");
                    break;
            }
        }

        Service_Background();
    }
}
