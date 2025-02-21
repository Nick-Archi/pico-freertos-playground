/*
* Objective: 
* [] Showcase the scheduler using a round-robin approach to select tasks for execution
* [] These tasks should blink LEDs connected to the GPIO pins of the RP2350
* [] Showcase how a generic function could be spun up as different tasks
*/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "pico/stdlib.h"

#define LED_ON (1)
#define LED_OFF (0)

typedef struct task_data_t
{
    int delay;
    int task_no;
    int gpio_pin;
}task_data;

static task_data task1 = 
{
    .delay = 1000,
    .task_no = 1,
    .gpio_pin = 21 
};

static task_data task2 = 
{
    .delay = 10000,
    .task_no = 2,
    .gpio_pin = 22 
};

void vTaskImpl(void* pvPara)
{
    task_data* task = (task_data*)pvPara;
    gpio_init(task->gpio_pin);
    gpio_set_dir(task->gpio_pin, GPIO_OUT);

    while(true)
    {
        gpio_put(task->gpio_pin, LED_ON);
        for(unsigned long i = 0; i < 1000000; i++);
        gpio_put(task->gpio_pin, LED_OFF);
        vTaskDelay(pdMS_TO_TICKS(task->delay));
    } 
}

int main()
{
    stdio_init_all();

    // create task with (function addr, string, stack size, , priority lvl, )
    xTaskCreate(vTaskImpl, "Task 1", 256, &task1, 1, NULL);
    xTaskCreate(vTaskImpl, "Task 2", 256, &task2, 1, NULL);

    vTaskStartScheduler();

    while(1){};
}
