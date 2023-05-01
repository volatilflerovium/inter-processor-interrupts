#ifndef SHARED_MEMORY_SETUP_H
#define SHARED_MEMORY_SETUP_H

#include "xipipsu.h"
#include "ipi_shmem.h"

//===================================================================
/*
 *
 *
 *
 *
 *
 * */
// All the buffers across all the platforms running on the board
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

//===================================================================

#ifdef ARM_A53_0
	//#define TO_A531_BUFF ipi_buffers[A530_TO_A531]
#elif defined ARM_A53_1
	//#define TO_R50 A531_TO_R50
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

//===================================================================



#endif

