/*************************************************************************//**
 * @file    dev_GUI_State_SendDevice.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine implementation for CONFIGURATION form.
 * @date    20.07.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        	REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <RTL.h>
#include "dev_GUI_hsm.h"		// includes drv_IR.h
#include "form_schemes.h"
#include "DialogBoxEx.h"
#include <string.h>

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
static void	 	OnFormDisplay	(void * pThis);
static uint32_t OnFormCallback	(void * pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     GUI_State_SendDevice
 * @brief   State in which reading status is showed in real time
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_SendDevice(Object* pThis, Event const * e){
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
    GUI_LoadScreen(OnFormCreate, OnFormDisplay, OnFormCallback);
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
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnGolMsg
 * @brief   Handles EV_SEND events
 *
 * @param   This      	this object
 * @param	m			T_GUI_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
/* Action performed by the Parent state GUI_State_Top
static State OnGolMsg(T_DEV_GUI * me, T_GUI_Message const * m){
	(void)me; (void)m; //not used
	// Action performed by the Parent state _State_Top
	// ignore events
	IGNORED();
}
*/

/*************************************************************************//**
 * @fun     OnTranMsg
 * @brief   Handles EV_TRANSITION events
 *
 * @param   This      	this object
 * @param	m			T_GUI_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
/* Action performed by the Parent state GUI_State_Top
static State OnTranMsg(T_DEV_GUI * me, T_GUI_Message const * m){
	(void)me; (void)m; //not used
	// Action performed by the Parent state _State_Top
	// ignore events
	IGNORED();
}
*/

/*************************************************************************//**
 * @fun     OnTimeout
 * @brief   Handles TIEMOUT_SIG event. Timeouts only can be due to event flag
 *			wait timeouts. In those cases, action to perform is resending the
 *			pdu data previously received from the Send/Read interface. Offspring
 *			states as WaitingAck and WaitingData will delegate timeout processing
 *			to this parent state. This keeps the StateOriented encapsulation.
 *
 * @param   This      	this object
 * @param	m			T_GUI_Message in process <me->msg>
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
/* Action performed by the Parent state GUI_State_Top
static State OnTimeout(T_DEV_GUI * me, T_GUI_Message const * m){
	(void)me; (void)m; //not used
	// Action performed by the Parent state _State_Top
	// ignore events
	IGNORED();
}
*/

/****************************************************************************************************//**
 *                        GRAPHICS HANDLERS 
 ********************************************************************************************************/
#define MENU_OBJ_WINDOW_1   	ID_WINDOW1
#define MENU_OBJ_BUTTON_1   	ID_OBJ_BUTTON_1
#define MENU_OBJ_BUTTON_2   	2
#define MENU_OBJ_BUTTON_3   	3
#define MENU_OBJ_BUTTON_4   	4
#define ID_LABEL				5
#define ID_PROGRESS_BAR			6

#define MAX_PROGS				70

typedef enum{
	PDU_wZONA = 0x00,
	PDU_wPROGRAMA,
	PDU_wAJUSTES,
	PDU_wRELOJ,
	PDU_wESTACIONES  
}E_PDU_WriteTypes;

const XCHAR * const Title_SendDevices_txt[] = {
	{"VOLCANDO"},
	{"SENDING"}
};

const XCHAR * const BodyBox_txt[5][3] = {
	{"Zona",	  "Zone",		"Zona"},
	{"Ajustes",	  "Settings",	"Impostazioni"},
	{"Reloj",	  "Clock",		"Orologio"},
	{"Estaciones","Seasons",	"Stagione"},
	{"Programa ", "Program ",	"Programma "}
};
const XCHAR * const Title_Repetir_txt[] = {
	{"¡Enviado Ok!"},
	{"Sent... Ok!"},
	{"Inviato... Ok!"}
};
const XCHAR * const Body_Repetir_txt[] = {
	{"¿Otro mas?"},
	{"Other one?"},
	{"Altro?"}
};

static XCHAR 		spdu[16];
static STATICTEXT  *lpdu;
static PROGRESSBAR *lprogress;

static uint8_t pduList[] = {PDU_wZONA,PDU_wAJUSTES,PDU_wRELOJ,PDU_wESTACIONES,PDU_VACACIONES,PDU_wPROGRAMA};
static uint8_t pduSize[] = {       13,           5,        10, 	 	       18, 	 	      18,          22};
static uint8_t pduIndex = 0;
static uint8_t progIndex = 0;
static uint8_t exitStatus = 0;

extern T_PDU_Full commPdu;

static void StartWrite(void *pThis);
static void OnDiscardRedo(void *pThis);
static void OnAcceptRedo(void *pThis);

/*************************************************************************//**
 * @fun     OnFormCreate
 * @brief   Creates the user form and all the widgets contained on it. Even
 *			it initialize all graphics variables.
 ****************************************************************************/
static void OnFormCreate(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	// not used: WINDOW* pOBJ_WINDOW_1;
	GOLFree();

	WndCreate(MENU_OBJ_WINDOW_1,       		// ID
              0,0,GetMaxX(),GetMaxY(), 	// dimension
              WND_DRAW |WND_TITLECENTER,                	// will be dislayed after creation
              NULL, 
              (XCHAR *)Title_SendDevices_txt[me->language],	   				// set text 
              WindowsScheme);               // default GOL scheme 

	pduIndex = 0;
	progIndex = 0;
	exitStatus = 0;
	me->isReceiving = 0;
	me->rwPdu = &commPdu;
  	strcpy(spdu, BodyBox_txt[pduIndex][me->language]);
	lpdu = StCreate(ID_LABEL, 4, 20, GetMaxX()-4, 36, ST_CENTER_ALIGN | ST_UPDATE | ST_DRAW, &spdu[0], altScheme); 
	lprogress = PbCreate(ID_PROGRESS_BAR, 10, 40,GetMaxX()-10, 52, PB_DRAW, 0, MAX_PROGS+3, altScheme);
	
	// Start trasnmission
	me->isDataReady = 1;
}

/*************************************************************************//**
 * @fun     OnFormDisplay
 * @brief   Displays all the information required in this state
 ****************************************************************************/
static void OnFormDisplay(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	SetState(lpdu, ST_DRAW);
	PbDraw(lprogress);

	if(exitStatus){
		IBeepLong();
		ICancelRemote();
		NotifyTransition(&GUI_State_Devices);
	}

	else if(me->isDataReady && ICheckRemoteReady()){
		if(progIndex >= MAX_PROGS){
			IBeepLong();
			DialogBoxEx_create(pThis,0, (XCHAR*)Title_Repetir_txt[me->language],(XCHAR*)Body_Repetir_txt[me->language], OnDiscardRedo, OnAcceptRedo, (char*)NO_txt[me->language], (char*)SI_txt[me->language],NULL);
		}
		else{
			IBeepShort();
			me->statCounter = 0;
			StartWrite(me);
		}
	}	
}


static void StartWrite(void *pThis){	
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
  	strcpy(spdu, BodyBox_txt[pduIndex][me->language]);
	PbSetPos(lprogress, pduIndex+progIndex);
	me->auxCounter = 0;
	if(pduList[pduIndex]!= PDU_wPROGRAMA){
		ISendRemote(&me->rwPdu->Field[pduIndex][0],pduSize[pduIndex]);
	}
	else{
		char p[3];
		p[0] = (char)(0x30+(progIndex/10));
		p[1] = (char)(0x30+(progIndex%10));
		p[2] = 0;
		strcat(spdu, &p[0]);
		ISendRemote((uint8_t*)(me->rwPdu->progs + (progIndex * 22)),pduSize[pduIndex]);
	}
	me->isDataReady = 0;
}

static void OnDiscardRedo(void *pThis){
	exitStatus = 1;
}

static void OnAcceptRedo(void *pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	pduIndex = 0;
	progIndex = 0;
	exitStatus = 0;
	me->isReceiving = 0;
	me->rwPdu = &commPdu;
	me->statCounter = 0;
  	strcpy(spdu, BodyBox_txt[pduIndex][me->language]);
	// Start trasnmission
	me->isDataReady = 1;
}



/*************************************************************************//**
 * @fun     OnFormCallback
 * @brief   Executes all the pending operations according with the reception
 *			of the GOL_MSG preprocessed by this state.
 *
 * @param   objMsg     	this object
 * @param	pObj		object who receives the message
  * @param	pMsg		pointer to the message to be processed
*
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static uint32_t OnFormCallback(void *pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg){
	// not used:  LISTBOX* pLb;
  	// not used:  LISTITEM* l_item;
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	me->auxCounter = 0;
  	
  	switch(GetObjID(pObj)){
   		case MENU_OBJ_WINDOW_1:
			if (pMsg->type == TYPE_KEYBOARD){				
				switch (pMsg->param1){
				 	case KEY_DOWN:
					break;
					case KEY_UP:
					break;
					case KEY_ENTER:
					break;
					case KEY_CANCEL:
						IBeepLong();
						ICancelRemote();
						TRAN(&GUI_State_Devices);
				}	 
			}	
		break;
	}        
	return 1;
}

void GUI_NewData_Writing_Hook(T_DEV_GUI * me, uint8_t *pdata, uint16_t size){
	if(pduList[pduIndex]!= PDU_wPROGRAMA){
		pduIndex++;
	}
	else{
		progIndex++;
	}
	me->isDataReady = 1;
}


