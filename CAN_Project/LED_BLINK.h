//LED_BLINK.h
#ifndef __LED_BLINK_H__
#define __LED_BLINK_H__

/* Reverse status values (CAN 0x102 Data1) */
#define SAFE        0
#define WARNING     1
#define STOP        2

/* Indicator commands (CAN 0x100 Data1) */
#define IND_OFF     0
#define IND_LEFT    1
#define IND_RIGHT   2

/* Vehicle mode (owned by Main node) */
#define FORWARD     0
#define REVERSE     1

/* Ultrasonic pins (Reverse node) */
#define TRIG        (1<<16)
#define ECHO        (1<<17)
#define TIMEOUT     50000

void Blink_left(void);
void Blink_right(void);
void LED_Init(void);

void Ultrasonic_Init(void);
unsigned int Ultrasonic_Trigger(void);

void timer_us(void);

#endif
