#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "rom/ets_sys.h"
#include "driver/gptimer.h"

extern bool gpio_flag ;


bool timer_ov_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    //ets_printf("timer_isr\n\r");
    gpio_flag = true;
    
    return true;
}

void mytimer_init()
{
    // TIMER

    // CLOCKS
    gptimer_handle_t gptimer = NULL;

    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };
    gptimer_new_timer(&timer_config, &gptimer);

    // REGISTRO DE EVENTOS
    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_ov_cb,
    };
    gptimer_register_event_callbacks(gptimer, &cbs, NULL);
    gptimer_enable(gptimer);

    // ALARMAS
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = 1000000, // period = 1s
        .flags.auto_reload_on_alarm = true,
        .reload_count = 0,
    };
    gptimer_set_alarm_action(gptimer, &alarm_config);

    // INICIO(OPCIONAL)
    gptimer_start(gptimer);
}