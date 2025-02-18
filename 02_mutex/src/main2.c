/*
* Objective:
* In this source file, I am attempting to blink 2 LEDs connected to the RP2350
* GPIO ports via 2 tasks. I am trying to implement a mutex in which will be held
* for some time before being released.

* Takeaways:
* [] Declaring, Initializing, obtaining, and releasing mutexes
*/

#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "pico/stdlib.h"

#define LED_OFF 0
#define LED_ON 1
#define LED_1 22
#define LED_2 21

static SemaphoreHandle_t mutex;

typedef struct gpio_state_t
{
    int led;
    int task;
}gpio_task;

static gpio_task gpio_task1; 
static gpio_task gpio_task2; 

void vTaskFunction(void* pvParameters)
{
    gpio_task* ptr = (gpio_task*) pvParameters;
    gpio_init(ptr->led);
    gpio_set_dir(ptr->led, GPIO_OUT); 
    while(true)
    {
        if(xSemaphoreTake(mutex, 0) == pdTRUE)
        {
            // [TODO] Switch to using vPrintLine() one day?
            printf("Task %d is currently running\n", ptr->task);
            gpio_put(ptr->led, LED_ON);        
            vTaskDelay(100);

            gpio_put(ptr->led, LED_OFF);
            xSemaphoreGive(mutex);
        }
        vTaskDelay(100); 
    }
}

int main()
{
    stdio_init_all();

    mutex = xSemaphoreCreateMutex();
    gpio_task1.led = LED_1;
    gpio_task1.task = 1;
    gpio_task2.led = LED_2;
    gpio_task2.task = 2;

    xTaskCreate(vTaskFunction,
                "Task 1",
                256,
                &gpio_task1,
                2,
                NULL);

    xTaskCreate(vTaskFunction,
                "Task 2",
                256,
                &gpio_task2,
                2,
                NULL);

    vTaskStartScheduler();

    while(1){};
}
