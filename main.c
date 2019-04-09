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
    //https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/l4/stm32l476g-disco/basics/basics.c
    rcc_osc_on(RCC_HSI16);
	
    flash_prefetch_enable();
    flash_set_ws(4);
    flash_dcache_enable();
    flash_icache_enable();
    /* 16MHz / 4 = > 4 * 40 = 160MHz VCO => 80MHz main pll  */
    rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSI16, 4, 40,
			0, 0, RCC_PLLCFGR_PLLR_DIV2);
    rcc_osc_on(RCC_PLL);
    /* either rcc_wait_for_osc_ready() or do other things */

    /* Enable clocks for the ports we need */
    //rcc_periph_clock_enable(RCC_GPIOA);

    /* Enable clocks for peripherals we need */
    rcc_periph_clock_enable(RCC_SYSCFG);

    rcc_set_sysclk_source(RCC_CFGR_SW_PLL); /* careful with the param here! */
    rcc_wait_for_sysclk_status(RCC_PLL);
    /* FIXME - eventually handled internally */
    rcc_ahb_frequency = 80e6;
    rcc_apb1_frequency = 80e6;
    rcc_apb2_frequency = 80e6;
}

static void task1(void *args) {
    int i;

    (void)args;

    //not sure if A11 is connected to something
    //gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11);
    
    for (;;) {
        //gpio_toggle(GPIOA,GPIO11);
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


