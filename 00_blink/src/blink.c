#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "pico/stdlib.h"

static QueueHandle_t xQueue = NULL;

void led_task()
{   
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    int uIValueToSend;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        uIValueToSend = 1;
        xQueueSend(xQueue, &uIValueToSend, 0U);
        vTaskDelay(100);
        
        gpio_put(LED_PIN, 0);
        uIValueToSend = 0;
        xQueueSend(xQueue, &uIValueToSend, 0U);        
        vTaskDelay(100);
    }
}

void usb_task(void* pvParameters)
{
	uint uIReceiveValue;
	
	while(1)
	{
		xQueueReceive(xQueue, &uIReceiveValue, portMAX_DELAY);
		if(uIReceiveValue)
		{
			printf("LED is ON!\n");
		}
		else
		{
			printf("LED is OFF!\n");		
		}
	}
}

int main()
{
    stdio_init_all();

	xQueue = xQueueCreate(1, sizeof(uint));

    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(usb_task, "USB_Task", 256, NULL, 1, NULL);    
    vTaskStartScheduler();

    while(1){};
}
