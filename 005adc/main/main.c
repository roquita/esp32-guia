/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

const static char *TAG = "EXAMPLE";

/*---------------------------------------------------------------
        ADC General Macros
---------------------------------------------------------------*/
/*
static int adc_raw[2][10];
static int voltage[2][10];
static bool example_adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle);
static void example_adc_calibration_deinit(adc_cali_handle_t handle);
*/
static bool example_adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle);
void app_main(void)
{
    //-------------ADC1 Init---------------//
    // gpio32
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config1 = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_11,
    };
    adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_4, &config1);

    //-------------ADC1 Calibration Init---------------//
    adc_cali_handle_t adc1_cali_handle = NULL;
    bool do_calibration1 = example_adc_calibration_init(ADC_UNIT_1, ADC_ATTEN_DB_11, &adc1_cali_handle);






    //-------------ADC2 Init---------------//
    // gpio4
    adc_oneshot_unit_handle_t adc2_handle;
    adc_oneshot_unit_init_cfg_t init_config2 = {
        .unit_id = ADC_UNIT_2,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    adc_oneshot_new_unit(&init_config2, &adc2_handle);

    //-------------ADC2 Config---------------//
    adc_oneshot_chan_cfg_t config2 = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_11,
    };
    adc_oneshot_config_channel(adc2_handle, ADC_CHANNEL_0, &config2);

    //-------------ADC2 Calibration Init---------------//
    adc_cali_handle_t adc2_cali_handle = NULL;
    bool do_calibration2 = example_adc_calibration_init(ADC_UNIT_2, ADC_ATTEN_DB_11, &adc2_cali_handle);

    int reading1 = 0;
    int reading2 = 0;
    int volt1 = 0;
    int volt2 = 0;
    while (1)
    {
        for (int i = 0; i < 20; i++)
        {
            int raw_reading = 0;
            adc_oneshot_read(adc1_handle, ADC_CHANNEL_4, &raw_reading);
            reading1 += raw_reading;
        }
        reading1 = reading1 / 20;

        // printf( "ADC%d Channel[%d] Raw Data: %d\n\r", ADC_UNIT_1 + 1, ADC_CHANNEL_4, reading1);

        if (do_calibration1)
        {
            adc_cali_raw_to_voltage(adc1_cali_handle, reading1, &volt1);
            // printf("ADC%d Channel[%d] Cali Voltage: %d mV", ADC_UNIT_1 + 1, ADC_CHANNEL_4, voltage[0][0]);
        }
        for (int i = 0; i < 20; i++)
        {
            int raw_reading = 0;
            adc_oneshot_read(adc2_handle, ADC_CHANNEL_0, &reading2);
            reading2 += raw_reading;
        }
        reading2 = reading2 / 20;

        if (do_calibration2)
        {
            adc_cali_raw_to_voltage(adc2_cali_handle, reading2, &volt2);
            // printf("ADC%d Channel[%d] Cali Voltage: %d mV", ADC_UNIT_1 + 1, ADC_CHANNEL_0, voltage[0][1]);
        }

        printf("%d,%d\r", reading1,reading2);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    /*
        //Tear Down
        ESP_ERROR_CHECK(adc_oneshot_del_unit(adc1_handle));
        if (do_calibration1) {
            example_adc_calibration_deinit(adc1_cali_handle);
        }


        ESP_ERROR_CHECK(adc_oneshot_del_unit(adc2_handle));
        if (do_calibration2) {
            example_adc_calibration_deinit(adc2_cali_handle);
        }
    */
}

/*---------------------------------------------------------------
        ADC Calibration
---------------------------------------------------------------*/

static bool example_adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

    if (!calibrated)
    {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK)
        {
            calibrated = true;
        }
    }

    *out_handle = handle;
    if (ret == ESP_OK)
    {
        ESP_LOGI(TAG, "Calibration Success");
    }
    else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated)
    {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    }
    else
    {
        ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated;
}

static void example_adc_calibration_deinit(adc_cali_handle_t handle)
{

    ESP_LOGI(TAG, "deregister %s calibration scheme", "Line Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(handle));
}
