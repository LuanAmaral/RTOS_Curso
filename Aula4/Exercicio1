#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  
#include "esp_task_wdt.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

#include "esp_log.h"


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

#define BOTAO GPIO_NUM_0
#define LED   GPIO_NUM_2

SemaphoreHandle_t semaphore_handle;

void interrupt(void *arg)
{
    xSemaphoreGiveFromISR(semaphore_handle, NULL);
    gpio_set_level(LED, 1);

}

void TASK(void* parameter)
{
  for(;;)
  {
      xSemaphoreTake(semaphore_handle, portMAX_DELAY);
      gpio_set_level(LED, 0);
      ESP_LOGW("Task","Sincronizado com Interrupção");
  }
}

void app_main(void)
{
    gpio_config_t pin_config;
    pin_config.mode = GPIO_MODE_INPUT;
    pin_config.pin_bit_mask = 1 << BOTAO;
    pin_config.pull_up_en = GPIO_PULLUP_ENABLE;
    pin_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pin_config.intr_type = GPIO_INTR_LOW_LEVEL;
    
    gpio_config(&pin_config);

    pin_config.mode = GPIO_MODE_OUTPUT;
    pin_config.pin_bit_mask = 1 << LED;
    pin_config.pull_up_en = GPIO_PULLUP_DISABLE;
    pin_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pin_config.intr_type = GPIO_INTR_DISABLE;
    
    gpio_config(&pin_config);


    gpio_install_isr_service(0);
    gpio_isr_handler_add(BOTAO, interrupt, NULL);

    semaphore_handle = xSemaphoreCreateBinary();


    xTaskCreate(TASK,      
                "Task1",
                2048,
                NULL,
                2,
                NULL);   

    xTaskCreate(printString,      
                "printString",
                2048,
                NULL,
                1,
                NULL);   
    return;
}

#ifdef __cplusplus
}
#endif