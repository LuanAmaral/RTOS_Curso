#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  
#include "esp_task_wdt.h"

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


#ifdef __cplusplus
extern "C"
{
    void app_main();
}
#endif

void TASK1(void* parameter)
{
    char* taskParameters = (char*)parameter;
    for(;;)
    {
        printf("%s", taskParameters);
        for(uint32_t i = 0; i < 2000000; i++);
    }
}

void TASK2(void* parameter)
{
    char* taskParameters = (char*)parameter;
    for(;;)
    {
        printf("%s", taskParameters);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    xTaskCreate(TASK1,      
                "TASK1",
                2048,
                (void*)"task 1 print\n",
                1,
                NULL);   

    xTaskCreate(TASK2,      
                "TASK2",
                2048,
                (void*)"task 2 print\n",
                2,
                NULL);

    while (1)
    {
        esp_timer_get_time();
    }
    
    return;
}
