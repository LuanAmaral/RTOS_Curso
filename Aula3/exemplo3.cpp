#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  
#include "freertos/semphr.h"
#include "esp_task_wdt.h"
#include "esp_log.h"

#ifdef __cplusplus
extern "C"
{
#endif

SemaphoreHandle_t semaphore_handle;

void sincrona(void *arg)
{
  TickType_t ticks = pdMS_TO_TICKS(500);
  for(;;)
  {
      if( xSemaphoreTake(semaphore_handle, 0) == pdPASS )
      {
        ESP_LOGW("Sincrona","Faça alguma tarefa");
      }
      else
      {
        ESP_LOGW("Sincrona","Esperando Sincronizar");
      }
      vTaskDelay(ticks);
  }
}

void assincrona(void *arg)
{
  TickType_t ticks = pdMS_TO_TICKS(2000);
  for(;;)
  {
    xSemaphoreGive(semaphore_handle);
    xSemaphoreGive(semaphore_handle);
    xSemaphoreGive(semaphore_handle);

    ESP_LOGI("Assincrona", "Enviando Semaforo.");
    vTaskDelay(ticks); 
  }
}

void app_main(void)
{
    semaphore_handle = xSemaphoreCreateCounting(10, 0);

    xTaskCreate(sincrona, "writeTask1", 2048, NULL, 2, NULL );
    xTaskCreate(assincrona, "writeTask2", 2048, NULL, 2, NULL );

    return;
}

#ifdef __cplusplus
}
#endif