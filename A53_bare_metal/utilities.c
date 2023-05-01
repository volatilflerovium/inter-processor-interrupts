#include <sleep.h>
#include "utilities.h"

#include "../ipi_lib/shared_memory_setup.h"

//===================================================================

uint32_t receive_data(void* buffer, uint32_t buffer_size)// size in bytes
{

	//xil_printf("received data: offset: %d, words: %d\r\n", MsgBuffer.offset, MsgBuffer.data_length);
	return 1;//CopyMem2Buffer(&MsgBuffer, buffer, buffer_size);
}


//-------------------------------------------------------------------

void PingPongTest(const ipi_msg_t* MsgBuffer)
{
	ipi_msg_2_ipi_shmem_header_u data;
	data.buff=*MsgBuffer;

	ipi_shmem_header_t* header=&(data.shmem_header.header);

	xil_printf("ID: %d  target: %d x = x = x = x = x = x = x = x = x = x = sender: %d, offset: %d, words: %d\r\n", XID, header->target, header->sender, header->offset, header->data_length);

	Xil_DCacheFlushRange(ipi_buffers[header->mem_block_idx].SHARED_BUFFER_ADDR, ipi_buffers[header->mem_block_idx].BUFFER_LENGTH);
}

//-------------------------------------------------------------------

void populate_mem(void* mem_p, uint32_t bytes)
{
	static uint32_t counter=0;
	uint32_t words=bytes/WORD32_SIZE;
	uint32_t data_buffer[words];

	for(uint32_t i=0; i<words; i++){
		data_buffer[i]=counter+i;
		//xil_printf("%d : ", data_buffer[i]);
	}
	//xil_printf("\r\n\r\n");
	counter++;

	memcpy(mem_p, data_buffer, words*WORD32_SIZE);
}

//-------------------------------------------------------------------

void print_mem_data(void* p, uint32_t bytes)
{
	/*
	uint32_t* tmp_buffer=(uint32_t*)p;

	for(u32 idx = 0; idx <bytes; idx++) {
		//xil_printf("%d : ", tmp_buffer[idx]);
	}
	xil_printf("\r\n\r\n");//*/
}

//-------------------------------------------------------------------


