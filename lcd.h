

#ifndef LCD_H
#define LCD_H

#include <avr/io.h>

void lcd_init(void);

void lcd_clearScreen(void);

void lcd_barGraph(uint8_t *);


#endif /* LCD_H */
