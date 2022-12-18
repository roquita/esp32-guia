#include "max7219.h"

#define MAX7219_TAG "MAX7219"

spi_device_handle_t spi;

/*
uint8_t bufer_3[8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}}
*/

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
    a=10  b11 c12 d13 e14 f15
*/

uint8_t bufer_0[8] = {0x3C, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x3C};
uint8_t bufer_1[8] = {0x3C, 0x14, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
uint8_t bufer_2[8] = {0x3c, 0x04, 0x04, 0x08, 0x30, 0x20, 0x20, 0x3C};
uint8_t bufer_3[8] = {0x3C, 0x04, 0x04, 0x1C, 0x1C, 0x04, 0x04, 0x3C};
uint8_t bufer_4[8] = {0x24, 0x24, 0x24, 0x3C, 0x04, 0x04, 0x04, 0x04};
uint8_t bufer_5[8] = {0x3C, 0x20, 0x20, 0x3C, 0x04, 0x04, 0x04, 0x3C};
uint8_t bufer_6[8] = {0x3C, 0x20, 0x20, 0x3C, 0x24, 0x24, 0x24, 0x3C};
uint8_t bufer_7[8] = {0x3C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
uint8_t bufer_8[8] = {0x3C, 0x24, 0x24, 0X3C, 0x24, 0x24, 0x24, 0x3C};
uint8_t bufer_9[8] = {0x3C, 0x24, 0x24, 0X3C, 0x04, 0x04, 0x04, 0x3C};

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
void max7219_init()
{

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

    esp_err_t err;

    err = max7219_send_data(spi, MAX7219_REG_DISPLAY_TEST, 0x00);
    ESP_LOGI(MAX7219_TAG, "line %d ||err = %s", __LINE__, esp_err_to_name(err));
    /*
        err = max7219_send_data(spi, MAX7219_REG_SHUTDOWN, 0x00);
        ESP_LOGI(MAX7219_TAG, "line %d ||err = %s", __LINE__, esp_err_to_name(err));

        err = max7219_send_data(spi, MAX7219_REG_SHUTDOWN, 0x01);
        ESP_LOGI(MAX7219_TAG, "line %d ||err = %s", __LINE__, esp_err_to_name(err));
    */
    err = max7219_send_data(spi, MAX7219_REG_INTENSITY, 0x07);
    ESP_LOGI(MAX7219_TAG, "line %d ||err = %s", __LINE__, esp_err_to_name(err));

    err = max7219_send_data(spi, MAX7219_REG_DECODE_MODE, 0x00);
    ESP_LOGI(MAX7219_TAG, "line %d ||err = %s", __LINE__, esp_err_to_name(err));

    err = max7219_send_data(spi, MAX7219_REG_SCAN, 0x07);
    ESP_LOGI(MAX7219_TAG, "line %d ||err = %s", __LINE__, esp_err_to_name(err));

    max7219_send_data(spi, MAX7219_REG_DIGIT_0, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_1, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_2, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_3, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_4, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_5, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_6, 0);
    max7219_send_data(spi, MAX7219_REG_DIGIT_7, 0);

    /*
    max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_3[0]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_3[1]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_3[2]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_3[3]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_3[4]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_3[5]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_3[6]);
    max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_3[7]);
    */
}
void max7219_print_number(int number)
{
    switch (number)
    {
    case 0:
        max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_0[0]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_0[1]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_0[2]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_0[3]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_0[4]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_0[5]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_0[6]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_0[7]);
        break;
    case 1:
        max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_1[0]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_1[1]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_1[2]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_1[3]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_1[4]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_1[5]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_1[6]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_1[7]);
        break;
    case 2:
        max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_2[0]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_2[1]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_2[2]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_2[3]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_2[4]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_2[5]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_2[6]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_2[7]);
        break;
    case 3:
        max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_3[0]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_3[1]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_3[2]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_3[3]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_3[4]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_3[5]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_3[6]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_3[7]);
        break;
    case 4:
        max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_4[0]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_4[1]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_4[2]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_4[3]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_4[4]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_4[5]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_4[6]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_4[7]);
        break;
    case 5:
        max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_5[0]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_5[1]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_5[2]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_5[3]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_5[4]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_5[5]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_5[6]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_5[7]);
        break;
    case 6:
        max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_6[0]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_6[1]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_6[2]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_6[3]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_6[4]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_6[5]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_6[6]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_6[7]);
        break;
    case 7:
        max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_7[0]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_7[1]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_7[2]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_7[3]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_7[4]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_7[5]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_7[6]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_7[7]);
        break;
    case 8:
        max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_8[0]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_8[1]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_8[2]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_8[3]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_8[4]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_8[5]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_8[6]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_8[7]);
        break;
    case 9:
        max7219_send_data(spi, MAX7219_REG_DIGIT_0, bufer_9[0]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_1, bufer_9[1]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_2, bufer_9[2]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_3, bufer_9[3]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_4, bufer_9[4]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_5, bufer_9[5]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_6, bufer_9[6]);
        max7219_send_data(spi, MAX7219_REG_DIGIT_7, bufer_9[7]);
        break;

    default:

        break;
    }
}
