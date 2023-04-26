#include <stdio.h>
#include <sleep.h>

#include "FreeRTOS.h"
#include "task.h"
#include "xil_printf.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "xipipsu.h"
#include "xipipsu_hw.h"
#include "xil_cache.h"

#include "../ipi_lib/ipi_commun.h"
#include "../ipi_lib/shared_memory_setup.h"

#include "utilities.h"

//===================================================================

int main()
{
	print_mutex=xSemaphoreCreateMutex();
	receiver_buffer=xMessageBufferCreate(248);

	xil_printf("Test IPI Demo on %s\r\n", SENDER);

	sys_thread_new("test_thread_ipi", start_ipi_thread, 0, THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);

	sys_thread_new("blocked task", blocked_thread, 0, THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);

	vTaskStartScheduler();



    return 0;
}

