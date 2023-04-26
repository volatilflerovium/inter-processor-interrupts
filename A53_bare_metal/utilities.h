#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>

#include "xil_printf.h"

#include "../ipi_lib/ipi_shmem.h"

//===================================================================

#define DEBUG

#ifdef DEBUG
#define PrintF(format_string, ...)	printf(format_string, ##__VA_ARGS__);
#else
#define PrintF(format_string, ...)
#endif


#define THREAD_STACKSIZE 1024
#define DEFAULT_THREAD_PRIO 2

//-------------------------------------------------------------------


uint32_t receive_data(void* buffer, uint32_t size);

//-------------------------------------------------------------------

// for testing
int PingPongTest(const ipi_msg_t* MsgBuffer);
void populate_mem(void* mem_p, uint32_t bytes);
void print_mem_data(void* p, uint32_t bytes);


//===================================================================






#endif

