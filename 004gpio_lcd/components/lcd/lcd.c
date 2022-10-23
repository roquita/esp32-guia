#include "lcd.h"

static void send_command(lcd16x2_t* lcd, uint8_t cmd, bool just_upper_bits){
	lcd->rs_signal_set_level(0); //command mode
	lcd->rw_signal_set_level(0); // write mode
	
	// push 4 bites in the bus
	lcd->push_bits(cmd >> 4);
	
	// shift out bits
	lcd->e_signal_set_level(0);
	lcd->delay_us(1);
	lcd->e_signal_set_level(1);
	lcd->delay_us(1);
	lcd->e_signal_set_level(0);
	lcd->delay_us(100);
	
	if (!just_upper_bits)
	{
		// push 4 bites in the bus
		lcd->push_bits(cmd & 0xf);
		
		// shift out bits
		lcd->e_signal_set_level(0);
		lcd->delay_us(1);
		lcd->e_signal_set_level(1);
		lcd->delay_us(1);
		lcd->e_signal_set_level(0);
		lcd->delay_us(100);
	}

}

static void send_data(lcd16x2_t* lcd, uint8_t data){
	lcd->rs_signal_set_level(1); // data mode
	lcd->rw_signal_set_level(0); // write mode	
	
	// push 4 bites in the bus
	lcd->push_bits(data >> 4);
	
	// shift out bits
	lcd->e_signal_set_level(0);
	lcd->delay_us(1);
	lcd->e_signal_set_level(1);
	lcd->delay_us(1);
	lcd->e_signal_set_level(0);
	lcd->delay_us(100);
	
	// push 4 bites in the bus
	lcd->push_bits(data & 0xf);
	
	// shift out bits
	lcd->e_signal_set_level(0);
	lcd->delay_us(1);
	lcd->e_signal_set_level(1);
	lcd->delay_us(1);
	lcd->e_signal_set_level(0);
	lcd->delay_us(100);	
}

void lcd16x2_init(lcd16x2_t* lcd){
	lcd->delay_ms(50);
	send_command(lcd,0x30, true);
	lcd->delay_ms(5);
	send_command(lcd,0x30, true);
	lcd->delay_ms(1);
	send_command(lcd,0x30, true);
	lcd->delay_ms(1);
	send_command(lcd,0x20, true);
	lcd->delay_ms(1);
	
	lcd16x2_function_set(lcd, FOUR_BIT_LENGTH, lcd->lines, lcd->font);
	lcd16x2_onoff_control(lcd, DISPLAY_OFF, CURSOR_OFF, BLINK_OFF);
	lcd16x2_clear_display(lcd);
	lcd16x2_entry_mode(lcd, DDRAM_INC, AUTO_SHIFT_ON);

	lcd16x2_onoff_control(lcd, DISPLAY_ON, CURSOR_OFF, BLINK_OFF);
}

void lcd16x2_clear_display(lcd16x2_t* lcd){
	uint8_t cmd = 0x01;
	send_command(lcd,cmd, false);
	lcd->delay_ms(3);
}
void lcd16x2_return_home(lcd16x2_t* lcd){
	uint8_t cmd = 0x02;
	send_command(lcd,cmd, false);
	lcd->delay_ms(3);
}
void lcd16x2_entry_mode(lcd16x2_t* lcd, inc_dec_ddram_t ddram, auto_shift_enable_t auto_shift){
	uint8_t cmd = (1<<2) | (ddram<<1) | (auto_shift<<0);
	send_command(lcd,cmd, false);
	lcd->delay_us(100);
} 

void lcd16x2_onoff_control(lcd16x2_t* lcd, display_enable_t display, cursor_enable_t cursor, blink_enable_t blink){
	uint8_t cmd = (1<<3) | (display<<2) | (cursor<<1) | (blink<<0);
	send_command(lcd,cmd, false);
	lcd->delay_us(100);
}

void lcd16x2_cursor_or_display_shift(lcd16x2_t* lcd,shift_target_t target,shift_direction_t direction){
	uint8_t cmd = (1<<4) | (target<<3) | (direction<<2);
	send_command(lcd,cmd, false);
	lcd->delay_us(100);
}

void lcd16x2_function_set(lcd16x2_t* lcd,data_length_t length,lines_t lines,font_t font){
	uint8_t cmd = (1<<5) | (length<<4) | (lines<<3) | (font<<2);
	send_command(lcd,cmd, false);
	lcd->delay_us(100);
}

void lcd16x2_set_cgram_addr(lcd16x2_t* lcd){
	
}

void lcd16x2_set_ddram_addr(lcd16x2_t* lcd, uint8_t addr){
	uint8_t cmd = (1<<7) | addr;
	send_command(lcd,cmd, false);
	lcd->delay_us(100);
}

void lcd16x2_read_busy_flag(lcd16x2_t* lcd){
	
}

void lcd16x2_write_data(lcd16x2_t* lcd, uint8_t char_addr){
	send_data(lcd,char_addr);
	lcd->delay_us(100);
}

void lcd16x2_read_data(lcd16x2_t* lcd){
}

void lcd16x2_print_string(lcd16x2_t* lcd,char* str){	
	while(*str != 0){
		lcd16x2_write_data(lcd,*str);
		str++;
	}
}