#ifndef IPI_SHMEM_H
#define IPI_SHMEM_H

#include "ipi_commun.h"

//===================================================================
/*
 * Passing data via shared memory is achieved using dedicated memory
 * blocks. 
 * If we want processor P1 to send data via shared memory to processor P2
 * you we to setup a dedicated memory block where P1 will write to
 * and P2 will read from. This block is not the same if P2 want to send
 * data to P1 via shared memory. 
 *
 * These blocks are implemented as circular buffer.
 *
 * */

typedef struct
{
	const uintptr_t SHARED_BUFFER_ADDR; // base address for the block
	const uint32_t BUFFER_LENGTH;	    // length of the block in bytes
	uint32_t head;                      // current available position in the block
	uint32_t target_idx;                // the enumeration (enum XIPI_CORE_TARGETS) value for the target processor  
} Shared_Mem_Block;

//===================================================================

typedef struct
{
	uint32_t sender		:16; // the enumeration (enum XIPI_CORE_TARGETS) value for the sender processor
	uint32_t target		:16; // the enumeration (enum XIPI_CORE_TARGETS) value for the target processor
	uint32_t mem_block_idx;  // the enumeration (enum MEM_INDEX) value for memory block where data is written
	uint32_t offset;         // position in the memory block where data start
	uint32_t data_length;    // length (in bytes) of the data 
	uint32_t padding[IPI_MSG_SIZE-4]; // 4 is for 4 words used by the other data members of this structure    
} ipi_shmem_header_t;

/*
#define IPI_SHMEM_HEADER_SIZE ((WORD32_SIZE-1)+sizeof(ipi_shmem_header_t))/WORD32_SIZE // words

typedef struct
{
	ipi_shmem_header_t header;
	uint32_t padding[IPI_MSG_SIZE-IPI_SHMEM_HEADER_SIZE];
} ipi_shmem_adapter_t;
*/

typedef union
{
	ipi_msg_t buff;
	ipi_shmem_header_t shmem_header;
} ipi_msg_2_ipi_shmem_header_u;

//===================================================================

/*
 * Copy the data from the respective shared memory block into 
 * a buffer.
 * 
 * @param buffer	destination buffer to copy the data to
 * @param buffer_size the size (in bytes) of the buffer
 *
 * */
uint32_t CopyMem2Buffer(ipi_shmem_header_t* MsgBuffer, void* buffer, uint32_t buffer_size);


/*
 * Write data into a specific memory block using a callback function. 
 * After the data is written, it trigger the ipi system.
 *
 * @param mem_block pointer to a Shared_Mem_Block where data will be written.
 *
 * @param words number of words expected to be written to shared memory. 
 * Internally, if its value is less than the maximum value allowed, 
 * it is passed as the second argument for the callback function.
 *
 * @param pupulate_memory pointer to a callback which takes two parameters, a pointer to
 *        the memory location to write to and the number of words to be written.
 *
 * */

void Write2SharedMem(Shared_Mem_Block* mem_block, const uint32_t words, void (*populate_mem)(void *p, uint32_t words));


/*
 * Write data from a specific buffer into a specific memory block. 
 * After the data is written, it trigger the ipi system.
 *
 * @param mem_block pointer to a Shared_Mem_Block where data will be written.
 *
 * @param src_buff pointer to the buffer that contains the data to be copy.
 *
 * @param words number of words expected to be written to shared memory. 
 * Internally, if its value is less than the maximum value allowed, 
 * it is passed as the second argument for the callback function.
 *
 * */
void WriteBuff2SharedMem(Shared_Mem_Block* mem_block, void *src_buff, uint32_t words);

//===================================================================

typedef struct
{
	const uint32_t sender		:16;
	const uint32_t target		:16;
	const uint32_t mem_block_idx;
	const uint32_t offset;
	const uint32_t data_length; //in bytes
	uintptr_t const addr;
} allocated_mem_t;

/*
 * It reserves memory in a specific shared memory block.
 *
 * @param mem_block a pointer to the specific Shared_Mem_block struct
 *        where we want to reserve memory.
 *
 * @param buffer_size the size of the block of reserve memory.
 *
 * @return it return a structure containing details of the memory block.
 *         When the reserve memory is populated, we need this structure
 *         in order to send the respective message.  
 * */

allocated_mem_t GetAllocatedBuffer(Shared_Mem_Block* mem_block, uint32_t buffer_size);

/*
 * 
 *
 * */
void SendBuffer(allocated_mem_t* mem);

typedef struct
{
	const void* const data_ptr;
	const uint32_t data_length;
} buffer_data_t;

buffer_data_t GetBufferData(ipi_shmem_header_t* MsgBuffer);

//===================================================================

#endif

