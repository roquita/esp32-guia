#pragma once 

typedef enum{
    P0=0,
    P1,
    P2,
    P3,
    P4,
    P5,
    P6,
    P7
}i2c_adapter_pins_t;

void i2c_adapter_init();
void i2c_adapter_set_pin(i2c_adapter_pins_t pin);
void i2c_adapter_clear_pin(i2c_adapter_pins_t pin);