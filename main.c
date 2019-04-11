#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>

#define mainECHO_TASK_PRIORITY                ( tskIDLE_PRIORITY + 1 )

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,signed portCHAR *pcTaskName);

void vApplicationStackOverflowHook(xTaskHandle *pxTask,signed portCHAR *pcTaskName)
{
    (void)pxTask;
    (void)pcTaskName;
    for(;;);
}

static void clock_setup(void)
{
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
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Enable clocks for peripherals we need */
	rcc_periph_clock_enable(RCC_USART3);
    rcc_periph_clock_enable(RCC_SYSCFG);

    rcc_set_sysclk_source(RCC_CFGR_SW_PLL); /* careful with the param here! */
    rcc_wait_for_sysclk_status(RCC_PLL);
    /* FIXME - eventually handled internally */
    rcc_ahb_frequency = 80e6;
    rcc_apb1_frequency = 80e6;
    rcc_apb2_frequency = 80e6;
}

static void usart_setup(void)
{
    /* Setup GPIO pins for USART3 transmit. */
    gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10|GPIO11);

    /* Setup USART3 TX pin as alternate function. */
    gpio_set_af(GPIOC, GPIO_AF7, GPIO10);

    usart_set_baudrate(USART3, 115200);
    usart_set_databits(USART3, 8);
    usart_set_stopbits(USART3, USART_STOPBITS_1);
    usart_set_mode(USART3, USART_MODE_TX);
    usart_set_parity(USART3, USART_PARITY_NONE);
    usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(USART3);
}

/**
 * Use USART3 as a console.
 * This is a syscall for newlib
 * @param file
 * @param ptr
 * @param len
 * @return
 */
int _write(int file, char *ptr, int len)
{
    int i;

    if (file == STDOUT_FILENO || file == STDERR_FILENO) {
        for (i = 0; i < len; i++) {
            if (ptr[i] == '\n') {
                usart_send_blocking(USART3, '\r');
            }
            usart_send_blocking(USART3, ptr[i]);
        }
        return i;
    }
    errno = EIO;
    return -1;
}

static void task1(void *args)
{
    int i;

    (void)args;
    
    for (;;) {
        printf("%d\n", i);
        for (i = 0; i < 300000; i++)
            __asm__("nop");
    }
}

int main(void)
{
    clock_setup();
    usart_setup();
    printf("hello world!\n");
    xTaskCreate(task1,"TASK1",100,NULL,configMAX_PRIORITIES-1,NULL);
    vTaskStartScheduler();
    for (;;);
    return 0;
}


