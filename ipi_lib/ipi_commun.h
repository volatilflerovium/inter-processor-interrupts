#ifndef IPI_COMMUN_H
#define IPI_COMMUN_H

#include <stdio.h>

#include "xil_printf.h"
#include "xil_exception.h"
#include "xipipsu.h"
#include "xipipsu_hw.h"
#include "xil_cache.h"

#include "core_config.h"

#ifdef MICROBLAZE
	#include "xintc.h"
#else
	#include "xscugic.h"
#endif

//===================================================================

extern XIpiPsu IpiInst;

#ifdef MICROBLAZE
	#define IPI_CHANNEL_ID 	XPAR_PSU_IPI_7_DEVICE_ID
	#define INTC_DEVICE_ID 	XPAR_INTC_0_DEVICE_ID

	extern XIntc InterruptController; /* Instance of the Interrupt Controller */
	#define IPI_Cntrlr_ptr &InterruptController
#else
	#define IPI_CHANNEL_ID	XPAR_XIPIPSU_0_DEVICE_ID
	#define INTC_DEVICE_ID	XPAR_SCUGIC_0_DEVICE_ID

	extern XScuGic GicInst;

	#ifdef FREERTOS
		extern XScuGic xInterruptController;
		#define IPI_Cntrlr_ptr &xInterruptController
	#else
		#define IPI_Cntrlr_ptr &GicInst
	#endif
#endif

//===================================================================

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#define WORD32_SIZE sizeof(uint32_t)

//===================================================================

#define IPI_MSG_SIZE 8

/*
 * For this setup, we are sending messages of 8 words.
 * */
typedef struct
{
	uint32_t sender		:16;
	uint32_t target		:16;
	uint32_t buff[IPI_MSG_SIZE-1];
} ipi_msg_t;

//===================================================================

/*
 * A IpiHander is a pointer to function that takes a pointer to 
 * a ipi_shmem_header_t.
 * */
typedef struct
{
	void (*IpiHandler)(const ipi_msg_t* header);
} ipi_hander_wraper_t;


/*
 * Initialize the interprocessor interrup system
 *
 * @param *p pointer to a hander which will process the income messages.
 *
 * @return TRUE on success FALSE otherwise
 * */
XStatus start_ipi(ipi_hander_wraper_t *p);

void Send_ipi_msg(u32 target, ipi_msg_t* ipi_msg_buff);

void Send_ipi_resp(u32 target, ipi_msg_t* ipi_msg_buff);

//===================================================================




#endif

