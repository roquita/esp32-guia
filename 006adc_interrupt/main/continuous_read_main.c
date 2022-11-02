/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_adc/adc_continuous.h"
static const char *TAG = "EXAMPLE";
bool adc_conv_done_flag = false;


static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    adc_conv_done_flag = true;
    
    return true;
}


void app_main(void)
{
    adc_continuous_handle_t handle = NULL;

    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 8*1, 
        .conv_frame_size = 4*2,
    };
    adc_continuous_new_handle(&adc_config, &handle);

    adc_continuous_config_t dig_cfg = {
        .pattern_num = 2,            // 2 channels to scan
        .sample_freq_hz = 20 * 1000, // 20khz,  <20khz-2Mhz>
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE1,
    };
    

    adc_digi_pattern_config_t adc_pattern[2] = {0};

    // gpio 32
    adc_pattern[0].atten = ADC_ATTEN_DB_11;
    adc_pattern[0].channel = ADC_CHANNEL_4;
    adc_pattern[0].unit = ADC_UNIT_1;
    adc_pattern[0].bit_width = ADC_BITWIDTH_12;

    // gpio 33
    adc_pattern[1].atten = ADC_ATTEN_DB_11;
    adc_pattern[1].channel = ADC_CHANNEL_5;
    adc_pattern[1].unit = ADC_UNIT_1;
    adc_pattern[1].bit_width = ADC_BITWIDTH_12;

    dig_cfg.adc_pattern = adc_pattern;

    adc_continuous_config(handle, &dig_cfg);

    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = s_conv_done_cb,
    };

    adc_continuous_register_event_callbacks(handle, &cbs, NULL);


    adc_continuous_start(handle);

    while (1)
    {
        if (adc_conv_done_flag)
        {
            adc_conv_done_flag = false;

            esp_err_t ret;
            uint32_t ret_num = 0;
            uint8_t result[8*1] = {0};
            memset(result, 0, 8*1);

            ret = adc_continuous_read(handle, result, 8*1, &ret_num, 0);
            if (ret == ESP_OK)
            {

                adc_digi_output_data_t *p0 = (void *)&result[0];
                adc_digi_output_data_t *p1 = (void *)&result[2];

                ESP_LOGI(TAG, "ret_num :%lu || gpio32 ADC1 , Channel: %u, Value: %u || gpio33 ADC1 , Channel: %u, Value: %u",
                         ret_num,
                         p0->type1.channel, p0->type1.data,
                         p1->type1.channel, p1->type1.data);
            }

            //adc_continuous_stop(handle);
        }
        vTaskDelay(10);
    }

    adc_continuous_stop(handle);
    adc_continuous_deinit(handle);
}
