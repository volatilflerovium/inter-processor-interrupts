#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

#include "xil_printf.h"
#include "xil_exception.h"
#include "xipipsu.h"
#include "xipipsu_hw.h"
#include "xil_cache.h"

//===================================================================

//All the cores in the platform project
enum XIPI_CORE_TARGETS
{
	A53_0=0,
	A53_1,
	R5_0,
	R5_1,
	MBLAZE,
	XTARGET_COUNT
};

extern const u32 XIPI_TARGETS[XTARGET_COUNT];

//===================================================================

#ifdef ARM_A53_0
	#define XID A53_0
#elif defined ARM_A53_1
	#define XID A53_1
#elif defined ARM_R5_0
	#define XID R5_0
#elif defined ARM_R5_1
	#define XID R5_1
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
#elif defined ARM_R5_1
	#define SENDER "Cortex-R5[1]"
#elif defined MICROBLAZE
	#define SENDER "MicroBlaze"
#endif

//===================================================================


#endif

