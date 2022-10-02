#pragma once

#include "stdbool.h"
#include "stdint.h"
/*
This library works in 4 bit mode only:
D0, D1, D2, D3 must be connected to GND.
Others lcd-pins connections can be chosen by user.
*/
typedef enum{DDRAM_DEC,DDRAM_INC}inc_dec_ddram_t;
typedef enum{AUTO_SHIFT_ON,AUTO_SHIFT_OFF}auto_shift_enable_t;
typedef enum{DISPLAY_OFF,DISPLAY_ON}display_enable_t;
typedef enum{CURSOR_OFF,CURSOR_ON}cursor_enable_t;
typedef enum{BLINK_OFF,BLINK_ON}blink_enable_t;
typedef enum{SHIFT_CURSOR, SHIFT_DISPLAY}shift_target_t;
typedef enum{TO_THE_LEFT, TO_THE_RIGHT}shift_direction_t;
typedef enum{FOUR_BIT_LENGTH, EIGHT_BIT_LENGTH}data_length_t;
typedef enum{ONE_LINE_MODE, TWO_LINE_MODE}lines_t;
typedef enum{FONT_5X8, FONT_5X11}font_t;

typedef void (*delay_ms_fn)(uint8_t ms);
typedef void (*delay_us_fn)(uint8_t us);
typedef void (*rs_signal_set_level_fn)(int level);
typedef void (*rw_signal_set_level_fn)(int level);
typedef void (*e_signal_set_level_fn)(int level);
typedef void (*push_bits_fn)(uint8_t four_bits);

typedef struct{
	delay_ms_fn delay_ms;	
	delay_us_fn delay_us;
	rs_signal_set_level_fn rs_signal_set_level;
	rw_signal_set_level_fn rw_signal_set_level;
	e_signal_set_level_fn e_signal_set_level;
	push_bits_fn push_bits;
	lines_t lines;
	font_t font;
}lcd16x2_t;

// native commands
void lcd16x2_init(lcd16x2_t* lcd);
void lcd16x2_clear_display(lcd16x2_t* lcd);
void lcd16x2_return_home(lcd16x2_t* lcd);
void lcd16x2_entry_mode(lcd16x2_t* lcd, inc_dec_ddram_t, auto_shift_enable_t);
void lcd16x2_onoff_control(lcd16x2_t* lcd, display_enable_t, cursor_enable_t, blink_enable_t);
void lcd16x2_cursor_or_display_shift(lcd16x2_t* lcd,shift_target_t,shift_direction_t);
void lcd16x2_function_set(lcd16x2_t* lcd,data_length_t,lines_t,font_t);
void lcd16x2_set_cgram_addr(lcd16x2_t* lcd);
void lcd16x2_set_ddram_addr(lcd16x2_t* lcd, uint8_t addr);
void lcd16x2_read_busy_flag(lcd16x2_t* lcd);
void lcd16x2_write_data(lcd16x2_t* lcd, uint8_t char_addr);
void lcd16x2_read_data(lcd16x2_t* lcd);

// custom functions
void lcd16x2_print_string(lcd16x2_t* lcd, char* str);

