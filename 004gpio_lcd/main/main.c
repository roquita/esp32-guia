#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mybutton.h"
#include "mytimer.h"
#include "mylcd.h"
#include "i2c_adapter.h"

bool gpio_flag = false;
int contador = 0;

esp_err_t i2c_read(uint8_t *data);
esp_err_t i2c_write(uint8_t data);


void app_main(void)
{
    mybutton_init();
    mylcd_init();
    mytimer_init();

    while (1)
    {
        
        if (gpio_flag == true)
        {
            gpio_flag = false;

            printf("contador = %i\n\r", contador);
            mylcd_printCounter(contador);
            contador++;
        }
        vTaskDelay(pdMS_TO_TICKS(1));
        

       /*
        for (int offset = 0; offset < 8; offset++)
        {
            i2c_write(1 << offset);

            uint8_t data = 0;
            i2c_read(&data);

            printf("0b%01u%01u%01u%01u%01u%01u%01u%01u || p7p6p5p4p3p2p1p0 \n\r",
                   (data >> 7) & 1, (data >> 6) & 1, (data >> 5) & 1, (data >> 4) & 1,
                   (data >> 3) & 1, (data >> 2) & 1, (data >> 1) & 1, (data >> 0) & 1);

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        */
    }
}
