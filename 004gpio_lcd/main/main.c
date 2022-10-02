#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
//#include "driver/gptimer.h"
#include "lcd.h"

//#include "rom/ets_sys.h"

//#define ESP_INTR_FLAG_DEFAULT 0

lcd16x2_t lcd;

void lcd_delay_ms_fn(uint8_t ms)
{
    for (uint8_t i = 0; i < ms; i++)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
void lcd_delay_us_fn(uint8_t us)
{
    vTaskDelay(pdMS_TO_TICKS(1));
}
void lcd_rs_signal_set_level_fn(int level)
{
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
 four_bits =   b7 b6 b5 b4 b3 b2 b1 b0

    b0 -> D4
    b1 -> D5
    b2 -> D6
    b3 -> D7
   */
    uint32_t d4_level = four_bits & 0b1;
    uint32_t d5_level = (four_bits >> 1) & 0b1;
    uint32_t d6_level = (four_bits >> 2) & 0b1;
    uint32_t d7_level = (four_bits >> 3) & 0b1;

    gpio_set_level(19, d4_level);
    gpio_set_level(21, d5_level);
    gpio_set_level(22, d6_level);
    gpio_set_level(23, d7_level);
}

/*
bool lcd_flag = false;
int counter = 0;
static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    // ets_printf("hola\r\n");
    // lcd_flag = true;
}
*/

/*
static bool IRAM_ATTR example_timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    // ets_printf("hola\r\n");
    // lcd_flag = true;
    // return false;
}
*/
void app_main(void)
{
    /*
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };
    gptimer_new_timer(&timer_config, &gptimer);

    gptimer_event_callbacks_t cbs = {
        .on_alarm = example_timer_on_alarm_cb,
    };
    gptimer_register_event_callbacks(gptimer, &cbs, NULL);
    gptimer_enable(gptimer);
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = 1000000, // period = 1s
        .flags.auto_reload_on_alarm = true,
        .reload_count = 0,
    };
    gptimer_set_alarm_action(gptimer, &alarm_config);
    gptimer_start(gptimer);
*/

/*
    const gpio_config_t config_boton = {
        .pin_bit_mask = (uint64_t)(1 << 0),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE,
    };

    gpio_config(&config_boton);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(0, gpio_isr_handler, NULL);
*/

    const gpio_config_t config_pines = {
        .pin_bit_mask = (uint64_t)((1 << 4) | (1 << 14) | (1 << 18) | (1 << 19) | (1 << 21) | (1 << 22) | (1 << 23)),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&config_pines);

    lcd.delay_ms = lcd_delay_ms_fn;
    lcd.delay_us = lcd_delay_us_fn;
    lcd.rs_signal_set_level = lcd_rs_signal_set_level_fn;
    lcd.rw_signal_set_level = lcd_rw_signal_set_level_fn;
    lcd.e_signal_set_level = lcd_e_signal_set_level_fn;
    lcd.push_bits = lcd_push_bits_fn;
    lcd.lines = TWO_LINE_MODE;
    lcd.font = FONT_5X8;

    lcd16x2_init(&lcd);    
    lcd16x2_set_ddram_addr(&lcd, 0x40);
    lcd16x2_print_string(&lcd, "LCD esp32");

    printf("loop\n\r");

/*
    while (1)
    {
        if (lcd_flag == true)
        {
            lcd_flag = false;
            lcd16x2_set_ddram_addr(&lcd, 0x00);
            lcd16x2_write_data(&lcd, counter + 48);
            counter++;
            counter = (counter >= 10) ? 0 : counter;
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
    */
}
