#ifndef LIQUIDCRYSTAL_I2C_H_
#define LIQUIDCRYSTAL_I2C_H_

#include "port_system.h"

/* Command */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

/* Entry Mode */
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/* Display On/Off */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

/* Cursor Shift */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

/* Function Set */
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

/// @brief Backlight on bit
#define LCD_BACKLIGHT 0x08

/// @brief Backlight off bit
#define LCD_NOBACKLIGHT 0x00

/// @brief Enable Bit
#define ENABLE 0x04

/// @brief Read Write Bit
#define RW 0x0

/// @brief Register Select Bit
#define RS 0x01

/// @brief I2C address shifted one bit to the left as a 7 bit address is expected
#define DEVICE_ADDR     (0x27 << 1)

/// @brief 
/// @param rows 
void port_lcd_init(uint8_t rows);

/// @brief 
void port_lcd_clear();

/// @brief 
void port_lcd_home();

/// @brief 
void port_lcd_no_display();

/// @brief 
void port_lcd_display();

/// @brief 
void port_lcd_no_blink();

/// @brief 
void port_lcd_blink();

/// @brief 
void port_lcd_no_cursor();

/// @brief 
void port_lcd_cursor();

/// @brief 
void port_lcd_scroll_display_left();

/// @brief 
void port_lcd_scroll_display_right();

/// @brief 
void port_lcd_print_left();

/// @brief 
void port_lcd_print_right();

/// @brief 
void port_lcd_left_to_right();

/// @brief 
void port_lcd_right_to_left();

/// @brief 
void port_lcd_shift_increment();

/// @brief 
void port_lcd_shift_decrement();

/// @brief 
void port_lcd_no_backlight();

/// @brief 
void port_lcd_backlight();

/// @brief 
void port_lcd_autoscroll();

/// @brief 
void port_lcd_no_autoscroll();

/// @brief 
/// @param  
/// @param  
void port_lcd_create_special_char(uint8_t, uint8_t[]);

/// @brief 
/// @param  
void port_lcd_print_special_char(uint8_t);

/// @brief 
/// @param  
/// @param  
void port_lcd_set_cursor(uint8_t, uint8_t);

/// @brief 
/// @param new_val 
void port_lcd_set_backlight(uint8_t new_val);

/// @brief 
/// @param char_num 
/// @param rows 
void port_lcd_load_custom_character(uint8_t char_num, uint8_t *rows);

/// @brief 
/// @param  
void port_lcd_print_str(const char[]);

#endif /* LIQUIDCRYSTAL_I2C_H_ */
