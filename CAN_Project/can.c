#include <lpc21xx.h>

#include "can.h"
#include "can_defines.h"


void Init_CAN1(void)
{
    /*
        CAN1_RX = P0.25
        CAN1_TX = dedicated CAN1 TX pin
    */

    PINSEL1 &= ~(CAN1_RD1_MASK);
    PINSEL1 |=  (CAN1_RD1_FUNC);

    /* Reset CAN1 controller */
    C1MOD = 1;

    /* Accept all CAN messages */
    AFMR = 2;

    /* CAN baud rate */
    C1BTR = BTR_LVAL;

    /* Enable CAN1 */
    C1MOD = 0;
}


void CAN1_Tx(struct CAN_Frame txFrame)
{
    /* Wait until TX buffer 1 is available */
    while((C1GSR & TBS1_BIT_READ) == 0);

    /* CAN ID */
    C1TID1 = txFrame.ID;

    /*
        RTR  -> bit 30
        DLC  -> bits 16-19
    */
    C1TFI1 = (txFrame.vbf.RTR << 30) |
             (txFrame.vbf.DLC << 16);

    /*
        Data frame
        (C1TDB1 is only transmitted when DLC > 4)
    */
    if(txFrame.vbf.RTR != 1)
    {
        C1TDA1 = txFrame.Data1;
        C1TDB1 = txFrame.Data2;
    }

    /* Select TX buffer 1 and transmit */
    C1CMR = STB1_BIT_SET | TR_BIT_SET;

    /* Wait until transmission completes */
    while((C1GSR & TCS1_BIT_READ) == 0);
}


void CAN1_Rx(struct CAN_Frame *rxFrame)
{
    /* Wait for received CAN message */
    while((C1GSR & RBS_BIT_READ) == 0);

    /* Read ID */
    rxFrame->ID = C1RID;

    /* Read RTR */
    rxFrame->vbf.RTR =
        (C1RFS >> 30) & 1;

    /* Read DLC */
    rxFrame->vbf.DLC =
        (C1RFS >> 16) & 0x0F;

    /* Data frame */
    if(rxFrame->vbf.RTR == 0)
    {
        rxFrame->Data1 = C1RDA;
        rxFrame->Data2 = C1RDB;
    }

    /* Release RX buffer (registers above were read BEFORE this) */
    C1CMR = RRB_BIT_SET;
}


void CAN1_RecoverOverrun(void)
{
    /*
        If frames arrive faster than the main loop reads them, the
        controller sets the Data Overrun flag and drops frames.  NXP
        forum reports on the related LPC229x CAN controller describe
        reception staying dead after an overrun until the controller
        is reset, so reset it here as a precaution.
    */
#if CAN_OVERRUN_RECOVERY
    if(C1GSR & DOS_BIT_READ)
    {
        C1CMR = CDO_BIT_SET;    /* clear the overrun flag      */
        Init_CAN1();            /* reset + re-initialise CAN1  */
    }
#endif
}
