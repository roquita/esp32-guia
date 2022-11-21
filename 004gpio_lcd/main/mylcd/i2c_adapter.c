#include "driver/i2c.h"
#include "i2c_adapter.h"

#define DEV_ADDR 0x27
#define TICKS_TO_WAIT 100
int i2c_master_port = I2C_NUM_0;

esp_err_t i2c_read(uint8_t *data)
{
    esp_err_t err = ESP_OK;

    i2c_cmd_handle_t handle = i2c_cmd_link_create();

    err = i2c_master_start(handle);
    if (err != ESP_OK)
    {
        goto end;
    }

    err = i2c_master_write_byte(handle, DEV_ADDR << 1 | I2C_MASTER_READ, true);
    if (err != ESP_OK)
    {
        goto end;
    }

    err = i2c_master_read_byte(handle, data, I2C_MASTER_NACK);
    if (err != ESP_OK)
    {
        goto end;
    }

    i2c_master_stop(handle);
    err = i2c_master_cmd_begin(i2c_master_port, handle, TICKS_TO_WAIT);

end:
    i2c_cmd_link_delete(handle);
    return err;
}

esp_err_t i2c_write(uint8_t data)
{
    esp_err_t err = ESP_OK;

    i2c_cmd_handle_t handle = i2c_cmd_link_create();

    err = i2c_master_start(handle);
    if (err != ESP_OK)
    {
        goto end;
    }

    err = i2c_master_write_byte(handle, DEV_ADDR << 1 | I2C_MASTER_WRITE, true);
    if (err != ESP_OK)
    {
        goto end;
    }

    err = i2c_master_write_byte(handle, data, true);
    if (err != ESP_OK)
    {
        goto end;
    }

    i2c_master_stop(handle);

    err = i2c_master_cmd_begin(i2c_master_port, handle, TICKS_TO_WAIT);

end:
    i2c_cmd_link_delete(handle);
    return err;
}

void i2c_adapter_init()
{

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 27,
        .scl_io_num = 26,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000, // 100khz
    };

    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);

    i2c_write(0);
}

void i2c_adapter_set_pin(i2c_adapter_pins_t pin)
{
    uint8_t pins;
    i2c_read(&pins);

    pins |= (1 << pin);
    i2c_write(pins);
}
void i2c_adapter_clear_pin(i2c_adapter_pins_t pin)
{
    uint8_t pins;
    i2c_read(&pins);

    pins &= ~(1 << pin);
    i2c_write(pins);
}
