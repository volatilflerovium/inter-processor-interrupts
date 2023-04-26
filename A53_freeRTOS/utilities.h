#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>

#include "xil_printf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "message_buffer.h"

#include "../ipi_lib/ipi_shmem.h"

//===================================================================

#define DEBUG

#ifdef DEBUG
#define PrintF(format_string, ...)	xSemaphoreTake(print_mutex, portMAX_DELAY); printf(format_string, ##__VA_ARGS__); xSemaphoreGive(print_mutex);
#else
#define PrintF(format_string, ...)
#endif

#define THREAD_STACKSIZE 1024
#define DEFAULT_THREAD_PRIO 2

//-------------------------------------------------------------------

extern xTaskHandle blockedTask;

extern MessageBufferHandle_t receiver_buffer;


//-------------------------------------------------------------------

extern SemaphoreHandle_t print_mutex;

int sys_thread_new( const char *pcName, void( *pxThread )( void *pvParameters ), void *pvArg, int iStackSize, int iPriority );

int sys_new_task( const char *pcName, void( *pxThread )( void *pvParameters ), void *pvArg, int iStackSize, int iPriority, xTaskHandle* xCreatedTask_ptr);

uint32_t receive_data(void* buffer, uint32_t size);

int ipi_shmem_handler(const ipi_msg_t* MsgBuffer);

void wake_up2(void* p, uint32_t words);

//-------------------------------------------------------------------

// for testing
void PingPongTest(const ipi_msg_t* MsgBuffer);
void populate_mem(void* mem_p, uint32_t bytes);
void print_mem_data(void* p, uint32_t words);


//-------------------------------------------------------------------



#endif

