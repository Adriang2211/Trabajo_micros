/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#include "syslib/cy_syslib.h"

#warning This is a template. Modify it according to your project and remove this line. 

#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
#define configCPU_CLOCK_HZ                      SystemCoreClock
#define configTICK_RATE_HZ                      10000u //Original 1000u
#define configMAX_PRIORITIES                    7
#define configMINIMAL_STACK_SIZE                128
#define configMAX_TASK_NAME_LEN                 16
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_TASK_NOTIFICATIONS            1
#define configUSE_MUTEXES                       0
#define configUSE_RECURSIVE_MUTEXES             0
#define configUSE_COUNTING_SEMAPHORES           0
#define configQUEUE_REGISTRY_SIZE               10
#define configUSE_QUEUE_SETS                    0
#define configUSE_TIME_SLICING                  0
#define configUSE_NEWLIB_REENTRANT              0
#define configENABLE_BACKWARD_COMPATIBILITY     0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   10240
#define configAPPLICATION_ALLOCATED_HEAP        0

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configUSE_MALLOC_FAILED_HOOK            0
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               3
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* FreeRTOS MPU specific definitions. */
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

/*
Interrupt nesting behavior configuration.
This is explained here: http://www.freertos.org/a00110.html

Priorities are controlled by two macros:
- configKERNEL_INTERRUPT_PRIORITY determines the priority of the RTOS daemon task
- configMAX_API_CALL_INTERRUPT_PRIORITY dictates the priority of ISRs that make API calls

Notes:
1. Interrupts that do not call API functions should be >= configKERNEL_INTERRUPT_PRIORITY
   and will nest.
2. Interrupts that call API functions must have priority between KERNEL_INTERRUPT_PRIORITY
   and MAX_API_CALL_INTERRUPT_PRIORITY (inclusive).
3. Interrupts running above MAX_API_CALL_INTERRUPT_PRIORITY are never delayed by the OS.
*/
/*
PSoC 6 __NVIC_PRIO_BITS = 3

0 (high)
1           MAX_API_CALL_INTERRUPT_PRIORITY 001xxxxx (0x3F)
2
3
4
5           
6
7 (low)     KERNEL_INTERRUPT_PRIORITY       111xxxxx (0xFF)

!!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html

If you call a FreeRTOS API function from an interrupt with priority higher than 
MAX_API_CALL_INTERRUPT_PRIORITY FreeRTOS will generate an exception. If you need 
to call a FreeRTOS API function from your system’s highest priority interrupt 
you must reduce all interrupt priorities to MAX_API_CALL_INTERRUPT_PRIORITY or 
lower.

If your system pipe (IPC) interrupt priority is less than or equal to 
MAX_API_CALL_INTERRUPT_PRIORITY then care must be taken with code that writes to 
flash (including the Flash/BLE/Emulated EEPROM/Bootloader drivers from Cypress 
PDL). The duration of critical sections must be kept short - see the 
Configuration Considerations section of the flash driver in the PDL API 
Reference.  

*/

/* Put KERNEL_INTERRUPT_PRIORITY in top __NVIC_PRIO_BITS bits of CM4 register */
#define configKERNEL_INTERRUPT_PRIORITY         0xFF
/* 
Put MAX_SYSCALL_INTERRUPT_PRIORITY in top __NVIC_PRIO_BITS bits of CM4 register
NOTE For IAR compiler make sure that changes of this macro is reflected in  
file portable\IAR\CM4F\portasm.s in PendSV_Handler: routine
*/
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    0x3F
/* configMAX_API_CALL_INTERRUPT_PRIORITY is a new name for configMAX_SYSCALL_INTERRUPT_PRIORITY
 that is used by newer ports only. The two are equivalent. */
#define configMAX_API_CALL_INTERRUPT_PRIORITY 	configMAX_SYSCALL_INTERRUPT_PRIORITY


/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              1

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names - or at least those used in the unmodified vector table. */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
