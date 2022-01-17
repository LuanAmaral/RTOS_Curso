#include <stdio.h>
#include "Serial.h"

Serial::Serial()
{
}

void Serial::init(uart_port_t _uart_num, uart_config_t _uart_config, uint8_t tx_pin, uint8_t rx_pin)
{
    uart_num = _uart_num;
    uart_config = _uart_config;
    QueueHandle_t uart_queue;
   
    ESP_ERROR_CHECK(uart_driver_install(uart_num, BUFFER_SIZE, 
                                        0, 10, &uart_queue, 0));
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config)); 
    ESP_ERROR_CHECK(uart_set_pin(uart_num, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
}

esp_err_t Serial::write(char* tx_data, size_t tx_data_length)
{
    if(uart_write_bytes(uart_num, tx_data, tx_data_length) == tx_data_length)
        return ESP_OK;

    return ESP_FAIL;
}

esp_err_t Serial::read(char* rx_data)
{
    size_t rx_length;
    ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, &rx_length));
    if (rx_length == uart_read_bytes(uart_num, rx_data, rx_length, 100))
    {
        uart_flush(uart_num);
        return ESP_OK;
    }
    uart_flush(uart_num);   
    return ESP_FAIL;

}

Serial::~Serial()
{
    //TODO
}