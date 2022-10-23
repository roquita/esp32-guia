#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
#include "lcd.h"

/*
rs = gpio4
rw = gpio14
e = gpio 18
d4 = gpio19
d5 = gpio21
d6 = gpio22
d7 = gpio23
*/
void lcd_delay_ms_fn(uint8_t ms)
{
    for (uint8_t i = 0; i < ms; i++)        
        vTaskDelay(pdMS_TO_TICKS(1));
    
}
void lcd_delay_us_fn(uint8_t us)
{
    for (uint8_t i = 0; i < us; i++)
        ets_delay_us(1);
}
void lcd_rs_signal_set_level_fn(int level)
{
    /*
    if (level == 0)
    {
        gpio_set_level(4, 0);
    }else{
          gpio_set_level(4, 1);
    }
    */
    gpio_set_level(4, level == 0 ? 0 : 1);
}
void lcd_rw_signal_set_level_fn(int level)
{
    gpio_set_level(14, level == 0 ? 0 : 1);
}
void lcd_e_signal_set_level_fn(int level)
{
    gpio_set_level(18, level == 0 ? 0 : 1);
}
void lcd_push_bits_fn(uint8_t four_bits)
{
    /*
    four bits => b7 b6 b5 b4 b3 b2 b1 b0
    b0 -> D4
    b1 -> D5
    b2 -> D6
    b3 -> D7
    */

    int d4_level = four_bits & 0b1;
    int d5_level = (four_bits >> 1) & 0b1;
    int d6_level = (four_bits >> 2) & 0b1;
    int d7_level = (four_bits >> 3) & 0b1;

    gpio_set_level(19, d4_level);
    gpio_set_level(21, d5_level);
    gpio_set_level(22, d6_level);
    gpio_set_level(23, d7_level);
}