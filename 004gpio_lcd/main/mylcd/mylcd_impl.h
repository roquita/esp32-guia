#pragma once

#include "stdint.h"

void lcd_delay_ms_fn(uint8_t ms);
void lcd_delay_us_fn(uint8_t us);
void lcd_rs_signal_set_level_fn(int level);
void lcd_rw_signal_set_level_fn(int level);
void lcd_e_signal_set_level_fn(int level);
void lcd_push_bits_fn(uint8_t four_bits);