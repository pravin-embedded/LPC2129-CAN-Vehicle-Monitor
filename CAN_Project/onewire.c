//onewire.c
#include <lpc21xx.h>
#include "onewire.h"
#include "delay.h"
void OneWire_Output(void)
{
	IODIR0|= (1<<OW_PIN);
}

void OneWire_Input(void)
{
	IODIR0&=~(1<<OW_PIN);
}

unsigned char OneWire_Reset(void)
{
	OneWire_Output();
	IOCLR0 = (1<<OW_PIN);
	delay_US(480);
	OneWire_Input();
	delay_US(60);
	if((IOPIN0 & (1<<OW_PIN))==0)
	{
		delay_US(240);
		return 1;
	}
	else
	{
		delay_US(240);
		return 0;
	}
}

void OneWire_WriteBit(unsigned char bit)
{
	if(bit==1)
	{
		OneWire_Output();
		IOCLR0 = (1 << OW_PIN);
		delay_US(6);
		OneWire_Input();
		delay_US(54);
	}
	else
	{
		OneWire_Output();
		IOCLR0 = (1 << OW_PIN);
		delay_US(60);
		OneWire_Input();
		delay_US(2);
	}
}

unsigned char OneWire_ReadBit(void)
{
		unsigned char bit;
		OneWire_Output();
		IOCLR0 = (1 << OW_PIN);
		delay_US(6);
		OneWire_Input();
		delay_US(9);
		bit=((IOPIN0 >> OW_PIN)&1);
		delay_US(55);
		return bit;
}

void OneWire_WriteByte(unsigned char data)
{
	int i;
	for(i=0;i<8;i++)
	{
		OneWire_WriteBit(data & 1);
		data>>=1;
	}
}

unsigned char OneWire_ReadByte(void)
{
	int i;
	unsigned char data=0,bit;
	for(i=0;i<8;i++)
	{
		bit=OneWire_ReadBit();
		data|=(bit<<i);
	}
	return data;
}
