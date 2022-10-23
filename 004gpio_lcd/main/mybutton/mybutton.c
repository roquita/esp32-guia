#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern bool gpio_flag ;
void gpio0_isr(void *arg)
{

    // ets_printf("gpio0_isr\n\r");
    gpio_flag = true;
}

void mybutton_init()
{
    // BOTON
    gpio_config_t boton_config = {
        .intr_type = GPIO_INTR_POSEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (uint64_t)1 << 0,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    gpio_config(&boton_config);

    gpio_install_isr_service(0); // prioridad
    gpio_isr_handler_add(GPIO_NUM_0, gpio0_isr, NULL);
}