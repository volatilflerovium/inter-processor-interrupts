#include "core_config.h"

//===================================================================

/*
 * Notice that all the A53 processors share the same IPI, 
 * this means that XPAR_XIPIPS_TARGET_PSU_CORTEXA53_0_CH0_MASK, ...,
 * XPAR_XIPIPS_TARGET_PSU_CORTEXA53_3_CH0_MASK
 * are all the same by definition
 *
 */
const u32 XIPI_TARGETS[XTARGET_COUNT]={
		XPAR_XIPIPS_TARGET_PSU_CORTEXA53_0_CH0_MASK,
		XPAR_XIPIPS_TARGET_PSU_CORTEXA53_1_CH0_MASK,
		XPAR_XIPIPS_TARGET_PSU_CORTEXR5_0_CH0_MASK,
		XPAR_XIPIPS_TARGET_MICROBLAZE_0_CH1_MASK
};

//===================================================================

