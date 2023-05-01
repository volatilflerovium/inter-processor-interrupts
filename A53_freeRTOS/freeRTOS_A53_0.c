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

void start_ipi_thread(void* p)
{
	PrintF("Starting ipi thread %s id: %d\r\n", SENDER, XID);

	ipi_hander_wraper_t reader={.IpiHandler=ipi_shmem_handler};
	start_ipi(&reader);

	vTaskDelay(2570/portTICK_PERIOD_MS);
	//Write2SharedMem(&ipi_buffers[TO_A531], WORD32_SIZE*32, populate_mem);
	while (1) {

		vTaskDelay(5570/portTICK_PERIOD_MS);
		//xil_printf("in A53 0...\r\n");
		//Write2SharedMem(&ipi_buffers[TO_R51], WORD32_SIZE*32, populate_mem);
	}

	vTaskDelete(NULL);
}

//===================================================================

void blocked_thread(void *p)
{
	PrintF("%s starting blocked task\r\n", SENDER);
	uint32_t buffer[32];
	while(1) {

		receive_data((void*)buffer,  32*sizeof(uint32_t));

		PrintF("%s got notification!\r\n", SENDER);
		for(int i=0; i<32; i++){
			xil_printf(" %d +", buffer[i]);
		}
		xil_printf("\r\n");//*/

		vTaskDelay(2500/portTICK_PERIOD_MS);
		//Write2SharedMem(&ipi_buffers[TO_A531], WORD32_SIZE*32, populate_mem);
	}

	vTaskDelete(NULL);
}
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

