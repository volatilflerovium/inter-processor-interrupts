#include <sleep.h>
#include "utilities.h"

#include "../ipi_lib/shared_memory_setup.h"

//===================================================================

SemaphoreHandle_t print_mutex;

xTaskHandle blockedTask;

MessageBufferHandle_t receiver_buffer;

//-------------------------------------------------------------------

int sys_thread_new( const char *pcName, void( *pxThread )( void *pvParameters ), void *pvArg, int iStackSize, int iPriority )
{
	xTaskHandle xCreatedTask;
	portBASE_TYPE xResult;

	xResult = xTaskCreate( pxThread, ( const char * const) pcName, iStackSize, pvArg, iPriority, &xCreatedTask );

	if( xResult == pdPASS )
	{
		return 0;
	}

	return 1;
}

//-------------------------------------------------------------------

int sys_new_task( const char *pcName, void( *pxThread )( void *pvParameters ), void *pvArg, int iStackSize, int iPriority, xTaskHandle* xCreatedTask_ptr)
{
	portBASE_TYPE xResult;

	xResult = xTaskCreate( pxThread, ( const char * const) pcName, iStackSize, pvArg, iPriority, xCreatedTask_ptr );

	if( xResult == pdPASS )
	{
		return 0;
	}

	return 1;
}

//-------------------------------------------------------------------

uint32_t receive_data(void* buffer, uint32_t buffer_size)// size in bytes
{
	ipi_shmem_header_t MsgBuffer;
	if(xMessageBufferReceive(receiver_buffer, (void*)&MsgBuffer, sizeof(ipi_shmem_header_t), portMAX_DELAY)<1){
		xil_printf("error\r\n");
		return 0;
	}

	return CopyMem2Buffer(&MsgBuffer, buffer, buffer_size);
}

//-------------------------------------------------------------------

int ipi_shmem_handler(const ipi_msg_t* MsgBuffer)
{
	ipi_msg_2_ipi_shmem_header_u data;
	data.buff=*MsgBuffer;

	ipi_shmem_header_t* header=&(data.shmem_header.header);

	if(header->target==XID && header->sender<XTARGET_COUNT){
		xil_printf("ID: %d  target: %d x = x = x = x = x = x = x = x = x = x = sender: %d, offset: %d, words: %d\r\n", XID, header->target, header->sender, header->offset, header->data_length);

		//Xil_DCacheFlushRange(ipi_buffers[header->mem_block_idx].SHARED_BUFFER_ADDR, ipi_buffers[header->mem_block_idx].BUFFER_LENGTH);

		xMessageBufferSendFromISR(receiver_buffer, (void*)header, sizeof(ipi_shmem_header_t), NULL);
		return TRUE;
	}
	return FALSE;
}


//-------------------------------------------------------------------

void PingPongTest(const ipi_msg_t* MsgBuffer)
{
	ipi_msg_2_ipi_shmem_header_u data;
	data.buff=*MsgBuffer;

	ipi_shmem_header_t* header=&(data.shmem_header.header);
	//xil_printf("bytes: %d\r\n",  MsgBuffer->data_length);

	//print_mem_data((void*)MsgBuffer->addr, MsgBuffer->data_length);
}

//-------------------------------------------------------------------

void populate_mem(void* mem_p, uint32_t bytes)
{
	static uint32_t counter=0;
	uint32_t words=bytes/WORD32_SIZE;
	uint32_t data_buffer[words];
	xil_printf("sending data\r\n");
	for(uint32_t i=0; i<words; i++){
		data_buffer[i]=counter+i;
		//xil_printf("%d : ", data_buffer[i]);
	}
	//xil_printf("\r\n\r\n");
	counter++;

	memcpy(mem_p, data_buffer, bytes);
}

//-------------------------------------------------------------------

void print_mem_data(void* p, uint32_t words)
{
	uint32_t* tmp_buffer=(uint32_t*)p;
	xil_printf("Data sent.\r\n");
	for(u32 idx = 0; idx <words; idx++) {
		//xil_printf("%d : ", tmp_buffer[idx]);
	}
	xil_printf("\r\n\r\n");
}

//-------------------------------------------------------------------


