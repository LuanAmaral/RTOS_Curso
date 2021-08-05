#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  
#include "esp_task_wdt.h"

#include "Serial.h"

#define CONFIG_FREERTOS_UNICORE 1
// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


#ifdef __cplusplus
extern "C"
{
#endif

Serial uart;
#define UART_BAUD_RATE 300
#define UART_TX_PIN 1U
#define UART_RX_PIN 3U

void TASK(void* parameter)
{
    char* str =(char*)parameter;
    for(;;)
    {
        for(int i=0; str[i]!='\0'; i++)
        {
            uart.write(&str[i], 1);
        }   
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
        uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart.init(UART_NUM_0, uart_config, UART_TX_PIN, UART_RX_PIN);
    vTaskDelay(pdMS_TO_TICKS(1000));

    xTaskCreate(TASK,      
                "TASK2",
                2048,
                (void*)"*",
                2,
                NULL);   

    xTaskCreate(TASK,      
                "TASK1",
                2048,
                (void*)"Curso de RTOS\nAula 2 -> Questao desafio:\nFazer um programa que uma string é interrompida por outra string\n",
                1,
                NULL);

    // while (1)
    // {
    //     esp_timer_get_time();
    // }
    
    return;
}

#ifdef __cplusplus
}
#endif