//onewire.h
#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

/*
    DS18B20 DQ pin = P0.<OW_PIN>   (needs 4.7k pull-up to 3.3V)
    P0.17 is free on the Main node ONLY if the LCD RW pin is tied to GND.
    If your DQ wire is on another pin, change this one number.
*/
#define OW_PIN 17


void OneWire_Output(void);
void OneWire_Input(void);

unsigned char OneWire_Reset(void);

void OneWire_WriteBit(unsigned char bit);
unsigned char OneWire_ReadBit(void);

void OneWire_WriteByte(unsigned char data);
unsigned char OneWire_ReadByte(void);

#endif
