
#include "unity_config.h"
#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <unity.h>
#include "helper.c"



configRUN_TIME_COUNTER_TYPE first_stats, second_stats, third_stats, elapsed_stats;
TickType_t elapsed_ticks;

void setUp(void) {}

void tearDown(void) {}

SemaphoreHandle_t semaphore;
void priority_inversion(void *args){
   // printf("\n priority inversion %s\n",(char*)args);
        xSemaphoreTake(semaphore, portMAX_DELAY);
       // printf("semaphore taken %s \n",(char*)args);
        busy_wait_ms(1000);
        xSemaphoreGive(semaphore);
        while(1);
}

void busy_wait(void *args){
    while(1){
        busy_wait_ms(1000);
    }
}

void test_inversion_binary()
{
semaphore  = xSemaphoreCreateBinary();
xSemaphoreGive(semaphore);

    run_analyzer(priority_inversion, tskIDLE_PRIORITY+(1), 0, &first_stats,busy_wait,tskIDLE_PRIORITY+(2),10,&second_stats,priority_inversion, tskIDLE_PRIORITY+(3), 10, &third_stats, &elapsed_stats, &elapsed_ticks);
    printf("\n First Stats %llu",first_stats);
    printf("\n second Stats %llu",second_stats);
    printf("\n third Stats %llu",third_stats);
    printf("\n runtime %llu\n",elapsed_stats);
    TEST_ASSERT(second_stats > first_stats);
    TEST_ASSERT(second_stats > third_stats);
vSemaphoreDelete(semaphore);
}

void test_inversion_mutex()
{
semaphore  = xSemaphoreCreateMutex();


    run_analyzer(priority_inversion, tskIDLE_PRIORITY+(1), 0, &first_stats,
                busy_wait,tskIDLE_PRIORITY+(2),10,&second_stats,
                    priority_inversion, tskIDLE_PRIORITY+(3), 10, &third_stats,
                    &elapsed_stats, &elapsed_ticks);
    printf("\n First Stats %llu",first_stats);
    printf("\n second Stats %llu",second_stats);
    printf("\n third Stats %llu",third_stats);
    printf("\n runtime %llu\n",elapsed_stats);
    TEST_ASSERT(third_stats > first_stats);
    TEST_ASSERT(third_stats > second_stats);
    TEST_ASSERT(first_stats > second_stats);
vSemaphoreDelete(semaphore);
}


void main_thread (void *args)
{
    while (1) {
        UNITY_BEGIN();
        //RUN_TEST(test_inversion_binary);
        RUN_TEST(test_inversion_mutex);
        UNITY_END();
        vTaskDelay(10000);
    }
}

int main(void)
{
    stdio_init_all();
    TaskHandle_t main_handle;
    xTaskCreate(main_thread, "MainThread",
                configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+10, &main_handle);
    vTaskStartScheduler();
    return 0;
}