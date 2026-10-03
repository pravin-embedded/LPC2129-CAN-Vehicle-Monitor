#ifndef __DS18B20_H__
#define __DS18B20_H__

unsigned char DS18B20_StartConversion(void);
void DS18B20_ReadRaw(unsigned char *LSB, unsigned char *MSB);
float DS18B20_ReadTemperature(void);

#endif
