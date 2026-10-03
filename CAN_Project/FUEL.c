/*
    FUEL NODE

    CAN (Data1 only, DLC = 1, Data2 = 0):
        TX 0x101 : fuel percentage 0..100  -> Main node
*/

#include <lpc21xx.h>
#include "delay.h"
#include "types.h"
#include "lcd.h"
#include "ADC_defines.h"
#include "can.h"
#include "can_ids.h"

#define ADC_EMPTY 85
#define ADC_FULL  700

int main(void)
{
    CAN_Frame txMsg;

    u32 dval;
    f32 eAR;
    u32 fuel;

    LCD_Init();
    ADC_Init();
    Init_CAN1();

    while(1)
    {
        ADC_Read(CH0, &dval, &eAR);

        /* Limit ADC value to calibrated sensor range */
        if(dval < ADC_EMPTY)
            dval = ADC_EMPTY;

        if(dval > ADC_FULL)
            dval = ADC_FULL;

        fuel = ((dval - ADC_EMPTY) * 100) / (ADC_FULL - ADC_EMPTY);

        /* Display ADC Value */
        LCD_Cmd(0x80);
        LCD_str("ADC:");
        LCD_U32(dval);
        LCD_str("   ");

        /* Display Fuel Percentage */
        LCD_Cmd(0xC0);
        LCD_str("FUEL:");
        LCD_U32(fuel);
        LCD_Char('%');
        LCD_str("   ");

        /* Low Fuel Warning */
        if(fuel < 20)
        {
            LCD_Cmd(0xCC);      // Position after percentage
            LCD_str("LOW");
        }
        else
        {
            LCD_Cmd(0xCC);
            LCD_str("   ");
        }

        /* Send fuel percentage to Main node */
        txMsg.ID      = CAN_ID_FUEL;
        txMsg.vbf.RTR = 0;
        txMsg.vbf.DLC = 1;
        txMsg.Data1   = fuel;
        txMsg.Data2   = 0;

        CAN1_Tx(txMsg);

        delay_MS(200);
    }
}
