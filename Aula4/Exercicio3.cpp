#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  
#include "esp_task_wdt.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_task_wdt.h"


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

#define BOTAO       GPIO_NUM_0
#define LED         GPIO_NUM_2

#define EVENT_ISR   1UL<<0UL
#define EVENT_TASK  1UL<<1UL

EventGroupHandle_t event_group;

void interrupt(void *arg)
{
    BaseType_t isHighTask;
    xEventGroupSetBitsFromISR(event_group,EVENT_ISR, &isHighTask);
    gpio_set_level(LED, !gpio_get_level(LED));
    portYIELD_FROM_ISR(isHighTask);
}

void setEvent(void* parameter)
{
  TickType_t ticks = pdMS_TO_TICKS(5000);
  for(;;)
  {
      xEventGroupSetBits(event_group, EVENT_TASK);
      ESP_LOGW("Task","Task setando Bit");
      vTaskDelay(ticks);
  }
}

void sincronizadaPorEventos(void *arg)
{
  const EventBits_t bits_to_wait_for = (EVENT_TASK | EVENT_ISR);
  for(;;)
  {
    xEventGroupWaitBits(event_group, bits_to_wait_for, pdTRUE, pdTRUE,portMAX_DELAY);
    ESP_LOGI("TASK", "Interrução e task ocorreram ");
    uint64_t start_time = esp_timer_get_time();
    while(esp_timer_get_time() - start_time <= 3000000);
    vTaskDelay(pdMS_TO_TICKS(10));
    
  }

}

void app_main(void)
{
    gpio_config_t pin_config;
    pin_config.mode = GPIO_MODE_INPUT;
    pin_config.pin_bit_mask = 1 << BOTAO;
    pin_config.pull_up_en = GPIO_PULLUP_ENABLE;
    pin_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pin_config.intr_type = GPIO_INTR_NEGEDGE;
    
    gpio_config(&pin_config);
   
    pin_config.mode = GPIO_MODE_OUTPUT;
    pin_config.pin_bit_mask = 1 << LED;
    pin_config.pull_up_en = GPIO_PULLUP_DISABLE;
    pin_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pin_config.intr_type = GPIO_INTR_DISABLE;
    
    gpio_config(&pin_config);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BOTAO, interrupt, NULL);
    gpio_set_level(LED, 0);

    event_group = xEventGroupCreate();

    ESP_LOGI("MAIN", "SET Task");
    xTaskCreate(setEvent,      
                "setEvent",
                2048,
                NULL,
                2,
                NULL);   

    xTaskCreate(sincronizadaPorEventos,      
                "sincronizadaPorEventos",
                2048,
                NULL,
                2,
                NULL);   

    return;
    
}

#ifdef __cplusplus
}
#endif