#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

#include "xil_printf.h"
#include "xil_exception.h"
#include "xipipsu.h"
#include "xipipsu_hw.h"
#include "xil_cache.h"

//===================================================================
/*
 * For each processor in the system we define a preprocessor macro
 * using the -D option flag. 

 * So, this example our system has 2 A53, 1 R5 and 1 microBlazefor, derefore
 * we define:
 *
 * in the A53_0 macro: ARM_A53_0 
 * in the A53_1 macro: ARM_A53_1
 * in the R5_0 macro: ARM_R5_0
 * and in the microblaze macro MICROBLAZE
 *
 * We enumerate all the processors in the system which we want
 * to exchange messages. This will help us to identify the actual
 * 
 * 
 *
 * */

enum XIPI_CORE_TARGETS
{
	A53_0=0,
	A53_1,
	R5_0,
	MBLAZE,
	XTARGET_COUNT
};

/*
 * We put all the IPI targets for the system in a array in the same
 * order as described by XIPI_CORE_TARGETS
 *
 * */

extern const u32 XIPI_TARGETS[XTARGET_COUNT];

//===================================================================

/*
 * We capture the index of the IPI for the current processor in 
 * the array XIPI_TARGETS.
 *
 * */

#ifdef ARM_A53_0
	#define XID A53_0
#elif defined ARM_A53_1
	#define XID A53_1
#elif defined ARM_R5_0
	#define XID R5_0
#elif defined MICROBLAZE
	#define XID MBLAZE
#endif

//===================================================================

//for testing:

#ifdef ARM_A53_0
	#define SENDER "Cortex-A53[0]"
#elif defined ARM_A53_1
	#define SENDER "Cortex-A53[1]"
#elif defined ARM_R5_0
	#define SENDER "Cortex-R5[0]"
#elif defined MICROBLAZE
	#define SENDER "MicroBlaze"
#endif

//===================================================================


#endif

