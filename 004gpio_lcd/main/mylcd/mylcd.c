#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lcd.h"
#include "mylcd_impl.h"
#include "driver/i2c.h"
#include "i2c_adapter.h"

lcd16x2_t mylcd;

void mylcd_init()
{
    // LCD
    i2c_adapter_init();

    mylcd.delay_ms = lcd_delay_ms_fn;
    mylcd.delay_us = lcd_delay_us_fn;
    mylcd.rs_signal_set_level = lcd_rs_signal_set_level_fn;
    mylcd.rw_signal_set_level = lcd_rw_signal_set_level_fn;
    mylcd.e_signal_set_level = lcd_e_signal_set_level_fn;
    mylcd.push_bits = lcd_push_bits_fn;
    mylcd.lines = TWO_LINE_MODE;
    mylcd.font = FONT_5X8;

    lcd16x2_init(&mylcd);
}
void mylcd_printCounter(int counter)
{
    char texto[20] = {0};
    sprintf(texto, "contador=%i", counter);
    lcd16x2_set_ddram_addr(&mylcd, 0x40);
    lcd16x2_print_string(&mylcd, texto);
}