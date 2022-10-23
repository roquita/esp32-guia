#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mybutton.h"
#include "mytimer.h"
#include "mylcd.h"

bool gpio_flag = false;
int contador = 0;

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
    }
}

