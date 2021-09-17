#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  
#include "esp_task_wdt.h"
#include "freertos/queue.h"

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

QueueHandle_t queue;

void recieve(void *arg)
{
  uint32_t counting;
  TickType_t ticks_delay = pdMS_TO_TICKS(5000);

  for(;;)
  {
      while(xQueueReceive(queue, &counting, 0) == pdPASS)
      {
        ESP_LOGI("Recieve", "send task counting -> %d", counting);
      }

      vTaskDelay(ticks_delay);
  }
}

void send(void *arg)
{
  uint32_t counting = 0;
  TickType_t tick_delay = pdMS_TO_TICKS(1000);
  
  for(;;)
  {
    counting++;
    xQueueSendToBack(queue, &counting, portMAX_DELAY);
    vTaskDelay(tick_delay);
  }
}

void app_main(void)
{
    queue = xQueueCreate(5, sizeof(int32_t));

    xTaskCreate(send,      
                "send",
                2048,
                NULL,
                2,
                NULL);   

    xTaskCreate(recieve,      
                "recieve",
                2048,
                NULL,
                2,
                NULL);
    return;
}

#ifdef __cplusplus
}
#endif