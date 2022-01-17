/*
    função para comunicação Serial ESP32
    Luan R Amaral
*/

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdio.h>
#include <stdint.h> 
#include "driver/uart.h"
#include "esp_attr.h"

#define BUFFER_SIZE 1000

class Serial
{
private:
    uart_config_t uart_config;
    uint8_t uart_num;
public:
    Serial();
    ~Serial();
    void init(uart_port_t _uart_num, uart_config_t _uart_config, uint8_t tx_pin, uint8_t rx_pin);
    esp_err_t write(char *tx_data, size_t tx_data_length);
    esp_err_t read(char *rx_data);
};
#endif