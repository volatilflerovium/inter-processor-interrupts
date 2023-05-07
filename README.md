# Inter Processor Interrupts
C library for shared memory and messaging using inter processor interrupts. 

Design based on the Xilinx Vitis Unified Software Development Platform 
and targeted to the Xilinx Zynq UltraScale+ MPSoC.

# Introduction

This project is based on [Inter Processor Interrupts and Message Buffers](https://ohwr.org/project/soc-course/wikis/Inter-Processor-Interrupts-and-Message-Buffers)

The idea is to exchange interrup messages among the processors in the
system. Theses messages contain information about the sender, the 
memory addres and the size of the data at that memory address.

# Project organization

For this project we have deployed 2 A53 processors (one running baremetal and the
other freeRTOS), 1 R5 (running freeRTOS) and 1 microblaze (running baremetal).

The directory ipi_lib needs to be shared across all the cores.

Code for each processor is located in respective directory.

# Setup

**Vivado IPI master mapping**

[![image41.png](https://i.postimg.cc/NGkdMpZr/image41.png)](https://postimg.cc/hhhLZbDS)

**Memory region setup**

We use OCM RAM with base address at 0xFFFC0000

A53_0
[![A53_0](https://i.postimg.cc/Y0FGvnCL/A53-0.png)](https://postimg.cc/QKXxPq5j)

A53_1
[![A53_1](https://i.postimg.cc/y8sgNT4W/A53-1.png)](https://postimg.cc/xcpTxMdS)

R5_0
[![R5_0](https://i.postimg.cc/BvwXM981/R5-0.png)](https://postimg.cc/R3KSZYLM)

microBlaze
[![MicroBlaze](https://i.postimg.cc/x8GjrWd5/MBZ.png)](https://postimg.cc/BP6fH7HP)


For each processor in the system we define a preprocessor macro using the -D option flag 
[![d-symbol](https://i.postimg.cc/fTVrkxs7/d-symbol.png)](https://postimg.cc/6yN0m4yT)

So, in this example our system has 2 A53, 1 R5 and 1 microBlazefor, therefore we define:
 
    in processor A53_0 macro: ARM_A53_0 
    in processor A53_1 macro: ARM_A53_1
    in processor R5_0 macro: ARM_R5_0
    and in the microblaze macro MICROBLAZE

We define a enumeration 

    enum XIPI_CORE_TARGETS
    {
		A53_0=0,
		A53_1,
		R5_0,
		MBLAZE,
		XTARGET_COUNT
    };

and we used this enumeration to index an array that contains IPI's of 
the system

    const u32 XIPI_TARGETS[XTARGET_COUNT]={
		XPAR_XIPIPS_TARGET_PSU_CORTEXA53_0_CH0_MASK,
		XPAR_XIPIPS_TARGET_PSU_CORTEXA53_1_CH0_MASK,
		XPAR_XIPIPS_TARGET_PSU_CORTEXR5_0_CH0_MASK,
		XPAR_XIPIPS_TARGET_MICROBLAZE_0_CH1_MASK
    };

Notice that all the A53 processors share the same IPI,  this means that 
XPAR_XIPIPS_TARGET_PSU_CORTEXA53_0_CH0_MASK, ...,
XPAR_XIPIPS_TARGET_PSU_CORTEXA53_3_CH0_MASK have the same value.

Finally, using the macros, we capture the index of the IPI for the current
processor in the array XIPI_TARGETS:

	#ifdef ARM_A53_0
		#define XID A53_0
	#elif defined ARM_A53_1
		#define XID A53_1
	#elif defined ARM_R5_0
		#define XID R5_0
	#elif defined MICROBLAZE
	    #define XID MBLAZE
    #endif

<!--
For this setup, we are sending messages of 8 words in the format of the
following structure:

	typedef struct
	{
		uint32_t sender		:16;
		uint32_t target		:16;
		uint32_t buff[7];
	} ipi_msg_t;

We use the first word to register the index in the array XIPI_TARGETS 
of the sender and the index of the target processor.
-->

**Shared Memory Setup**

    shared_memory_setup.h

If we want processor P1 to send data via shared memory to processor P2
you we to setup a dedicated memory block where P1 will write to
and P2 will read from. This block is not the same if P2 want to send
data to P1 via shared memory. For the sake of the argument, let's
call each of these pairs a channel. 

Therefore we define an enumeration with all the channels that our
system requires. We use this enumeration to index the array of
Share_Mem_Block's 

    enum MEM_INDEX
    {
        R50_TO_A530,
        R50_TO_A531,
        MBLAZE_TO_R50,
        MBLAZE_TO_A531,
        R50_TO_MBLAZE,
        TOTAL_BUFFERS
    };

    extern Shared_Mem_Block ipi_buffers[TOTAL_BUFFERS];

Further simplification

    #ifdef ARM_A53_0
	    //A53_0 is not passing data via shared memory
    #elif defined ARM_A53_1
	    //A53_1 is not passing data via shared memory
    #elif defined ARM_R5_0
        #define TO_A530 R50_TO_A530
        #define TO_A531 R50_TO_A531
        #define TO_MBLAZE R50_TO_MBLAZE
    #elif defined ARM_R5_1
        #define TO_A530 R51_TO_A530
    #elif defined MICROBLAZE
        #define TO_R50 MBLAZE_TO_R50
        #define TO_A531 MBLAZE_TO_A531
    #endif

So in shared_memory_setup.h, ipi_buffers is 

	Shared_Mem_Block ipi_buffers[TOTAL_BUFFERS]={
	/*R50_TO_A530*/{
	                .SHARED_BUFFER_ADDR=0xFFFC0000, 
	                .BUFFER_LENGTH=WORD32_SIZE*1024, 
	                .head=0, 
	                .target_idx=A53_0
	               },
	/*R50_TO_A531*/{
	                .SHARED_BUFFER_ADDR=0xFFFC0000+WORD32_SIZE*1024, 
	                .BUFFER_LENGTH=WORD32_SIZE*1024, 
	                .head=0, 
	                .target_idx=A53_1
	                },
	/*MBLAZE_TO_R50*/{
    	              .SHARED_BUFFER_ADDR=0xFFFC0000+2*WORD32_SIZE*1024,
    	              .BUFFER_LENGTH=WORD32_SIZE*1024, 
    	              .head=0, 
    	              .target_idx=R5_0
    	             },
	/*MBLAZE_TOA531*/{
    	              .SHARED_BUFFER_ADDR=0xFFFC0000+3*WORD32_SIZE*1024,
    	              .BUFFER_LENGTH=WORD32_SIZE*1024, 
    	              .head=0, 
    	              .target_idx=A53_1
    	             },
	/*R50_TO_MBLAZE*/{
    	              .SHARED_BUFFER_ADDR=0xFFFC0000+4*WORD32_SIZE*1024,
    	              .BUFFER_LENGTH=WORD32_SIZE*1024, 
    	              .head=0, 
    	              .target_idx=MBLAZE
    	             }
	};

# Example

In baremetal

	void generate_data_and_sent(...some_args...);

	int main()
	{
	    init_platform();
	
		xil_printf("Test IPI Demo on %s\r\n", SENDER);

		/*We setup a custom function to handle incoming messages*/
		ipi_hander_wraper_t reader={.IpiHandler=message_hander_function};

		/*We start the IPI system*/
		start_ipi(&reader);

	    while (1) {
			/*we pass a function that generates data that need to be sent
              to another processor in the system...
            */
    		Write2SharedMem(&ipi_buffers[TO_R51], WORD32_SIZE*32, populate_mem);

			// or
			
			generate_data_and_sent(...some_args...);

    		sleep(1);
    	};

	    cleanup_platform();

	    return 0;
	}

	void generate_data_and_sent(...some_args...)
	{
		/*
		proces data, and generate more data...
		put that data in a buffer of size N (source_buffer, buffer_size respectively)
		and sent it to the specific processor in the system
		*/

		WriteBuff2SharedMem(&ipi_buffers[TO_TARGET], source_buff, buffer_size);
	}

See directory microblaze for the definition of the functions used.


# Library

The function that will consume the message from another processor is
wrapped in a structure:

	/*
	 * A IpiHander is a pointer to function that takes a pointer to 
	 * a ipi_shmem_header_t.
	 *
	 * */
	typedef struct
	{
		void (*IpiHandler)(const ipi_msg_t* header);
	} ipi_hander_wraper_t;


	/*
	 * Initialize the interprocessor interrup system
	 *
	 * @param *p pointer to a hander which will process income messages.
	 *
	 * @return TRUE on success FALSE otherwise
	 *
	 * */

	XStatus start_ipi(ipi_hander_wraper_t *p);


	/*
	 * Copy the data from the respective shared memory block into a buffer.
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

We define

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
	 * After the allocated memory is populated it can be sent
	 *
	 * */

	void SendBuffer(allocated_mem_t* mem);

	/*
	 * To get a pointer to the memory where the received data start
	 * we can use
	 *
	 * */

	buffer_data_t GetBufferData(ipi_shmem_header_t* MsgBuffer);

which return a structure:

	typedef struct
	{
		const void* const data_ptr;
		const uint32_t data_length;
	} buffer_data_t;


# Notes

Notice that despite the four A53 share the same IPI, our implementation
is able to route messages to a specific A53 core.



