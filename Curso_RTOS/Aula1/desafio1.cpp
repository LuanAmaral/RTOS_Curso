#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  

#include "driver/gpio.h"

#define LED_PIN  (gpio_num_t) 2u

#ifdef __cplusplus
extern "C"
{
    void app_main();
}
#endif


void blink(void* args)
{
    while (1)
    {   
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void blink2(void* args)
{
    while (1)
    {   
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(323));
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(323));
    }
}

void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    xTaskCreate(blink,
                "blink function",
                2048,
                NULL,
                0,
                NULL);   

    xTaskCreate(blink2,
                "blink function",
                2048,
                NULL,
                0,  
                NULL);  

    
    return ;    
}
