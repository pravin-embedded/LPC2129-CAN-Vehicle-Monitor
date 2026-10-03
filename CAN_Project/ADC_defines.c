//adc_defines.c
#include <lpc21xx.h>
#include "delay.h"
#include "ADC_defines.h"
void ADC_Init(void)
{
	PINSEL1|=0x15400000;
	ADCR = PDN_BIT|CLKDIV_VALUE;
}
void ADC_Read(u32 CHN0, u32 *AdcDval,f32* eAR)
{
	ADCR&=~(255<<0);
	ADCR|=CHN0|START_CONV;
	delay_US(10);
	while(((ADDR>>DONE_BIT)&1)==0);
	ADCR&=~(START_CONV);
	*AdcDval=((ADDR >> RESULT)&1023);
	*eAR=(3.3/1024.0f)*(*AdcDval);
}

