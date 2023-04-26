#include <stdio.h>
#include <sleep.h>

#include "platform.h"
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
	init_platform();

	xil_printf("Test IPI Demo on %s\r\n", SENDER);

	ipi_hander_wraper_t reader={.IpiHandler=PingPongTest};
	start_ipi(&reader);

	while (1) {
    	//Write2SharedMem(&ipi_buffers[TO_R51], WORD32_SIZE*32, populate_mem);

    	//xil_printf("in A53_1...\r\n");
    	sleep(2.5);
	};

	cleanup_platform();

	return 0;
}






