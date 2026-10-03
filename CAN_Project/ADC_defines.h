#include "types.h"
//ADC_defines.h
#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)
#define ADCLK 3000000
#define DIVIDER ((PCLK/ADCLK)-1)

// ADC SFRs
#define CLKDIV_VALUE (DIVIDER<<8)
#define PDN_BIT (1<<21)
#define START_CONV (1<<24)

//ADDR
#define RESULT 6
#define DONE_BIT 31

//channel define
#define CH0 (1<<0)
#define CH1 (1<<1)
#define CH2 (1<<2)
#define CH3 (1<<3)

void ADC_Read(u32 CHN0, u32 *AdcDval,f32* eAR);
void ADC_Init(void);


