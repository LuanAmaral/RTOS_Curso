#include <stdio.h>

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
    xTaskCreatePinnedToCore(TASK,      
                "TASK1",
                2048,
                (void*)"task 1 print\n",
                2,
                NULL,
                APP_CPU_NUM);   

    xTaskCreatePinnedToCore(TASK,      
                "TASK2",
                2048,
                (void*)"task 2 print\n",
                2,
                NULL,
                PRO_CPU_NUM);

    return;
}
