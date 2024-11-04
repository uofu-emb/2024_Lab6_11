/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

int count = 0;
bool on = false;

#define HIGH_PRIORITY_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define LOW_PRIORITY_PRIORITY     ( tskIDLE_PRIORITY + 2UL )
#define HIGH_PRIORITY_STACK_SIZE configMINIMAL_STACK_SIZE
#define LOW_PRIORITY_STACK_SIZE configMINIMAL_STACK_SIZE

void low_priority(__unused void *params) {

}

void high_priority(__unused void *params) {

}

int main( void )
{
    stdio_init_all();
    const char *rtos_name;
    rtos_name = "FreeRTOS";
    TaskHandle_t task;
    xTaskCreate(high_priority, "highPriorityThread",
                HIGH_PRIORITY_STACK_SIZE, NULL, HIGH_PRIORITY_PRIORITY, &task);
                
    TaskHandle_t task1;
    xTaskCreate(high_priority, "lowPriorityThread",
                LOW_PRIORITY_STACK_SIZE, NULL, LOW_PRIORITY_PRIORITY, &task1);
    vTaskStartScheduler();
    return 0;
}
