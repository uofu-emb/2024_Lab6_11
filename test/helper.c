#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "helper.h"

#define TEST_DURATION 5000

void run_analyzer(TaskFunction_t pri_thread_entry,
                  int pri_priority,  uint32_t pri_delay,
                  configRUN_TIME_COUNTER_TYPE *pri_duration,

                  TaskFunction_t sec_thread_entry,
                  int sec_priority,  uint32_t sec_delay,
                  configRUN_TIME_COUNTER_TYPE *sec_duration,

                  configRUN_TIME_COUNTER_TYPE *total_duration,
                  TickType_t *total_ticks)
                  {
                    TaskHandle_t primaryTask, secondaryTask;

                    char *primaryName = "primaryTask";
                    char *secondaryName = "secondaryTask";


                    TickType_t start_ticks = xTaskGetTickCount();
                    configRUN_TIME_COUNTER_TYPE start_count = portGET_RUN_TIME_COUNTER_VALUE();

                    vTaskDelay(pri_delay);

                    xTaskCreate(pri_thread_entry,"primaryTask",configMINIMAL_STACK_SIZE,primaryName,pri_priority,&primaryTask);

                    vTaskDelay(sec_delay);

                    xTaskCreate(sec_thread_entry, "secondaryTask",configMINIMAL_STACK_SIZE,secondaryName,sec_priority,&secondaryTask);

                    TickType_t end_ticks = xTaskGetTickCount();
                    configRUN_TIME_COUNTER_TYPE end_count = portGET_RUN_TIME_COUNTER_VALUE();
                    configRUN_TIME_COUNTER_TYPE primary = ulTaskGetRunTimeCounter(primaryTask);
                    configRUN_TIME_COUNTER_TYPE secondary = ulTaskGetRunTimeCounter(secondaryTask);
                    configRUN_TIME_COUNTER_TYPE elapsed = end_count - start_count;
                    TickType_t elapsed_ticks = end_ticks - start_ticks;



                    *pri_duration = primary;
                    *sec_duration = secondary;
                    *total_duration = elapsed;
                    *total_ticks = elapsed_ticks;

                    vTaskDelete(primaryTask);
                    vTaskDelete(secondaryTask);

                  }