
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"

#define MAX7219_HOST HSPI_HOST

#define PIN_NUM_MISO -1
#define PIN_NUM_MOSI 25
#define PIN_NUM_CLK 27
#define PIN_NUM_CS 26

#define MAX7219_REG_DIGIT_0 0x01
#define MAX7219_REG_DIGIT_1 0x02
#define MAX7219_REG_DIGIT_2 0x03
#define MAX7219_REG_DIGIT_3 0x04
#define MAX7219_REG_DIGIT_4 0x05
#define MAX7219_REG_DIGIT_5 0x06
#define MAX7219_REG_DIGIT_6 0x07
#define MAX7219_REG_DIGIT_7 0x08
#define MAX7219_REG_DECODE_MODE 0x09
#define MAX7219_REG_INTENSITY 0x0A
#define MAX7219_REG_SCAN 0x0B
#define MAX7219_REG_SHUTDOWN 0x0C
#define MAX7219_REG_DISPLAY_TEST 0x0F
/*
uint8_t bufer_3[8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}}
*/
uint8_t bufer_3[8] = {0x3C, 0x04, 0x04, 0x1C, 0x1C, 0x04, 0x04, 0x3C};

esp_err_t max7219_send_data(spi_device_handle_t spi, uint8_t addr, uint8_t data)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t)); // Zero out the transaction
    t.addr = addr;
    t.length = 8;        // Command is 8 bits
    t.tx_buffer = &data; // The data is the cmd itself

    return spi_device_polling_transmit(spi, &t); // Transmit!
}

// Initialize the display
void max7219_init(spi_device_handle_t spi)
{
    esp_err_t err;

    err = max7219_send_data(spi, MAX7219_REG_DISPLAY_TEST, 0x01);
    ESP_LOGI("main", "line %d ||err = %s", __LINE__, esp_err_to_name(err));

    vTaskDelay(pdMS_TO_TICKS(3000));

    err = max7219_send_data(spi, MAX7219_REG_DISPLAY_TEST, 0x00);
    ESP_LOGI("main", "line %d ||err = %s", __LINE__, esp_err_to_name(err));

    err = max7219_send_data(spi, MAX7219_REG_SHUTDOWN, 0x00);
    ESP_LOGI("main", "line %d ||err = %s", __LINE__, esp_err_to_name(err));

    err = max7219_send_data(spi, MAX7219_REG_SHUTDOWN, 0x01);
    ESP_LOGI("main", "line %d ||err = %s", __LINE__, esp_err_to_name(err));

    err = max7219_send_data(spi, MAX7219_REG_INTENSITY, 0x07);
    ESP_LOGI("main", "line %d ||err = %s", __LINE__, esp_err_to_name(err));

    err = max7219_send_data(spi, MAX7219_REG_DECODE_MODE, 0x00);
    ESP_LOGI("main", "line %d ||err = %s", __LINE__, esp_err_to_name(err));

    err = max7219_send_data(spi, MAX7219_REG_SCAN, 0x07);
    ESP_LOGI("main", "line %d ||err = %s", __LINE__, esp_err_to_name(err));

    max7219_send_data(spi, MAX7219_REG_DIGIT_0, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_1, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_2, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_3, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_4, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_5, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_6, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_7, 0);

    max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_3[0]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_3[1]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_3[2]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_3[3]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_3[4]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_3[5]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_3[6]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_3[7]);
}

void app_main(void)
{
    spi_device_handle_t spi;
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1};

    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 8,
        .dummy_bits = 0,
        .clock_speed_hz = 1 * 1000 * 1000, // Clock out at 1 MHz
        .mode = 0,                         // SPI mode 0
        .spics_io_num = PIN_NUM_CS,        // CS pin
        .queue_size = 7,                   // We want to be able to queue 7 transactions at a time
    };

    // Initialize the SPI bus
    spi_bus_initialize(MAX7219_HOST, &buscfg, SPI_DMA_DISABLED); // SPI_DMA_CH_AUTO

    // Attach max7219 to the SPI bus
    spi_bus_add_device(MAX7219_HOST, &devcfg, &spi);

    // Initialize the device
    max7219_init(spi);
}
