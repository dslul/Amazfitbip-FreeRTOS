#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>

#define mainECHO_TASK_PRIORITY                ( tskIDLE_PRIORITY + 1 )

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,signed portCHAR *pcTaskName);

void vApplicationStackOverflowHook(xTaskHandle *pxTask,signed portCHAR *pcTaskName) {
    (void)pxTask;
    (void)pcTaskName;
    for(;;);
}

static void clock_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
}

static void task1(void *args) {
    int i;

    (void)args;

    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11);
    
    for (;;) {
        gpio_toggle(GPIOA,GPIO11);
        for (i = 0; i < 300000; i++)
            __asm__("nop");
    }
}

int main(void) {

    clock_setup();
    xTaskCreate(task1,"TASK1",100,NULL,configMAX_PRIORITIES-1,NULL);
    vTaskStartScheduler();
    for (;;)
        ;
    return 0;
}


