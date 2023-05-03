#ifndef SHARED_MEMORY_SETUP_H
#define SHARED_MEMORY_SETUP_H

#include "xipipsu.h"
#include "ipi_shmem.h"

//===================================================================

/*
 * If we want processor P1 to send data via shared memory to processor P2
 * you we to setup a dedicated memory block where P1 will write to
 * and P2 will read from. This block is not the same if P2 want to send
 * data to P1 via shared memory. For the sake of the argument, let's
 * call each of these pairs a channel. 
 *
 * Therefore we define an enumeration with all the channels that our
 * system requires. We use this enumeration to index the array of
 * Share_Mem_Block's 
 *
 * */

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

/*
 * Further simplification
 *
 * */

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

//===================================================================



#endif

