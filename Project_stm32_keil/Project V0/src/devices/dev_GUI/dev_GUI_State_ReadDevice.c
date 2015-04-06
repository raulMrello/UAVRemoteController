/*************************************************************************//**
 * @file    dev_GUI_State_ReadDevice.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine implementation for CONFIGURATION form.
 * @date    20.07.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        	REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "dev_GUI_hsm.h"		// includes drv_IR.h

/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/

/*		Event handling routines		*/
static State 	OnEntry			(T_DEV_GUI * me, T_GUI_Message const * m);
static State 	OnExit			(T_DEV_GUI * me, T_GUI_Message const * m);
/* Action performed by the Parent state GUI_State_Top
static State 	OnGolMsg  		(T_DEV_GUI * me, T_GUI_Message const * m);
static State 	OnTranMsg  		(T_DEV_GUI * me, T_GUI_Message const * m);
static State 	OnTimeout		(T_DEV_GUI * me, T_GUI_Message const * m);
*/

/*		Graphics Message handling routines		*/
static void  	OnFormCreate	(void * pThis);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     GUI_State_ReadDevice
 * @brief   State in which reading status is showed in real time
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_ReadDevice(Object* pThis, Event const * e){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	switch(e->sig){
		case ENTRY_SIG:
			return(OnEntry(me,(T_GUI_Message const *)e));

		case EXIT_SIG:
			return(OnExit(me,(T_GUI_Message const *)e));

		case EV_GOLMSG:	// delegate to parent implementation
			SUPER(&GUI_State_Top);

		case EV_TRANSITION:	// delegate to parent implementation
			SUPER(&GUI_State_Top);
					
		case TIMEOUT_SIG:	// delegate to parent implementation
			SUPER(&GUI_State_Top);
	}
    SUPER(&StateMachine_Top);
}


/****************************************************************************************************//**
 *                        EVENT HANDLERS 
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     OnEntry
 * @brief   Handles ENTRY_SIG events
 *
 * @param   This      	this object
 * @param	m			T_GUI_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnEntry(T_DEV_GUI * me, T_GUI_Message const * m){
	(void)m;	// not used
    GUI_LoadScreen(OnFormCreate, ComMenu_Display, ComMenu_Callback);
	me->timeout = GUI_MBX_TIMEOUT; 	// a safety time till a msg is received
	// paint changes
	while(!GOLDraw());
	IDrawMem();
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnExit
 * @brief   Handles EXIT_SIG events
 *
 * @param   This      	this object
 * @param	m			T_GUI_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnExit(T_DEV_GUI * me, T_GUI_Message const * m){
	(void)me; (void)m;	// not used
	ICancelRemote();
	IBacklightPermit();
	IBacklightOn();
	HANDLED();
}

/****************************************************************************************************//**
 *                        GRAPHICS HANDLERS 
 ********************************************************************************************************/

/* pdu types to be read */
typedef enum{
	PDU_ZONA = 0x80,
	PDU_PROGRAMA,
	PDU_AJUSTES,
	PDU_RELOJ,
	PDU_ESTACIONES,
	PDU_VACACIONES = 0x89  
}E_PDU_Types;

/* private data types */
static const uint8_t pduList[] = {PDU_ZONA,PDU_AJUSTES,PDU_RELOJ,PDU_ESTACIONES,PDU_VACACIONES,PDU_PROGRAMA};
static const uint8_t pduSize[] = {      13,          5,       10, 		   18, 		      18,          22};
static uint8_t pduIndex = 0;

static uint8_t 	StartRead	(void * pThis, char ** pText, uint8_t *pReq, uint8_t *pSzReq, uint8_t **pData, uint8_t *pSzData);

static void OnSaveDevice(void *pThis);
static void OnSendDevice(void *pThis);

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     OnFormCreate
 * @brief   Creates the ComMenu form and assigns callback pointers.
 *
 * @param   pThis      	this object as void
 *
 * @return  n/a
 ****************************************************************************/
static void OnFormCreate(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;

	pduIndex = 0;
	me->rwPdu = &commPdu;
	me->rwPdu->progs = &progs[0][0];
	memset(&progs[0][0], 0, (MAX_PROGS*22));
	me->discardSeasonFlags = FALSE;
	ComMenu_Create(		pThis, 
						(XCHAR *)TEXTS[TIT_LEER_EQUIPO][me->language], 
						StartRead, 
						MAX_FIELDS+MAX_PROGS,
						1,
						(XCHAR *)TEXTS[TXT_Leido_Ok][me->language], 
						(XCHAR *)TEXTS[TXT_Opciones_][me->language], 
						(XCHAR *)TEXTS[TXT_Salvar][me->language], 
						(XCHAR *)TEXTS[TXT_Volcar][me->language], 
						OnSaveDevice,
						OnSendDevice,
						&GUI_State_Devices);
}

/*************************************************************************//**
 * @fun     StartRead
 * @brief   Starts a new read request till all programs will be transferred..
 *
 * @param   pThis      	this object as void
 * @param   pText      	text associated with the stream requested
 * @param   pReq	  	destination buffer to store pdu_request
 * @param   pSzReq		size of the pdu_request to send
 * @param   pData      	destination buffer to the read data
 * @param   pSzData    	size of the expected data read
 *
 * @retval  0			new transmission must start
 * @retval  -1			no more transmission required
 ****************************************************************************/
static uint8_t StartRead(void * pThis, char ** pText, uint8_t *pReq, uint8_t *pSzReq, uint8_t **pData, uint8_t *pSzData){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	if(pduList[pduIndex]!= PDU_PROGRAMA){
		*pText = (char*)TEXTS[TXT_Zona+pduIndex][me->language];
		*pReq 	= pduList[pduIndex];
		*pSzReq = 1;
		*pData = &me->rwPdu->Field[pduIndex][0];
		*pSzData = pduSize[pduIndex];
		pduIndex++;
		return 0;
	}
	*pText = (char*)TEXTS[TXT_Programas___][me->language];
	*pReq 	  = pduList[pduIndex];
	//*(pReq+1) is filled by ComMenu: First = 255, then each program number previously received
	*pSzReq = 2;
	*pData = (uint8_t*)(me->rwPdu->progs);
	*pSzData = pduSize[pduIndex];
	return(0);
}

/*************************************************************************//**
 * @fun     OnSaveDevice
 * @brief   Callback invoked by ComMenu form to save received data
 *
 * @param   pThis      	this object as void
 *
 ****************************************************************************/
static void OnSaveDevice(void *pThis){
	NotifyTransition(GUI_State_SaveDevice);
}

/*************************************************************************//**
 * @fun     OnSendDevice
 * @brief   Callback invoked by ComMenu form to send received data
 *
 * @param   pThis      	this object as void
 *
 ****************************************************************************/
static void OnSendDevice(void *pThis){
	NotifyTransition(GUI_State_SendDevice);
}


