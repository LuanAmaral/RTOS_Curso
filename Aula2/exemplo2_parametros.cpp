#include <stdio.h>

#define CONFIG_FREERTOS_UNICORE 1

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  

#ifdef __cplusplus
extern "C"
{
    void app_main();
}
#endif

void TASK(void* parameter)
{
    char* taskParameters = (char*)parameter;
    for(;;)
    {
        printf("%s", taskParameters);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void TASK1(void* args)
{
    while (1)
    {   
        printf("TASK 1 PRINTING!\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void TASK2(void* args)
{
    while (1)
    {   
        printf("TASK 2 PRINTING!\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    xTaskCreate(TASK,      
                "TASK1",
                2048,
                (void*)"task 1 print\n",
                1,
                NULL);   

    xTaskCreate(TASK,      
                "TASK2",
                2048,
                (void*)"task 2 print\n",
                1,
                NULL);

    return;
}
