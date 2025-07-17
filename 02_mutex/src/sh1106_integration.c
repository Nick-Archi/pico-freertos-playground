/*
* Objective:
* Create a task that will print to the SH1106 OLED via the sbm library 
*
* Takeaways:
* [] Declaring, Initializing, obtaining, and releasing mutexes
* [] Typedef a data structure used for task creation and execution
* [] Creating a generic task and casting the parameter into a data type
* [] Utilizing a queue to achieve IPC
* [] Calling "3rd party" library in the context of FreeRTOS
*/

// C libraries
#include <stdio.h>
#include <stdlib.h>

// FreeRTOS libraries
#include "FreeRTOS.h"
#include "queue.h"
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
    int page;
    uint8_t val; 
}oled_config;

static SemaphoreHandle_t mutex; // mutex for queue access
static QueueHandle_t q_handle; // handle for IPC queue

// instantiate objects for each task
static oled_config task1;
static oled_config task2;

// generic task (producer) to send info to consumer task that will 
// access the oled
void vProducer(void* pvParameters)
{
    oled_config config = *(oled_config*)pvParameters;
    int start = config.val;

    while(true)
    {
        // attempt to obtain mutex
        if(xSemaphoreTake(mutex, pdMS_TO_TICKS(0) == pdPASS))
        {
printf("Task obtained mutex...attempting to write to queue\n");
            xQueueSend(q_handle, (const void*) &config, pdMS_TO_TICKS(0));
            xSemaphoreGive(mutex);
            config.val++;
            vTaskDelay(100);
        }
    }
}

void vConsumer(void* pvParameters)
{
    oled_config config;
    char buffer[] = {1,1,1,1,0};
    BaseType_t status;
    while(true)
    {
        status = xQueueReceive(q_handle, &config, pdMS_TO_TICKS(100));
       
        if(status == pdPASS)
        {
printf("Receiver took from queue\n"); 
            // received config from queue now update oled
            itoa(config.val, buffer, 10);
            write_string(buffer, config.page, 0, sizeof(buffer) * 8);
            update_sh1106();
        }
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
    q_handle = xQueueCreate(3, sizeof(oled_config));
    task1.page = 3;
    task1.val = 0;
    task2.page = 4;
    task2.val = 1; 

    if(q_handle != NULL)
    {
        xTaskCreate(vProducer, "Producer 1", 256, &task1, 1, NULL); 
        xTaskCreate(vProducer, "Producer 2", 256, &task2, 1, NULL); 
        xTaskCreate(vConsumer, "Consumer", 256, NULL, 2, NULL);
        vTaskStartScheduler(); 
    }

    while(1){};

    return 0;
}
