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

Vivado IPI master mapping:
[![image41.png](https://i.postimg.cc/NGkdMpZr/image41.png)](https://postimg.cc/hhhLZbDS)

[![A53_0](https://i.postimg.cc/Y0FGvnCL/A53-0.png)](https://postimg.cc/QKXxPq5j)

[![A53_1](https://i.postimg.cc/y8sgNT4W/A53-1.png)](https://postimg.cc/xcpTxMdS)

[![R5_0](https://i.postimg.cc/BvwXM981/R5-0.png)](https://postimg.cc/R3KSZYLM)

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

    const u32 XIPI_TARGETS[XTARGET_COUNT];

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

(unfinished...)

# Notes

Notice that despite the four A53 share the same IPI, our implementation
is able to route messages to a specific A53 core.

