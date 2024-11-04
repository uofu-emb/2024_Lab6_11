#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include <semphr.h>

SemaphoreHandle_t semaphore;

void setUp(void) {}

void tearDown(void) {}

void test_inversion_binary()
{
semaphore  = xSemaphoreCreateBoolean();
}


int main (void)
{
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();
    RUN_TEST(test_inversion);
    sleep_ms(5000);
    return UNITY_END();
}
