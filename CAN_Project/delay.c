#include "type.h"
#include "delay.h"
void delay_US(u32 delayus)
{
	delayus*=12;
	while(delayus--);
}
void delay_MS(u32 delayms)
{
	delayms*=12000;
	while(delayms--);
}
