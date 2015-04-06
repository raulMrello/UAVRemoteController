/*************************************************************************//**
 * @file    dev_GUI_State_SendLocation.h
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
 * @fun     GUI_State_SendLocation
 * @brief   State in which reading status is showed in real time
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_SendLocation(Object* pThis, Event const * e){
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

/* pdu types to be send */
typedef enum{
	PDU_wZONA = 0x00,
	PDU_wPROGRAMA,
	PDU_wAJUSTES,
	PDU_wRELOJ,
	PDU_wESTACIONES  
}E_PDU_WriteTypes;


static const uint8_t pduList[] = {PDU_wZONA,PDU_wAJUSTES,PDU_wRELOJ,PDU_wESTACIONES,PDU_wPROGRAMA};
static const uint8_t pduSize[] = {       13,           5,        10, 	 	       18,           22};
static uint8_t pduIndex = 0;


static uint8_t StartWrite(void * pThis, char ** pText, uint8_t *pReq, uint8_t *pSzReq, uint8_t **pData, uint8_t *pSzData);
static void OnDiscardRedo(void *pThis);
static void OnAcceptRedo(void *pThis);

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
	// override season_type flag
	me->discardSeasonFlags = TRUE;
	ComMenu_Create(		pThis, 
						(XCHAR *)TEXTS[TIT_VOLCANDO][me->language], 
						StartWrite, 
						2,
						0,
						(XCHAR *)TEXTS[TXT_Enviado_Ok][me->language], 
						(XCHAR *)TEXTS[TXT_Otro_mas_][me->language], 
						(XCHAR *)TEXTS[TXT_NO][me->language], 
						(XCHAR *)TEXTS[TXT_SI][me->language], 
						OnDiscardRedo,
						OnAcceptRedo,
						&GUI_State_Location);
}

/*************************************************************************//**
 * @fun     StartWrite
 * @brief   Starts a new write operation till all programs will be transferred..
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
static uint8_t StartWrite(void * pThis, char ** pText, uint8_t *pReq, uint8_t *pSzReq, uint8_t **pData, uint8_t *pSzData){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	switch(pduList[pduIndex]){
		case PDU_wZONA:
		case PDU_wRELOJ:
		*pText = (char*)TEXTS[TXT_Zona+pduIndex][me->language];
		*pData = &me->rwPdu->Field[pduIndex][0];
		*pSzData = pduSize[pduIndex];
		pduIndex += 2;
		return 0;
		default:
			break;	
	}
	return((uint8_t)-1);
}

/*************************************************************************//**
 * @fun     OnDiscardRedo
 * @brief   Callback invoked by ComMenu form to discard data to resend
 *
 * @param   pThis      	this object as void
 *
 ****************************************************************************/
static void OnDiscardRedo(void *pThis){
	NotifyTransition(GUI_State_Location);
}

/*************************************************************************//**
 * @fun     OnAcceptRedo
 * @brief   Callback invoked by ComMenu form to resend data
 *
 * @param   pThis      	this object as void
 *
 ****************************************************************************/
static void OnAcceptRedo(void *pThis){
	NotifyTransition(GUI_State_SendLocation);
}



