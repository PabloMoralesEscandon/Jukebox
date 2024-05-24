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

/// @brief Initializes lcd screen
/// @param rows 
void port_lcd_init(uint8_t rows);

/// @brief Clears lcd screen
void port_lcd_clear();

/// @brief Returns lcd to home state
void port_lcd_home();

/// @brief Turns off lcd display
void port_lcd_no_display();

/// @brief Turns on lcd display
void port_lcd_display();

/// @brief Turns off cursor blink on the lcd display
void port_lcd_no_blink();

/// @brief Turns on cursor blink on the lcd display
void port_lcd_blink();

/// @brief Does not show cursor on the lcd display
void port_lcd_no_cursor();

/// @brief Shows cursor on the lcd display
void port_lcd_cursor();

/// @brief Scrolls lcd display to the left
void port_lcd_scroll_display_left();

/// @brief Scrolls lcd display to the right
void port_lcd_scroll_display_right();

/// @brief Sets print to the left
void port_lcd_print_left();

/// @brief Sets print to the right
void port_lcd_print_right();

/// @brief Sets print from left to right
void port_lcd_left_to_right();

/// @brief Sets print from right to left
void port_lcd_right_to_left();

/// @brief INcrements lcd shift
void port_lcd_shift_increment();

/// @brief Decrements lcd shift
void port_lcd_shift_decrement();

/// @brief Turns off lcd backlight
void port_lcd_no_backlight();

/// @brief Turns on lcd backlight
void port_lcd_backlight();

/// @brief Turns on lcd autoscoll
void port_lcd_autoscroll();

/// @brief Turns off lcd autoscoll
void port_lcd_no_autoscroll();

/// @brief Creates special custom character on the lcd screen
/// @param  id Character id
/// @param  info Character memory info
void port_lcd_create_special_char(uint8_t id, uint8_t[]);

/// @brief Prints custom character
/// @param  uint8_t Char id
void port_lcd_print_special_char(uint8_t id);

/// @brief Sets lcd cursor at desired position
/// @param  col Cursor column
/// @param  row Cursor row
void port_lcd_set_cursor(uint8_t col, uint8_t row);

/// @brief Sets backlight on or off
/// @param new_val On or off
void port_lcd_set_backlight(uint8_t new_val);

/// @brief Loads custom character from memory
/// @param char_num Character id
/// @param rows Number of rows
void port_lcd_load_custom_character(uint8_t char_num, uint8_t *rows);

/// @brief Prints a string on the lcd
/// @param  String
void port_lcd_print_str(const char[] );

#endif /* LIQUIDCRYSTAL_I2C_H_ */
