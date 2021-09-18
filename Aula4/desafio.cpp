#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  
#include "esp_task_wdt.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
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

#define EVENT_2SEG            1UL<<0UL
#define EVENT_5SEG            1UL<<1UL
#define EVENT_7SEG            1UL<<2UL
#define EVENT_BUTTON_RELEASE  1UL<<3UL

EventGroupHandle_t event_group;
SemaphoreHandle_t semaphr;

void interrupt(void *arg)
{
    BaseType_t isHighTask;
    xSemaphoreGiveFromISR(semaphr, &isHighTask);
    gpio_set_level(LED, !gpio_get_level(LED));
    portYIELD_FROM_ISR(isHighTask);
}

void readPressButtonTime(void* parameter)
{
  TickType_t ticks = pdMS_TO_TICKS(10);
  uint64_t pressed_time;
  for(;;)
  {
      xSemaphoreTake(semaphr, portMAX_DELAY);
      pressed_time = esp_timer_get_time();
      while(gpio_get_level(BOTAO) == 0)
      {
        if(esp_timer_get_time() - pressed_time > 2000000)
        {
            xEventGroupSetBits(event_group, EVENT_2SEG);
        }
        if(esp_timer_get_time() - pressed_time > 5000000)
        {
            xEventGroupSetBits(event_group, EVENT_5SEG);
        }
        if(esp_timer_get_time() - pressed_time > 7000000)
        {
            xEventGroupSetBits(event_group, EVENT_7SEG);
        }
        vTaskDelay(ticks);
      }
      xEventGroupSetBits(event_group, EVENT_BUTTON_RELEASE);
      ESP_LOGI("TIME", "BOtão pressionado por %u ms", (uint32_t) ((esp_timer_get_time()-pressed_time)/1000) );
  }
}

void readEvent(void *arg)
{
  const EventBits_t bits_to_wait_for = (EVENT_2SEG | EVENT_5SEG | EVENT_7SEG);
  EventBits_t bits;
  for(;;)
  {
    xEventGroupWaitBits(event_group, EVENT_BUTTON_RELEASE, pdTRUE, pdTRUE,portMAX_DELAY);
    bits = xEventGroupWaitBits(event_group, bits_to_wait_for, pdTRUE, pdFALSE, 0);

    if ( (bits & EVENT_2SEG) != 0)
    {
      ESP_LOGI("READ BITS", "Botão pressionado por 2 seg");
    }
    if ( (bits & EVENT_5SEG) != 0)
    {
      ESP_LOGI("READ BITS", "Botão pressionado por 5 seg");
    }
    if ( (bits & EVENT_7SEG) != 0)
    {
      ESP_LOGI("READ BITS", "Botão pressionado por 7 seg");
    }
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
    semaphr = xSemaphoreCreateBinary();

    ESP_LOGI("MAIN", "SET Task");
    xTaskCreate(readPressButtonTime,      
                "readPressButtonTime",
                2048,
                NULL,
                2,
                NULL);   

    xTaskCreate(readEvent,      
                "readEvent",
                2048,
                NULL,
                2,
                NULL);   

    return;
    
}

#ifdef __cplusplus
}
#endif