/*************************************************************************//**
 * @file    dev_IR.h
 * @mcu		STM32F1x
 * @brief   IR Device Manager
 * @date    11.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        	REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <string.h>
#include "dev_IR_hsm.h"		// includes dev_IR.h

 
/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/

static T_DEV_IR This;
static uint8_t	rxBuff[IR_MAX_MAC_LENGTH];		//!< reception buffer with decoded data
static uint8_t	txBuff[IR_MAX_PHY_LENGTH];		//!< transmission buffer to code data into


static void IR_Task	(Object *pThis);
#define IR_STACK_SIZE			400	// tamaño en bytes
static volatile unsigned long long 	IR_Stack[(IR_STACK_SIZE/8)];



/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		dev_IR_Init
 * @brief	Provided interface used by the initial system loading to carry out IR Device initialization
 * @return	
 **********************************************************************************/
uint8_t dev_IR_Init(uint8_t priority){
	ErrorCode err = ERR_OK;

	IDriverInit(19200);
	// configuracion de pines
	IDriverEnable(); //drv_GPIO_IR_EN_as_output();
	#if HARDWARE_RELEASE != HW_REV2	
	IDriverTurnOff(); //drv_GPIO_IR_EN_On();
	#endif

	// setup Rx/Tx buffers for IR communications
	This.rxBuff = &rxBuff[0];
	This.txBuff = &txBuff[0];

	This.recvTimeout = 0;

	// Initialize IRunnable interface. If succesful, an OS_TID is assigned.
	err = IRunnable_Init   (&This.runnable, 
							&This, 
							&This.runnable.tid, 
							sizeof(T_IR_Message), 
							4, 
							(void*)&IR_Stack, 
							(U16)IR_STACK_SIZE,
							priority,
							&IR_Task,
							&IR_State_Initial);
	return((uint8_t)err);
}


/****************************************************************************************************//**
 * @fun		dev_IR_Send
 * @brief	Provided interface to send data through the IR interface
 *
 * @param	PDU		pointer to the data buffer to send
 * @param	len		length of the data buffer
 *
 * @retval	0				succesfull
 * @retval	ERR_ARGS		argument checking errors
 * @retval	ERR_MEM			memory allocation errors
 * @retval	ERR_MBOX_FULL	mbx is full
 **********************************************************************************/
uint8_t dev_IR_Send(uint8_t *PDU, uint8_t len){
	T_IR_Message *msg;

	// arg checking
	if(PDU == 0 || len > (IR_MAX_PDU_LENGTH))
		return((uint8_t)ERR_ARGS);
	
	// message allocation
	msg = _alloc_box(This.runnable.inbox.memPool);
	if(msg == 0)
		return ((uint8_t)ERR_MEM);
	
	// message building	
	msg->super.sig = EV_SEND;
	msg->msgContent.SendMsg.len = len;
	memcpy(&msg->msgContent.SendMsg.pdu[0], PDU, len);

	// mailbox checking
	if(os_mbx_check(This.runnable.inbox.mailBox) == 0)
		return((uint8_t)ERR_MBOX_FULL);

	// message sending. Waits forever	
	os_mbx_send(This.runnable.inbox.mailBox, msg, 0xffff);
	return((uint8_t)ERR_OK);
}


/****************************************************************************************************//**
 * @fun		dev_IR_Read
 * @brief	Provided interface to be used by the GUI Manager to start read operations from remote devices
 *
 * @param	pduType			type of pdu to be read
 * @param	pduArgs			pointer to the pdu arguments attached to the Read request
 * @param	szArgs			max number of bytes in the argument buffer
 *
 * @retval	0				succesfull
 * @retval	ERR_ARGS		argument checking errors
 * @retval	ERR_MEM			memory allocation errors
 * @retval	ERR_MBOX_FULL	mbx is full
 **********************************************************************************/
uint8_t dev_IR_Read(uint8_t *pdu, uint8_t len){
	T_IR_Message *msg;

	// arg checking
	if(pdu == 0 || len > IR_MAX_PDU_LENGTH)
		return((uint8_t)ERR_ARGS);

	if(!IsPduTypeValid(*pdu))
		return((uint8_t)ERR_ARGS);
	
	// message allocation
	msg = _alloc_box(This.runnable.inbox.memPool);
	if(msg == 0)
		return ((uint8_t)ERR_MEM);
	
	// message building
	msg->super.sig = EV_READ;
	msg->msgContent.ReadMsg.len = len;
	memcpy(&msg->msgContent.ReadMsg.pdu[0], pdu, len);

	// mailbox checking
	if(os_mbx_check(This.runnable.inbox.mailBox) == 0)
		return((uint8_t)ERR_MBOX_FULL);

	// message sending. Waits forever	
	os_mbx_send(This.runnable.inbox.mailBox, msg, 0xffff);
	return((uint8_t)ERR_OK);
}


/****************************************************************************************************//**
 * @fun		dev_IR_Cancel
 * @brief	Provided interface to be used by the GUI Manager to cancel inprogress operations
 *
 * @retval	0				succesfull
 * @retval	ERR_ARGS		argument checking errors
 * @retval	ERR_MEM			memory allocation errors
 * @retval	ERR_MBOX_FULL	mbx is full
 **********************************************************************************/
uint8_t dev_IR_Cancel(void){
	// if not waiting events, then discard it
	if(This.waitfor != WAITFOR_EVT){
		return((uint8_t)ERR_OK);
	}
	os_evt_set(EVF_CANCELLED, This.runnable.tid);
	return((uint8_t)ERR_OK);
}


/****************************************************************************************************//**
 * @fun		dev_IR_Power
 * @brief	Provided interface to be used by the GUI Manager to power IR submodule
 *
 * @retval	0				succesfull
 * @retval	ERR_ARGS		argument checking errors
 * @retval	ERR_MEM			memory allocation errors
 * @retval	ERR_MBOX_FULL	mbx is full
 **********************************************************************************/
uint8_t dev_IR_Power(void){
	IDriverTurnOn();
	return((uint8_t)ERR_OK);
}

/****************************************************************************************************//**
 * @fun		dev_IR_Unpower
 * @brief	Provided interface to be used by the GUI Manager to unpower IR submodule
 *
 * @retval	0				succesfull
 * @retval	ERR_ARGS		argument checking errors
 * @retval	ERR_MEM			memory allocation errors
 * @retval	ERR_MBOX_FULL	mbx is full
 **********************************************************************************/
uint8_t dev_IR_Unpower(void){
	IDriverTurnOff();
	return((uint8_t)ERR_OK);
}


/****************************************************************************************************//**
 * @fun		dev_IR_IsWaitingReq
 * @brief	Provided interface to be used by the GUI Manager to know if it is waiting new action request.
 *
 * @retval	0				busy
 * @retval	1				ready for new request
 **********************************************************************************/
uint8_t dev_IR_IsWaitingReq (void){
	// if not waiting events, then discard it
	if(This.waitfor == WAITFOR_MBX){
		return(1);
	}
	return 0;
}


/****************************************************************************************************//**
 * @fun		dev_IR_NewRecvData
 * @brief	Provided interface to be used by the driver ISR to notify data received via IR.
 * @return	
 **********************************************************************************/
void dev_IR_NewRecvData(void){
	// if not waiting events, then discard it
	if(This.waitfor != WAITFOR_EVT){
		return;
	}
	isr_evt_set (EVF_DATARECV, This.runnable.tid);
}


/****************************************************************************************************//**
 * @fun		dev_IR_RecvTimeoutOverflow
 * @brief	Provided interface to notify recv timeout overflow from application code (MUST USE os_evt_set).
 * @return	
 **********************************************************************************/
void dev_IR_RecvTimeoutOverflow(void){
	// if not waiting events, then discard it
	if(This.waitfor != WAITFOR_EVT){
		return;
	}
	isr_evt_set (EVF_RECVTOUTOVF, This.runnable.tid);
	This.recvTimeout = 0;
	/*
	if(This.ntx > 0){
		IDriverDisableRx();				// disable incomming unexpected data
		IDriverInit(0);					// initialize ONLY rx buffer
		InitReceptionProcess(&This);		// initialize decoder to discard bad data
		ISend(This.txBuff, This.ntx);		// resend request
	}
	*/
}


/****************************************************************************************************//**
 * @fun		dev_IR_NewSentData
 * @brief	Provided interface to be used by the driver ISR to notify data sent via IR.
 * @return	
 **********************************************************************************/
void dev_IR_NewSentData(void){
	// if not waiting events, then discard it
	if(This.waitfor != WAITFOR_EVT){
		return;
	}
	isr_evt_set (EVF_DATASENT, This.runnable.tid);
}



/****************************************************************************************************//**
 * @fun		IR_Task
 * @brief	Task for this device. Launches the initial state of the HSM and afterwards waits for 
 *			incoming messages (os_mbx_wait) or eventflags (os_evt_wait_or) depending on the property
 *			<waitfor>. In both cases, is timeout is overrun then a timeout event is dispatched to the
 *			HSM. In other case, the proper event is dispatched.
 * @return	
 **********************************************************************************/
static void IR_Task(Object *pThis){
	T_DEV_IR * me = (T_DEV_IR*)pThis;
	T_IR_Message * msg;
    // Starts the state machine
    StateMachine_Init(me);       

	for(;;){
		switch(me->waitfor){
			// wait for mbx message
			case WAITFOR_MBX:{
				OS_RESULT errno;
				errno = os_mbx_wait(me->runnable.inbox.mailBox, (void**)&msg, me->timeout);
				// dispatch message to the state machine
				// if msg is valid : !NULL and !timeout event
				// dispatch message to the state machine and then free box preallocation
				if(msg != 0 && errno != OS_R_TMO){
					StateMachine_Dispatch(me, (Event const *)msg);
					_free_box(me->runnable.inbox.memPool, msg);
				}		
				break;
			}
			// wait for event flag
			case WAITFOR_EVT:{
				OS_RESULT errno;
				uint16_t recvflag;
				const Event dataReceivedEvt = {EV_DATARECV};
				const Event dataSentEvt 	= {EV_DATASENT};
				const Event cancelledEvt 	= {EV_CANCELLED};

				errno = os_evt_wait_or(EVF_IR_ALLFLASG, me->timeout);
				// if timeout, launch timeout event
				if(errno == OS_R_TMO){
					StateMachine_Dispatch(me, (Event const *)&timeoutEvt);
				}
				// if EVF_DATARECV or EVF_CANCELLED flag, check and dispatch
				else{
					recvflag = os_evt_get();			// get event received
					if((recvflag & EVF_RECVTOUTOVF) != 0){	// process cancel request with high priority
						os_evt_clr(EVF_RECVTOUTOVF, me->runnable.tid);
						StateMachine_Dispatch(me, (Event const *)&timeoutEvt);
					}
					if((recvflag & EVF_CANCELLED) != 0){	// process cancel request with high priority
						os_evt_clr(EVF_CANCELLED, me->runnable.tid);
						StateMachine_Dispatch(me, (Event const *)&cancelledEvt);
					}
					if((recvflag & EVF_DATASENT) != 0){	// process data sent notification
						os_evt_clr(EVF_DATASENT, me->runnable.tid);
						StateMachine_Dispatch(me, (Event const *)&dataSentEvt);
					}
					if((recvflag & EVF_DATARECV) != 0){	// process data received notification
						os_evt_clr(EVF_DATARECV, me->runnable.tid);
						StateMachine_Dispatch(me, (Event const *)&dataReceivedEvt);
					}
				}
				break;
			}
		}
	}
}

/*************************************************************************//**
 * @fun     ByteToManchester
 * @brief   Code a byte into a coded Manchester dual byte
 *
 * @param   data   		byte to be coded
 * @param   msb			msb coded
 * @param   lsb			lsb coded
 ****************************************************************************/
static void ByteToManchester(uint8_t data, uint8_t * msb, uint8_t * lsb){
	uint8_t auxData;
	uint8_t auxByte;

	// process MSB
	auxData = data;
    auxByte = (uint8_t)(auxData & 0x80);  //b7
    auxData = (uint8_t)(auxData >> 1);
    auxByte += (uint8_t)(auxData & 0x60); //b7 b7 b6
    auxData = (uint8_t)(auxData >> 1);
    auxByte += (uint8_t)(auxData & 0x18); //b7 b7 b6 b6 b5
    auxData = (uint8_t)(auxData >> 1);
    auxByte += (uint8_t)(auxData & 0x06); //b7 b7 b6 b6 b5 b5 b4
    auxData = (uint8_t)(auxData >> 1);
    auxByte += (uint8_t)(auxData & 0x01); //b7 b7 b6 b6 b5 b5 b4 b4
    auxByte ^= IR_MANCHESTER_FLAG;
	*msb = auxByte;

	// process LSB
    auxData = (uint8_t)(data << 4);
    auxByte = (uint8_t)(auxData & 0x80);  //b3
    auxData = (uint8_t)(auxData >> 1);
    auxByte += (uint8_t)(auxData & 0x60); //b3 b3 b2
    auxData = (uint8_t)(auxData >> 1);
    auxByte += (uint8_t)(auxData & 0x18); //b3 b3 b2 b2 b1
    auxData = (uint8_t)(auxData >> 1);
    auxByte += (uint8_t)(auxData & 0x06); //b3 b3 b2 b2 b1 b1 b0
    auxData = (uint8_t)(auxData >> 1);
    auxByte += (uint8_t)(auxData & 0x01); //b3 b3 b2 b2 b1 b1 b0 b0
    auxByte ^= IR_MANCHESTER_FLAG;
	*lsb = auxByte;
}


/*************************************************************************//**
 * @fun     DecodeManchester
 * @brief   executes Manchester decoder. When status == 1 must notify the
 *			end of decodification and this function will return ERR_OK
 *
 * @param   This        Object T_IR_Manchester
 * @param   data        data to be decoded
 *
 * @retval  ERR_IR_OK      dual byte decoded into single byte 
 * @retval  ERR_IR_PENDING decoding in process 
 ****************************************************************************/
E_IR_ERRORS DecodeManchester(T_IR_Manchester *This, uint8_t data){
    uint8_t auxData = data;
    switch(This->status){
        case 0:
            This->result = 0;
            This->msbByte = data;            
            This->status++;
            auxData = (uint8_t)(auxData << 1);
            This->result += (uint8_t)(auxData & 0x80);
            auxData = (uint8_t)(auxData << 1);
            This->result += (uint8_t)(auxData & 0x40);
            auxData = (uint8_t)(auxData << 1);
            This->result += (uint8_t)(auxData & 0x20);
            auxData = (uint8_t)(auxData << 1);
            This->result += (uint8_t)(auxData & 0x10);
            return(ERR_IR_PENDING);
        case 1:
            This->lsbByte = data;
            This->status = 0;
            This->result += (uint8_t)(auxData & 0x01);
            auxData = (uint8_t)(auxData >> 1);
            This->result += (uint8_t)(auxData & 0x02);
            auxData = (uint8_t)(auxData >> 1);
            This->result += (uint8_t)(auxData & 0x04);
            auxData = (uint8_t)(auxData >> 1);
            This->result += (uint8_t)(auxData & 0x08);
            This->result ^= 0xFF;            
            return(ERR_IR_OK);
        default:
            This->status = 0;
            This->result = 0;
            This->lsbByte = 0;
            This->msbByte = 0;
            return(ERR_IR_PENDING);
    }
}

/*************************************************************************//**
 * @fun     CalculateChecksum
 * @brief   Calculate checksum (CRC16).
 *
 * @param   crc_init		accumulated crc16 calculated previously
 * @param   buffer			data location to be processed
 * @param	len				number of bytes to be processed
 ****************************************************************************/
static uint16_t CalculateChecksum(uint16_t crc_init, uint8_t *buffer, uint8_t len){  
    uint8_t i;
	uint8_t data;
	uint16_t crc = crc_init;
		
	while(len--){
		data = *buffer++;
     	crc = (uint16_t)(crc ^ ((uint16_t)data << 8));
     	for (i=0; i<8; i++){
	       	if (crc & 0x8000)
	           	crc = (uint16_t)((crc << 1) ^ 0x1021);
	       	else
	           	crc <<= 1;
	    }
	}
    return crc;
}

/*************************************************************************//**
 * @fun     CodeToManchester
 * @brief   Code a PDU stream into a coded Manchester stream
 *
 * @param   rawStream   destination coded stream
 * @param   pdu			original PDU stream
 * @param   size		number of bytes to process
 *
 * @return	uint16_t	number of bytes in the buffer
 ****************************************************************************/
uint16_t CodeToManchester(uint8_t * rawStream, uint8_t * pdu, uint16_t size){
	uint16_t 	i;
	uint16_t	ntx = 0;
	uint16_t	crc16 = 0;
	uint8_t * 	pwrite;
	uint8_t * 	pread;
	uint8_t 	msb;
	uint8_t 	lsb;
	uint8_t		size8 = (uint8_t)size;

	// prepare pointer to write destination	and read origin
	pread = pdu;
	pwrite = rawStream;

	// write the preamble
	//for(i=0;i<IR_PREAMBLE_LENGTH;i++){
	//	*pwrite = IR_PREAMBLE_FLAG;
	//	pwrite++;
	//	ntx++;
	//}
	*pwrite++ = 0x55;
	*pwrite++ = 0x55;
	*pwrite++ = 0x55;
	*pwrite++ = 0x55;
	*pwrite++ = 0x44;
	*pwrite++ = 0x11;
	*pwrite++ = 0x45;
	*pwrite++ = 0x15;
	ntx += 8;

	// write the headflag
	ByteToManchester(IR_HEADFLAG, &msb, &lsb);
	*pwrite = msb;
	pwrite++;
	*pwrite = lsb;
	pwrite++;
	ntx+=2;

	// add <size> to the CRC calculation
	crc16 = CalculateChecksum(crc16, &size8, 1);
	// write the size
	ByteToManchester(size8, &msb, &lsb);
	*pwrite = msb;
	pwrite++;
	*pwrite = lsb;
	pwrite++;
	ntx+=2;
	
	// write the data with CRC calculation
	for(i=0;i<size;i++){
		// add pdu[i] to the CRC calculation
		crc16 = CalculateChecksum(crc16, pread, 1);
		// code pdu[i] to manchester
		ByteToManchester(*pread, &msb, &lsb);
		*pwrite = msb;
		pwrite++;
		*pwrite = lsb;
		pwrite++;
		pread++;
		ntx+=2;
	}

	// write the CRC calculated
	ByteToManchester((crc16 >> 8), &msb, &lsb);
	*pwrite = msb;
	pwrite++;
	*pwrite = lsb;
	pwrite++;
	ntx+=2;
	ByteToManchester((crc16 & 0xFF), &msb, &lsb);
	*pwrite = msb;
	pwrite++;
	*pwrite = lsb;
	pwrite++;
	ntx+=2;

	return(ntx);
}

/*************************************************************************//**
 * @fun     SaveToBuffer
 * @brief   Save a byte into the reception buffer and increase the count
 *
 * @param   me   		device reference
 * @param   data		data to insert into the buffer
 ****************************************************************************/
static void SaveToBuffer(T_DEV_IR *me, uint8_t data){
    me->rxBuff[me->nrx] = data;
    //check if crc must be updated
    if(me->smMode == modeRECEIVING){
      me->crc16 = CalculateChecksum(me->crc16, &data, 1u);
    }
	//increase count
    me->nrx++;  
}

/*************************************************************************//**
 * @fun     ProcessByteReceived
 * @brief   Process the byte received. Depending on the reception operating
 *			mode, it should be inserted into the reception buffer or not. And
 *			even if the data belongs to PDU payload.
 *
 * @param   me   		device reference
 * @param   data		data to insert into the buffer
 *
 * @return	E_IR_ERRORS	depending on the status. 
 *						If buffering then ERR_IR_PENDING
 *						If end of reception then ERR_IR_OK
 *						Else other kind of errors
 ****************************************************************************/
E_IR_ERRORS ProcessByteReceived(T_DEV_IR *me, uint8_t data){
    E_IR_ERRORS err = ERR_IR_OK;
	uint16_t checksum;
	static uint8_t errPreamble = 0;

    switch(me->smMode){
        case modeDISCONNECTED:
            if(data != IR_HEADFLAG_MSB){
				if(++errPreamble > IR_PREAMBLE_LENGTH_ERROR){
					errPreamble = 0;
					return ERR_IR_PREAMBLE;	
				}
                return ERR_IR_PENDING;
            }
            me->smMode = modeBINDING;
            me->nrx = 0;
            return(ERR_IR_PENDING);
            
        case modeBINDING:
			errPreamble = 0;
            if(data != IR_HEADFLAG_LSB){
                return ERR_IR_HEADFLAG;
            }
            me->nrx = 0;					// initialize buffer
            SaveToBuffer(me,IR_HEADFLAG);	// save data to buffer
            me->manchester.status = 0;		// reset manchester decoder
            me->smMode = modeACCEPTED;
            return(ERR_IR_PENDING);                
            
        case modeACCEPTED:
            err = DecodeManchester(&me->manchester, data);
            if(err == ERR_IR_PENDING){
                return(ERR_IR_PENDING);                                
            }
            if(me->manchester.result > IR_MAX_MAC_LENGTH){
                return ERR_IR_PDULENGTH;
            }
            me->smMode = modeRECEIVING;
            me->crc16 = 0u;					// initialize crc calculation
            SaveToBuffer(me,me->manchester.result);
            return(ERR_IR_PENDING);                
        
        case modeRECEIVING:
            err = DecodeManchester(&me->manchester, data);
            if(err == ERR_IR_PENDING){
                return(ERR_IR_PENDING);                                
            }
            if(me->nrx >= (IR_MAC_ADDR_PDUDATA + me->rxBuff[IR_MAC_ADDR_PDULENGTH])){
                me->smMode = modeENDING;            
            }
            SaveToBuffer(me,me->manchester.result);
            return(ERR_IR_PENDING);                
        
        case modeENDING:
            err = DecodeManchester(&me->manchester, data);
            if(err == ERR_IR_PENDING){
                return(ERR_IR_PENDING);                                
            }
            SaveToBuffer(me,me->manchester.result);

            // checksum evaluation: firstly MSB then LSB
            checksum = (uint16_t)(me->rxBuff[IR_MAC_ADDR_PDUDATA + me->rxBuff[IR_MAC_ADDR_PDULENGTH]] << 8);
            checksum += me->rxBuff[IR_MAC_ADDR_PDUDATA + me->rxBuff[IR_MAC_ADDR_PDULENGTH]+1];
            if(checksum == me->crc16){
                me->nrx = 0;
                return(ERR_IR_OK);            
            }
            return(ERR_IR_CHECKSUM);
            
        default:
            return ERR_IR_UNKNOWN;
    }
}

/*************************************************************************//**
 * @fun     InitReceptionProcess
 * @brief   Initialize the reception process to its startup state
 *
 * @param   me   		device reference
 ****************************************************************************/
void InitReceptionProcess(T_DEV_IR* me){
    me->nrx = 0;
    me->crc16 = 0u;
    me->smMode = modeDISCONNECTED;
    me->manchester.status = 0;
    me->manchester.msbByte = 0;
    me->manchester.lsbByte = 0;
}


/****************************************************************************************************//**
 * @fun		IsPduTypeValid
 * @brief	Checks if a pduType is wrong or valid
 * @param	pduType		
 * @return	1 if pdu type valid, else 0 if unknown
 **********************************************************************************/
uint8_t IsPduTypeValid(uint8_t pduType){
	switch(pduType){
		case IR_PDU_ZONE_WR:
		case IR_PDU_PROG_WR:
		case IR_PDU_SETT_WR:
		case IR_PDU_CLOCK_WR:
		case IR_PDU_SEAS_WR:
		case IR_PDU_VACS_WR:
		case IR_PDU_ZONE_RD:
		case IR_PDU_PROG_RD:
		case IR_PDU_SETT_RD:
		case IR_PDU_CLOCK_RD:
		case IR_PDU_SEAS_RD:
		case IR_PDU_VACS_RD:
			return 1;
		default:
			return 0;
	}
}


/****************************************************************************************************//**
 * @fun		IR_ResetRecvTimeout
 * @brief	Protected routine to feed the dog.
 * @return	
 **********************************************************************************/
void IR_ResetRecvTimeout(void){
  #if IR_RECV_TIMEOUT_ENABLE > 0

	if(This.recvTimeout != 0){
		os_tmr_kill(This.recvTimeout);
		This.recvTimeout = 0;
	}

  #endif
}		


/****************************************************************************************************//**
 * @fun		IR_StartRecvTimeout
 * @brief	Protected routine to start the dog.
 * @return	
 **********************************************************************************/
void IR_StartRecvTimeout(void){
  #if IR_RECV_TIMEOUT_ENABLE > 0
	if(This.recvTimeout == 0){
		This.recvTimeout = os_tmr_create(IR_RECV_TIMEOUT, IR_OS_TMR);
	}
	else{
		os_tmr_kill(This.recvTimeout);
		This.recvTimeout = os_tmr_create(IR_RECV_TIMEOUT, IR_OS_TMR);
	}

  #endif
}		


