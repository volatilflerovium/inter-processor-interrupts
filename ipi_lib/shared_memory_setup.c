#include "shared_memory_setup.h"

//===================================================================

Shared_Mem_Block ipi_buffers[TOTAL_BUFFERS]={
/*R50_TO_A530*/{.SHARED_BUFFER_ADDR=0xFFFC0000,.BUFFER_LENGTH=WORD32_SIZE*1024, .head=0, .target_idx=A53_0},
/*R50_TO_A531*/{.SHARED_BUFFER_ADDR=0xFFFC0000+WORD32_SIZE*1024,.BUFFER_LENGTH=WORD32_SIZE*1024, .head=0, .target_idx=A53_1},
/*MBLAZE_TO_R50*/{.SHARED_BUFFER_ADDR=0xFFFC0000+2*WORD32_SIZE*1024,.BUFFER_LENGTH=WORD32_SIZE*1024, .head=0, .target_idx=R5_0},
/*MBLAZE_TOA531*/{.SHARED_BUFFER_ADDR=0xFFFC0000+3*WORD32_SIZE*1024,.BUFFER_LENGTH=WORD32_SIZE*1024, .head=0, .target_idx=A53_1},
/*R50_TO_MBLAZE*/{.SHARED_BUFFER_ADDR=0xFFFC0000+4*WORD32_SIZE*1024,.BUFFER_LENGTH=WORD32_SIZE*1024, .head=0, .target_idx=MBLAZE},
};

//===================================================================



