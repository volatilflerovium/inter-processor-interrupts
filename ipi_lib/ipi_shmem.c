#include "ipi_shmem.h"
#include "shared_memory_setup.h"

//===================================================================

uint32_t CopyMem2Buffer(ipi_shmem_header_t* MsgBuffer, void* buffer, uint32_t buffer_size)
{
	uint32_t data_size=buffer_size< MsgBuffer->data_length ? buffer_size : MsgBuffer->data_length;

	uintptr_t addr=ipi_buffers[MsgBuffer->mem_block_idx].SHARED_BUFFER_ADDR+MsgBuffer->offset;
	memcpy(buffer, (void*)addr, data_size);

	return data_size;
}

//===================================================================

void Write2SharedMem(Shared_Mem_Block* mem_block, const uint32_t bytes, void (*populate_mem)(void *p, uint32_t bytes))
{
	uint32_t data_size=bytes<mem_block->BUFFER_LENGTH ? bytes : mem_block->BUFFER_LENGTH;

	if(mem_block->BUFFER_LENGTH-mem_block->head<data_size){
		mem_block->head=0;
	}

	uintptr_t addr=mem_block->SHARED_BUFFER_ADDR+mem_block->head;
	ipi_msg_2_ipi_shmem_header_u MsgBuffer;
	MsgBuffer.shmem_header.sender=XID;
	MsgBuffer.shmem_header.target=mem_block->target_idx;
	MsgBuffer.shmem_header.offset=mem_block->head;
	MsgBuffer.shmem_header.data_length=data_size;

	for(uint32_t i=0; i<TOTAL_BUFFERS;i++){
		if(ipi_buffers[i].SHARED_BUFFER_ADDR==mem_block->SHARED_BUFFER_ADDR){
			MsgBuffer.shmem_header.mem_block_idx=i;
			break;
		}
	}

	uint32_t round_to_words=WORD32_SIZE*((data_size+WORD32_SIZE-1)/WORD32_SIZE);
	mem_block->head+=round_to_words;// reserve space for the current writing, so another process can write after the current data in written

	populate_mem((void*)addr, data_size);

	xil_printf("SENDER ID: %d\r\n", MsgBuffer.shmem_header.sender);
	Xil_DCacheFlushRange(mem_block->SHARED_BUFFER_ADDR, mem_block->BUFFER_LENGTH);

	Send_ipi_msg(XIPI_TARGETS[mem_block->target_idx], &MsgBuffer.buff);
}

//===================================================================

void WriteBuff2SharedMem(Shared_Mem_Block* mem_block, void *src_buff, uint32_t bytes)
{
	uint32_t data_size=bytes<mem_block->BUFFER_LENGTH ? bytes : mem_block->BUFFER_LENGTH;

	if(mem_block->BUFFER_LENGTH-mem_block->head<data_size){
		mem_block->head=0;
	}

	uintptr_t addr=mem_block->SHARED_BUFFER_ADDR+mem_block->head;
	ipi_msg_2_ipi_shmem_header_u MsgBuffer;
	MsgBuffer.shmem_header.sender=XID;
	MsgBuffer.shmem_header.target=mem_block->target_idx;
	MsgBuffer.shmem_header.offset=mem_block->head;
	MsgBuffer.shmem_header.data_length=data_size;

	for(uint32_t i=0; i<TOTAL_BUFFERS;i++){
		if(ipi_buffers[i].SHARED_BUFFER_ADDR==mem_block->SHARED_BUFFER_ADDR){
			MsgBuffer.shmem_header.mem_block_idx=i;
		}
	}

	uint32_t round_to_words=WORD32_SIZE*((data_size+WORD32_SIZE-1)/WORD32_SIZE);
	mem_block->head+=round_to_words;// reserve space for the current writing, so another process can write after the current data in written

	memcpy((void*)addr, src_buff, data_size);

	Xil_DCacheFlushRange(mem_block->SHARED_BUFFER_ADDR, mem_block->BUFFER_LENGTH);

	Send_ipi_msg(XIPI_TARGETS[mem_block->target_idx], &MsgBuffer.buff);
}

//===================================================================

allocated_mem_t GetAllocatedBuffer(Shared_Mem_Block* mem_block, uint32_t buffer_size)
{
	uint32_t data_size=buffer_size <mem_block->BUFFER_LENGTH ? buffer_size : mem_block->BUFFER_LENGTH;

	if(mem_block->BUFFER_LENGTH-mem_block->head<data_size){
		mem_block->head=0;
	}

	uint32_t idx=0;
	for(uint32_t i=0; i<TOTAL_BUFFERS;i++){
		if(ipi_buffers[i].SHARED_BUFFER_ADDR==mem_block->SHARED_BUFFER_ADDR){
			idx=i;
		}
	}
	allocated_mem_t buff={.sender=XID,
			.target=mem_block->target_idx,
			.mem_block_idx=idx,
			.offset=mem_block->head,
			.data_length=data_size,
			.addr=(mem_block->SHARED_BUFFER_ADDR+mem_block->head)
			};

	uint32_t round_to_words=WORD32_SIZE*((data_size+WORD32_SIZE-1)/WORD32_SIZE);
	mem_block->head+=round_to_words;
	return buff;
}

//===================================================================

void SendBuffer(allocated_mem_t* data)
{
	ipi_msg_2_ipi_shmem_header_u MsgBuffer;
	MsgBuffer.shmem_header.sender=data->sender;
	MsgBuffer.shmem_header.target=data->target;
	MsgBuffer.shmem_header.mem_block_idx=data->mem_block_idx;
	MsgBuffer.shmem_header.offset=data->offset;
	MsgBuffer.shmem_header.data_length=data->data_length;

	Xil_DCacheFlushRange(ipi_buffers[data->mem_block_idx].SHARED_BUFFER_ADDR, ipi_buffers[data->mem_block_idx].BUFFER_LENGTH);
	Send_ipi_msg(XIPI_TARGETS[data->target], &MsgBuffer.buff);
}

//===================================================================

buffer_data_t GetBufferData(ipi_shmem_header_t* MsgBuffer)
{
	buffer_data_t data={.data_ptr=(void*)(ipi_buffers[MsgBuffer->mem_block_idx].SHARED_BUFFER_ADDR+MsgBuffer->offset), .data_length=MsgBuffer->data_length};
	return data;
}

//===================================================================
















