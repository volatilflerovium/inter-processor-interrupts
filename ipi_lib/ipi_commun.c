#include "ipi_commun.h"

//===================================================================

XIpiPsu IpiInst;

#ifdef MICROBLAZE
	XIntc InterruptController;
#else
	XScuGic GicInst;
#endif

//===================================================================

static XStatus SetUpInterruptSystem(ipi_hander_wraper_t* handler_wrp);

static void IpiIntrHandler(void* hander_wpr_ptr);

XStatus start_ipi(ipi_hander_wraper_t* p)
{
	//Look Up the config data
	XIpiPsu_Config* CfgPtr=XIpiPsu_LookupConfig(IPI_CHANNEL_ID);

	// Init with the Cfg Data
	if(XST_FAILURE==XIpiPsu_CfgInitialize(&IpiInst, CfgPtr, CfgPtr->BaseAddress)){
		return XST_FAILURE;
	}

	XStatus status=SetUpInterruptSystem((ipi_hander_wraper_t*)p);

	XIpiPsu_InterruptEnable(&IpiInst, XIPIPSU_ALL_MASK);

	/* Clear Any existing Interrupts */
	XIpiPsu_ClearInterruptStatus(&IpiInst, XIPIPSU_ALL_MASK);

	return status;
}

//===================================================================

XStatus SetUpInterruptSystem(ipi_hander_wraper_t* handler_wrp)
{
#ifndef MICROBLAZE

	u32 IpiIntrId=IpiInst.Config.IntId;

	// Initialize the interrupt controller driver
	XScuGic_Config* IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);//* Config for interrupt controller * /
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	u32 Status = XScuGic_CfgInitialize(IPI_Cntrlr_ptr, IntcConfig, IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the interrupt controller interrupt handler to the
	 * hardware interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) XScuGic_InterruptHandler, IPI_Cntrlr_ptr);

	/*
	 * Connect a device driver handler that will be called when an
	 * interrupt for the device occurs, the device driver handler
	 * performs the specific interrupt processing for the device
	 */
	Status = XScuGic_Connect(IPI_Cntrlr_ptr, IpiIntrId, (Xil_InterruptHandler) IpiIntrHandler, (void*)handler_wrp);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Enable the interrupt for the device
	XScuGic_Enable(IPI_Cntrlr_ptr, IpiIntrId);

	// Enable interrupts
	Xil_ExceptionEnable();

	return XST_SUCCESS;

#else

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */

	int Status = XIntc_Initialize(IPI_Cntrlr_ptr, INTC_DEVICE_ID);
	if(Status != XST_SUCCESS){
		return Status;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built
	 * correctly.
	 */
	Status = XIntc_SelfTest(IPI_Cntrlr_ptr);
	if(Status != XST_SUCCESS){
		return Status;
	}

	/*
	 *  This is the Interrupt Number of the Device whose Interrupt Output is
	 *  connected to the Input of the Interrupt Controller
	 */
	u8 intc_intr_input=0U;//XPAR_GSMRXTX_DDCANDRX_RX_UCNT_UB_UBINTCONCATETC_AXI_INTC_0_GSMRXTX_DDCANDRX_RX_UCNT_UB_UBINTCONCATETC_IPICH10INT_RES_INTR;
	Status = XIntc_Connect(IPI_Cntrlr_ptr, intc_intr_input, (XInterruptHandler)IpiIntrHandler, (void*) handler_wrp);
	if(Status != XST_SUCCESS){
		return Status;
	}


	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts,
	 */
	//Status = XIntc_Start(XIntcInstancePtr, XIN_SIMULATION_MODE);
	Status = XIntc_Start(IPI_Cntrlr_ptr, XIN_REAL_MODE);
	if(Status != XST_SUCCESS){
		return Status;
	}

	/*
	 * Enable the interrupt for the device and then cause (simulate) an
	 * interrupt so the handlers will be called.
	 */
	XIntc_Enable(IPI_Cntrlr_ptr, intc_intr_input);

	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_InterruptHandler, IPI_Cntrlr_ptr);

	/*
	 * Enable exceptions.
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;

#endif

}

//===================================================================

void IpiIntrHandler(void* hander_wpr_ptr)
{
	// Holds the IPI status register value
	u32 IpiSrcMask = XIpiPsu_GetInterruptStatus(&IpiInst);

	ipi_msg_t MsgBuffer;

	ipi_hander_wraper_t* custom_ipi_hander=(ipi_hander_wraper_t*)hander_wpr_ptr;

	// Poll for each source
	for (u32 SrcIndex = 0U; SrcIndex < IpiInst.Config.TargetCount; SrcIndex++) {
		if (IpiSrcMask & IpiInst.Config.TargetList[SrcIndex].Mask) {
			XIpiPsu_ReadMessage(&IpiInst, IpiInst.Config.TargetList[SrcIndex].Mask, (void*)&MsgBuffer,  IPI_MSG_SIZE, XIPIPSU_BUF_TYPE_MSG);

			if(MsgBuffer.target==XID && MsgBuffer.sender<XTARGET_COUNT){
				custom_ipi_hander->IpiHandler(&MsgBuffer);
				XIpiPsu_ClearInterruptStatus(&IpiInst, IpiInst.Config.TargetList[SrcIndex].Mask);
			}
		}
	}
}

//===================================================================

void Send_ipi_msg(u32 target, ipi_msg_t* ipi_msg_buff)
{
	XIpiPsu_WriteMessage(&IpiInst, target, (void*)ipi_msg_buff, IPI_MSG_SIZE, XIPIPSU_BUF_TYPE_MSG);
	XIpiPsu_TriggerIpi(&IpiInst, target);
}

//===================================================================

void Send_ipi_resp(u32 target, ipi_msg_t* ipi_msg_buff)
{
	XIpiPsu_WriteMessage(&IpiInst, target, (void*)ipi_msg_buff, IPI_MSG_SIZE, XIPIPSU_BUF_TYPE_RESP);
	XIpiPsu_TriggerIpi(&IpiInst, target);
}

//===================================================================

