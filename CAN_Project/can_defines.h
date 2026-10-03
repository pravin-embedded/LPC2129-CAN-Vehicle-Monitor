#ifndef __CAN_DEFINES_H__
#define __CAN_DEFINES_H__

/*
    ------------------------------------------------------------------
    CAN1 RX pin : P0.25 -> RD1
    PINSEL1 bits 19:18 control P0.25   (01 = RD1)
    CAN1 TX is a dedicated pin, no PINSEL setting needed.
    ------------------------------------------------------------------
*/
#define RD1_PIN         0x00040000
#define CAN1_RD1_MASK   (3u << 18)
#define CAN1_RD1_FUNC   (1u << 18)


/*
    ------------------------------------------------------------------
    CAN BIT TIMING

    FOSC = 12 MHz, CCLK = 60 MHz (PLL x5)
    VPBDIV = 0  ->  PCLK = CCLK / 4 = 15 MHz   (this is what the CAN
    controller is really clocked from)

    125 kbps  ->  PCLK / BIT_RATE = 120 = BRP * QUANTA
    QUANTA = 20  ->  BRP = 6   (QUANTA = 16 would need BRP = 7.5: impossible)

    Sample point = 70 %  ->  TSEG1 = 13 tq, TSEG2 = 6 tq, SJW = 4 tq
    BTR_LVAL     = 0x005CC005

    ALL THREE NODES MUST USE THE SAME VALUES.
    ------------------------------------------------------------------
*/
#define PCLK        15000000
#define BIT_RATE    125000
#define QUANTA      20

#if (PCLK % (BIT_RATE * QUANTA)) != 0
#error "CAN: PCLK / (BIT_RATE * QUANTA) is not an integer - choose another QUANTA"
#endif

#define BRP             (PCLK / (BIT_RATE * QUANTA))

/* integer maths only (no floating point constant folding) */
#define SAMPLE_POINT_TQ ((QUANTA * 7) / 10)         /* 70 % */
#define TSEG1           (SAMPLE_POINT_TQ - 1)       /* prop_seg + phase_seg1 */
#define TSEG2           (QUANTA - (1 + TSEG1))      /* phase_seg2            */
#define SJW             ((TSEG2 >= 5) ? 4 : (TSEG2 - 1))
#define SAM             0                           /* sample bus once       */

#if (TSEG1 > 16) || (TSEG2 > 8) || (TSEG2 < 2) || (BRP > 1024)
#error "CAN: bit timing outside LPC2129 limits"
#endif

#define BTR_LVAL    (SAM << 23 |            \
                     (TSEG2 - 1) << 20 |    \
                     (TSEG1 - 1) << 16 |    \
                     (SJW - 1)   << 14 |    \
                     (BRP - 1))


/*
    Fallback: if the controller reports a data overrun, reset and re-init
    CAN1.  Set to 0 to disable it while debugging (CAN then behaves exactly
    like the plain reference driver).
*/
#define CAN_OVERRUN_RECOVERY    1


/* C1CMR command bits */
#define TR_BIT_SET      (1<<0)      /* transmit request          */
#define RRB_BIT_SET     (1<<2)      /* release receive buffer    */
#define CDO_BIT_SET     (1<<3)      /* clear data overrun        */
#define STB1_BIT_SET    (1<<5)      /* select TX buffer 1        */

/* C1GSR status bits */
#define RBS_BIT_READ    (1<<0)      /* receive buffer status     */
#define DOS_BIT_READ    (1<<1)      /* data overrun status       */
#define TBS1_BIT_READ   (1<<2)      /* TX buffer 1 status        */
#define TCS1_BIT_READ   (1<<3)      /* TX complete status        */

#endif
