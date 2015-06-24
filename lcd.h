

#ifndef LCD_H
#define LCD_H

#include <avr/io.h>

void lcd_init(void);

void lcd_clearScreen(void);

void lcd_writeBuffer(uint8_t *);




#endif /* LCD_H */
