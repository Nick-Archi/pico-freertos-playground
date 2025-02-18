#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "semphr.h"

// create mutex object
static SemaphoreHandle_t mutex;

#define DELAY (1500000000)

void task1(void* pvPara)
{
    char ch = '1';
    while(true)
    {
        // attempt to obtain mutex
        if(xSemaphoreTake(mutex, 0) == pdTRUE)
        {
            printf("Task %c is currently running\n", ch);
            for(int i = 0; i < 10; i++)
                putchar(ch); 
            puts("");
            xSemaphoreGive(mutex);
        }
        // introduce small delay
        // place task into block state 
        vTaskDelay(100); 
    }
}

void task2(void* pvPara)
{
    char ch = '2';
    while(true)
    {
        // attempt to obtain mutex
        if(xSemaphoreTake(mutex, 0) == pdTRUE)
        {
            printf("Task %c is currently running\n", ch);
            for(int i = 0; i < 10; i++)
                putchar(ch); 
            puts("");
            xSemaphoreGive(mutex);
        }
        // introduce small delay
        // place task into block state 
        vTaskDelay(100); 
    }
}

void task3(void* pvPara)
{
    char ch = '3';
    while(true)
    {
        // attempt to obtain mutex
        if(xSemaphoreTake(mutex, 0) == pdTRUE)
        {
            printf("Task %c is currently running\n", ch);
            for(int i = 0; i < 10; i++)
                putchar(ch); 
            puts("");
            xSemaphoreGive(mutex);
        }
        // introduce small delay
        // place task into block state 
        vTaskDelay(100); 
    }
}

int main()
{
    stdio_init_all();

    // inititalize mutex object
    mutex = xSemaphoreCreateMutex();

    // create task with (function addr, string, stack size, , priority lvl, )
    xTaskCreate(task1, "Task 1", 256, NULL, 2, NULL);
    xTaskCreate(task2, "Task 2", 256, NULL, 1, NULL);
    xTaskCreate(task3, "Task 3", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1){};
}
