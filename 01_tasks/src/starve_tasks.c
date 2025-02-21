/*
* Objective:
* [] showcase task starvation
* 
* In this example, task 2 is going to monapolize the scheduler and starve
* out task 1. Task 1 will get to execute everynowandthen but since task 2
* is higher priority and a lower delay, when it's ready to execute, it's going 
* to take precedence.
*/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "pico/stdlib.h"

#define DELAY (15000000)

typedef struct task_data_t
{
    char task_number;
    int delay;
}task_data;

static task_data task_data1 = 
{
    .task_number = '1', 
    .delay = 10
};

static task_data task_data2 = 
{
    .task_number = '2', 
    .delay = 2
};

void vTaskImpl(void* pvPara)
{
    while(true)
    {
        printf("Task %c is currently running\n", ((task_data*)pvPara)->task_number);
        
        // introduce small delay
        //for(int i = 0; i < ((task_data*)pvPara)->delay; i++);
        vTaskDelay(pdMS_TO_TICKS(((task_data*)pvPara)->delay));
    }
}

int main()
{
    stdio_init_all();

    // create task with (function addr, string, stack size, , priority lvl, )
    xTaskCreate(vTaskImpl, "Task 1", 256, &task_data1, 1, NULL);
    xTaskCreate(vTaskImpl, "Task 2", 256, &task_data2, 2, NULL);

    vTaskStartScheduler();

    while(1){};
}
