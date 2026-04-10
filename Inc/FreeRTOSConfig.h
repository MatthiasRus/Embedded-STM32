/*
 * FreeRTOSConfig.h
 *
 *  Created on: Apr 6, 2026
 *      Author: mat-ros
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32l476xx.h"
#define configTICK_TYPE_WIDTH_IN_BITS    TICK_TYPE_WIDTH_32_BITS
/* scheduler behaviour */
#define configUSE_PREEMPTION                    1
#define configUSE_TIME_SLICING                  1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0

/* clock — must match your HSI16 config */
#define configCPU_CLOCK_HZ                      16000000UL
#define configTICK_RATE_HZ                      1000U        /* 1ms tick */

/* task config */
#define configMAX_PRIORITIES                    5
#define configMINIMAL_STACK_SIZE                128U         /* words */
#define configMAX_TASK_NAME_LEN                 16

/* heap — heap_4 */
#define configTOTAL_HEAP_SIZE                   (8 * 1024)   /* 8KB from your 96KB SRAM */

/* hooks — all off for now */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configUSE_MALLOC_FAILED_HOOK            1            /* catch heap exhaustion */

/* debug — critical for catching stack overflows */
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configUSE_TRACE_FACILITY                0
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* software timers */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               3
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            256U

/* FreeRTOS API includes — what you want */
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_uxTaskGetStackHighWaterMark     1

/* Cortex-M4 interrupt config — critical, must match NVIC priorities */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5
#define configKERNEL_INTERRUPT_PRIORITY         \
    ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - 4) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    \
    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - 4) )

/* map FreeRTOS handlers to CMSIS names */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif
