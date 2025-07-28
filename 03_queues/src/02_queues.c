/*
* Objective:
* Create consumer tasks that will dump data into a queue that 
* is consumed by the producer task to print to the SH1106 OLED 
*
* Takeaways:
* [] Interaction with a queue 
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
    int task;
    int page;
    uint8_t val;
    uint16_t ticks; 
}oled_config;

static QueueHandle_t queue; // handle for IPC queue

// instantiate objects for each task
static oled_config task1;
static oled_config task2;
static oled_config task3;

static void setupOledConfig(oled_config* dst, oled_config src);

// generic task (producer) to send info to consumer task that will 
// access the oled
void vProducer(void* pvParameters)
{
    oled_config config = *(oled_config*)pvParameters;
    int start = config.val;

    while(true)
    {
        xQueueSend(queue, (const void*) &config, pdMS_TO_TICKS(portMAX_DELAY));
        vTaskDelay(pdMS_TO_TICKS(config.ticks));
        config.val++;
    }
}

void vConsumer(void* pvParameters)
{
    oled_config config;
    char buffer1[7];
    char buffer2[5];
    BaseType_t status;
    while(true)
    {
        status = xQueueReceive(queue, &config, pdMS_TO_TICKS(portMAX_DELAY));
        int size1 = sprintf(buffer1, "Task %d", config.task);
        // use sprintf to update a buffer of 0000 with the number
        int size2 = sprintf(buffer2, "%04d", config.val);
        if(status == pdPASS)
        {
printf("Receiver took from queue\n"); 
            // received config from queue now update oled
            write_string(buffer1, config.page, 0, size1 * 8);
            write_string(buffer2, config.page+1, 0, size2 * 8);
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
    queue = xQueueCreate(3, sizeof(oled_config));

    // using compand literals
    setupOledConfig(&task1, (oled_config){.task = 1, .page = 3, .val = 0, .ticks=1000});
    setupOledConfig(&task2, (oled_config){.task = 2, .page = 5, .val = 0, .ticks=500});
    setupOledConfig(&task3, (oled_config){.task = 3, .page = 7, .val = 0, .ticks=10000});

    if(queue != NULL)
    {
        xTaskCreate(vProducer, "Producer 1", 256, &task1, 1, NULL); 
        xTaskCreate(vProducer, "Producer 2", 256, &task2, 1, NULL); 
        xTaskCreate(vProducer, "Producer 3", 256, &task3, 2, NULL); 
        xTaskCreate(vConsumer, "Consumer", 256, NULL, 3, NULL);
        vTaskStartScheduler(); 
    }

    while(1){};

    return 0;
}

static void setupOledConfig(oled_config* dst, oled_config src)
{
    dst->task = src.task;
    dst->page = src.page;
    dst->val = src.val;
    dst->ticks = src.ticks;
}
