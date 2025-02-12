#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "pico/stdlib.h"

#define DELAY (1500000000)

void task1(void* pvPara)
{
    while(true)
    {
        printf("Task 1 is currently running\n");
        
        // introduce small delay
        //for(int i = 0; i < DELAY; i++);
        vTaskDelay(100); 
    }
}

void task2(void* pvPara)
{
    while(true)
    {
        printf("Task 2 is currently running\n");

        // introduce small delay
        //for(int i = 0; i < DELAY/2; i++);

        // place task into block state 
        vTaskDelay(100); 
    }
}

void task3(void* pvPara)
{
    while(true)
    {
        printf("Task 3 is currently running\n");

        // introduce small delay
        //for(int i = 0; i < DELAY/2; i++); 
 
        // place task into block state 
        vTaskDelay(101); 
    }
}

int main()
{
    stdio_init_all();

    // create task with (function addr, string, stack size, , priority lvl, )
    xTaskCreate(task1, "Task 1", 256, NULL, 2, NULL);
    xTaskCreate(task2, "Task 2", 256, NULL, 1, NULL);
    xTaskCreate(task3, "Task 3", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1){};
}
