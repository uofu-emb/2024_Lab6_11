
#include "unity_config.h"
#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <unity.h>
#include "helper.c"

SemaphoreHandle_t semaphore;

configRUN_TIME_COUNTER_TYPE first_stats, second_stats, elapsed_stats;
TickType_t elapsed_ticks;

void setUp(void) {}

void tearDown(void) {}


void priority_inversion(void *args){
    while(1){
        xSemaphoreTake(semaphore, portMAX_DELAY);
        busy_wait_ms (1000);
        xSemaphoreGive(semaphore);
    }
}

void test_inversion_binary()
{
semaphore  = xSemaphoreCreateBinary();

run_analyzer(priority_inversion,tskIDLE_PRIORITY +(1),0,&first_stats,priority_inversion,tskIDLE_PRIORITY+(2),1,&second_stats,&elapsed_stats,&elapsed_ticks);

 //check outputs
vSemaphoreDelete(semaphore);
}


int main (void)
{
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();
    RUN_TEST(test_inversion_binary);
    sleep_ms(5000);
    return UNITY_END();
}
