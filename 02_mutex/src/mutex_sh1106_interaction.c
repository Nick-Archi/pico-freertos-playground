/*
* Objective:
* Create a task that will print to the SH1106 OLED via the sbm library
* by utilizing a mutex. 
*
* Takeaways:
* [] Declaring, Initializing, obtaining, and releasing mutexes
* [] Typedef a data structure used for task creation and execution
* [] Creating a generic task and casting the parameter into a data type
* [] Calling "3rd party" library in the context of FreeRTOS
*/

// C libraries
#include <stdio.h>
#include <stdlib.h>

// FreeRTOS libraries
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

// Pico SDK Libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"

// sbm includes
#include "SH1106_Interactions.h"

// task structure to keep track of OLED configuration
typedef struct oled_config_t
{
    int task;
    int delay;
    int page;
    int val; 
}oled_config;

static SemaphoreHandle_t mutex; // mutex for queue access

// instantiate objects for each task
static oled_config task1;
static oled_config task2;

/* 
* generic task (producer) to send info to consumer task that will 
* access the oled
*/
void vTask(void* pvParameters)
{
    oled_config config = *(oled_config*)pvParameters;
    char buffer1[7]; // null char included..
    char buffer2[5];

    while(true)
    {
        int size1 = sprintf(buffer1, "Task %d", config.task);
        // use sprintf to update a buffer of 0000 with the number
        int size2 = sprintf(buffer2, "%04d", config.val);

        // attempt to obtain mutex
        if(xSemaphoreTake(mutex, pdMS_TO_TICKS(portMAX_DELAY)))
        {
            write_string(buffer1, config.page, 0, size1 * 8);
            write_string(buffer2, config.page+1, 0, size2 * 8);
//printf("Task %d obtained mutex...attempting to write to queue\n", config.task);
//printf("Buffer = %s\n", buffer);
            // write to OLED
            update_sh1106();

            xSemaphoreGive(mutex);
            config.val++;
        }
        vTaskDelay(pdMS_TO_TICKS(config.delay));
    }
}

void setup_oled(void)
{
    init_SH1106(
        POCI,
        RST,
        CS,
        PICO,
        CLK
    );

    begin_sh1106();
    set_buffer();
    clear_buffer();
}

int main()
{
    stdio_init_all();
    setup_oled();

    // setup
    mutex = xSemaphoreCreateMutex();
    task1.task = 1;
    task1.delay = 3;
    task1.page = 3;
    task1.val = 999;

    task2.task = 2;
    task2.delay = 5000;
    task2.page = 3;
    task2.val = 1; 

    xTaskCreate(vTask, "Producer 1", 256, &task1, 1, NULL); 
    xTaskCreate(vTask, "Producer 2", 256, &task2, 2, NULL); 
    vTaskStartScheduler(); 

    while(1){};

    return 0;
}
