//lcd.h
#include <lpc21xx.h>

// Only Prototypes go here (Notice the semicolons!)
void LCD_Init(void);
void LCD_Cmd(unsigned int cmd);
void LCD_Char(unsigned char Data);
void LCD_str( char *str);
void LCD_U32(unsigned int num);
void LCD_F32(float f);
void LCD_2Digit(unsigned int n);
void LCD_Num(unsigned int num);


