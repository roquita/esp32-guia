#pragma once

#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

esp_err_t max7219_send_data(spi_device_handle_t spi, uint8_t addr, uint8_t data);
void max7219_init();
void max7219_print_number(int number);
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