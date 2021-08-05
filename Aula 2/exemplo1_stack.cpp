#include <stdio.h>

#define CONFIG_FREERTOS_UNICORE 1

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  
#include "Serial.h"

#define SERIAL_UART
#define STACK_SIZE 512


#ifdef SERIAL_UART
#define UART_BAUD_RATE 115200
#define UART_TX_PIN 1U
#define UART_RX_PIN 3U
Serial uart;
#define print(X) { char str[] = X; uart.write(str, sizeof(str)); }

#else
#define print(X) {printf(X);}
#endif


#ifdef __cplusplus
extern "C"
{
    void app_main();
}
#endif


void helloWorld(void* args)
{
    while (1)
    {   
        print("hello world!\n")
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    #ifdef SERIAL_UART
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart.init(UART_NUM_0, uart_config, UART_TX_PIN, UART_RX_PIN);
    #endif

    xTaskCreate(helloWorld,      
                "hello world task",
                STACK_SIZE,
                NULL,
                0,
                NULL);   

    return;
}
