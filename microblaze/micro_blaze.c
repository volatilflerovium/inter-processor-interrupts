#include <stdio.h>
#include <sleep.h>

#include "platform.h"
#include "xil_printf.h"
#include "xil_exception.h"
//#include "xscugic.h"
#include "xintc.h"
#include "xipipsu.h"
#include "xipipsu_hw.h"
#include "xil_cache.h"

#include "../ipi_lib/shared_memory_setup.h"
#include "../ipi_lib/ipi_shmem.h"

#include "utilities.h"

//===================================================================


int main()
{
    init_platform();

	xil_printf("Test IPI Demo on %s\r\n", SENDER);

	ipi_hander_wraper_t reader={.IpiHandler=PingPongTest};
	start_ipi(&reader);

	//Write2SharedMem(32, populate_mem);

    while (1) {
    	//Write2SharedMem(&ipi_buffers[TO_R51], WORD32_SIZE*32, populate_mem);

    	//xil_printf("in %s...\r\n", SENDER);
    	sleep(1);
    };

    cleanup_platform();


    return 0;
}






