//ds18b20.c
#include "onewire.h"
#include "delay.h"
unsigned char DS18B20_StartConversion(void)
{
    if(!OneWire_Reset())
        return 0;

    OneWire_WriteByte(0xCC);
    OneWire_WriteByte(0x44);

    delay_MS(750);

    return 1;
}

void DS18B20_ReadRaw(unsigned char *LSB, unsigned char *MSB)
{
	OneWire_Reset();
	OneWire_WriteByte(0xCC);
	OneWire_WriteByte(0xBE);
	*LSB = OneWire_ReadByte();
	*MSB = OneWire_ReadByte();
}

float DS18B20_ReadTemperature(void)
{
	unsigned char LSB, MSB;
	short raw;
	if(!DS18B20_StartConversion())
	{
			return -1000.0;
	}
	DS18B20_ReadRaw(&LSB, &MSB);
	raw = ((short)MSB << 8) | LSB;
	return raw /16.0;
}

