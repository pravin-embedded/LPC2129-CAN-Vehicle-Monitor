#ifndef __CAN_H__
#define __CAN_H__

#include "types.h"

typedef struct CAN_Frame
{
    u32 ID;

    struct BitField
    {
        u8 RTR : 1;
        u8 DLC : 4;
    } vbf;

    u32 Data1;
    u32 Data2;

} CAN_Frame;

void Init_CAN1(void);
void CAN1_Tx(CAN_Frame);
void CAN1_Rx(CAN_Frame *);

/*
    Call regularly from the main loop AFTER draining the receive buffer.
    If the controller flagged a data overrun, clear it and re-initialise
    CAN1 so reception can never stay stuck.
*/
void CAN1_RecoverOverrun(void);

#endif
