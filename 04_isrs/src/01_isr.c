/*
* Objective:
* Application that upon pressing a button will update the # of button
* presses and relay this to the SH1106 OLED to print. Button debouncer
* is used to capture the button presses correctly 
*
* Takeaways:
* [] configurating GPIO pins to see interrupt
* [] ISR callback from GPIO IRQ of Falling Edge
* [] using timer to unblock semaphore for task (de-bounce logic)
* [] task samples the gpio pin after some time (de-bounce logic)
*/

//#define DBG 0
#define DEBOUNCE_MS 10

// C libraries
#include <stdio.h>
#include <stdlib.h>

// FreeRTOS libraries
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

// Pico SDK Libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

// sbm includes
//#include "SH1106_Interactions.h"
#include "display.h"
#include "driver.h"
#include "sh1106_driver.h"
#include "spi_transport.h"
#include "transport.h"

SemaphoreHandle_t sema;
TimerHandle_t h_debounce;

// structure to keep track of button state
typedef struct button_state_t
{
    uint8_t gpio;
    uint8_t state;
}button_state;

// task structure to keep track of OLED configuration
typedef struct oled_config_t
{
    int task;
    int page;
    uint8_t val;
    uint16_t ticks; 
}oled_config;

// instantiate objects for each task
static oled_config task1;

// led object
static button_state button1;

// sh1106 display object
Display sh1106_display;
DisplayPages sh1106_pgs[SH1106_PAGES];
DisplayPageInfo sh1106_info[SH1106_PAGES];
uint8_t sh1106_buffer[(SH1106_WIDTH * SH1106_HEIGHT) / 8];

SpiTransportConfig sh1106_ctx = 
{
    .spi = SPI_PORT,
    .cs = OLED_CS,
    .clk = OLED_CLK,
    .dc = OLED_POCI,
    .pico = OLED_PICO,
    .rst = OLED_RST,
    .baud = BAUD
};

static void setupOledConfig(oled_config* dst, oled_config src);
static void setupButtonConfig(button_state* dst, button_state src);

void vOledTask(void* pvParameters);
void vButtonHandlerISR();
void vSampleButtonTimer();

// task waiting on binarySemaphore from ISR
void vOledTask(void* pvParameters)
{
    oled_config* config = (oled_config*)pvParameters;
//    char buffer1[7];
    char buffer2[5];
#ifdef DBG
    uint16_t presses = 0;
#endif
    while(true)
    {
        xSemaphoreTake(sema, portMAX_DELAY);
        if(gpio_get(button1.gpio) == 0)
        {
//          int size1 = sprintf(buffer1, "Task %d", config->task);
            // use sprintf to update a buffer of 0000 with the number
            int size2 = sprintf(buffer2, "%04d", config->val);

//          write_string(buffer1, config->page, 0, size1 * 8);
            display_write_string(&sh1106_display, buffer2, config->page+1, 0, size2 * 8);
            display_update(&sh1106_display);
            config->val++;
#ifdef DBG
printf("Presses: %d\n", ++presses);
#endif
        }
        gpio_set_irq_enabled(button1.gpio, GPIO_IRQ_EDGE_FALL, true);
    }
}

/*
* @brief ISR that will fire when GPIO signal is low
*
*/
void vButtonHandlerISR()
{
    gpio_set_irq_enabled(button1.gpio, GPIO_IRQ_EDGE_FALL, false);
    xTimerStartFromISR(h_debounce, 0);
}

void setup_sh1106(void)
{
    setup_display_sh1106(&sh1106_display, sh1106_pgs, sh1106_info, SH1106_WIDTH, SH1106_HEIGHT, SH1106_PAGES, sh1106_buffer);

    sh1106_display.ctx = (void*)&sh1106_ctx;
    
    spi_transport_init((SpiTransportConfig*)sh1106_display.ctx);
    
    display_init(&sh1106_display);
    
    display_clear(&sh1106_display);    
}

int main()
{
    // using compand literals
    setupOledConfig(&task1, (oled_config){.task = 1, .page = 3, .val = 0, .ticks=1000});
    setupButtonConfig(&button1, (button_state){.gpio = 15});

    // setup GPIO pins
    gpio_init(button1.gpio);
    gpio_set_dir(button1.gpio, GPIO_IN);
    gpio_pull_up(button1.gpio);

    stdio_init_all();
    setup_sh1106();
#ifdef DBG
sleep_ms(10000); //[TODO] give me time to connect via minicom
printf("starting...\n");
#endif
    sema = xSemaphoreCreateBinary();
    h_debounce = xTimerCreate("Button_Debounce_Timer",
                              pdMS_TO_TICKS(DEBOUNCE_MS),
                              pdFALSE,
                              NULL,
                              vSampleButtonTimer);

    if(sema != NULL && h_debounce != NULL)
    {
        gpio_set_irq_enabled_with_callback(button1.gpio, GPIO_IRQ_EDGE_FALL, true, vButtonHandlerISR); 

        xTaskCreate(vOledTask, "Button_Task1", 256, &task1, 1, NULL); 
        vTaskStartScheduler(); 
    }

    while(1){};

    return 0;
}

/*
* @brief Function that is called when Timer expires to release semaphore
*
*/
void vSampleButtonTimer()
{
    xSemaphoreGive(sema);
}

static void setupOledConfig(oled_config* dst, oled_config src)
{
    dst->task = src.task;
    dst->page = src.page;
    dst->val = src.val;
    dst->ticks = src.ticks;
}

static void setupButtonConfig(button_state* dst, button_state src)
{
    dst->gpio = src.gpio;
    dst->state = src.state;
}
