#ifndef __CAN_IDS_H__
#define __CAN_IDS_H__

/*
    Shared CAN message map.
    Use this SAME file, unmodified, on Main, Reverse and Fuel nodes.

    Only Data1 (low byte) is used.  DLC = 1.  Data2 is always 0.
*/

/* Main    -> Reverse : Data1 = IND_LEFT (1) / IND_RIGHT (2)        */
#define CAN_ID_INDICATOR    0x100

/* Fuel    -> Main    : Data1 = fuel percentage 0..100              */
#define CAN_ID_FUEL         0x101

/* Reverse -> Main    : Data1 = SAFE (0) / WARNING (1) / STOP (2)   */
#define CAN_ID_REVERSE      0x102

#endif
