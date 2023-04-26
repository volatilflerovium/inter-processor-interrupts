#include "core_config.h"

//===================================================================

/*
 * Notice that XPAR_XIPIPS_TARGET_PSU_CORTEXA53_0_CH0_MASK and
 * XPAR_XIPIPS_TARGET_PSU_CORTEXA53_1_CH0_MASK
 * are the same by definition
 */
const u32 XIPI_TARGETS[XTARGET_COUNT]={
		XPAR_XIPIPS_TARGET_PSU_CORTEXA53_0_CH0_MASK,
		XPAR_XIPIPS_TARGET_PSU_CORTEXA53_1_CH0_MASK,
		XPAR_XIPIPS_TARGET_PSU_CORTEXR5_0_CH0_MASK,
		XPAR_XIPIPS_TARGET_PSU_CORTEXR5_1_CH0_MASK,
		XPAR_XIPIPS_TARGET_MICROBLAZE_0_CH1_MASK
};

//===================================================================

