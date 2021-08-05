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
        for(uint32_t i = 0; i < 5000000; i++);
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
                2,
                NULL);

    return;
}
