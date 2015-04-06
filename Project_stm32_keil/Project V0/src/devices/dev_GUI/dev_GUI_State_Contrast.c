/*************************************************************************//**
 * @file    dev_GUI_State_Contrast.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine implementation for CONTRAST form.
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        	REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "dev_GUI_hsm.h"		
//#include "ProgressBar.h"

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
 * @fun     GUI_State_Contrast
 * @brief   Menu state must show a menu list with all the functions allowable
 *			by the device
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_Contrast(Object* pThis, Event const * e){
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
	// get the actual contrast
	me->blContrast = IBacklightGetContrast();
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
#define ID_LABEL_MIN			5
#define ID_LABEL_MAX			6
#define ID_PROGRESS_BAR			7

static PROGRESSBAR *lprogress;
static uint8_t mustExit = FALSE;
static uint8_t initialContrast;
static uint8_t contrastOnEsc = (uint8_t)-1;

static void OnDiscardChanges (void *pThis);
static void OnSaveChanges	 (void *pThis);

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
              NULL, //(void*)&OrbisMenuInvert,               	// icon
              (XCHAR *)TEXTS[TIT_CONTRASTE][me->language],	   				// set text 
              me->WindowsScheme);               // default GOL scheme 
   
	initialContrast = me->blContrast;
	// reloads contrast after DialogBox Escape
	if(contrastOnEsc != (uint8_t)-1){
		me->blContrast = contrastOnEsc;
	}
	lprogress = PbCreate(ID_PROGRESS_BAR, 20, 25,105, 37, PB_DRAW, 0, 99, me->altScheme);
	StCreate(ID_LABEL_MIN, 20, 40, 70, 52, ST_UPDATE | ST_DRAW, "MIN", me->altScheme); 
	StCreate(ID_LABEL_MAX, 90, 40, 120, 52, ST_UPDATE | ST_DRAW, "MAX", me->altScheme); 
	PbSetPos(lprogress, me->blContrast);
	mustExit = FALSE;
}

/*************************************************************************//**
 * @fun     OnFormDisplay
 * @brief   Displays all the information required in this state
 ****************************************************************************/
static void OnFormDisplay(void * pThis){
	if(mustExit){
		IBeepLong();
		NotifyTransition(&GUI_State_Configuration);
	}	
	PbDraw(lprogress);
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
  	switch(GetObjID(pObj)){
   		case MENU_OBJ_WINDOW_1:
			if (pMsg->type == TYPE_KEYBOARD){
			   	switch (pMsg->param1){
				 	case KEY_LEFT:
						if(me->blContrast > 0){
							me->blContrast -= 25;
							if(me->blContrast >= 100){
								me->blContrast = 0;
							}
							PbSetPos(lprogress, me->blContrast);
							IBacklightSetContrast(me->blContrast);
						}
						break;
					case KEY_RIGHT:
						if(me->blContrast < 100){
							me->blContrast += 25;
							if(me->blContrast > 100){
								me->blContrast = 100;
							}
							PbSetPos(lprogress, me->blContrast);
							IBacklightSetContrast(me->blContrast);
						}
						break;
					case KEY_ENTER:
						contrastOnEsc = me->blContrast;
						DialogBoxEx_create( pThis,
											0, 
											(XCHAR *)TEXTS[TIT_CONTRASTE][me->language],
											(XCHAR *)TEXTS[TXT_Guardar_cambios_][me->language], 
											OnDiscardChanges, OnSaveChanges, 
											0, 0,
											(XCHAR *)TEXTS[TXT_NO][me->language],
											(XCHAR *)TEXTS[TXT_SI][me->language],
											GUI_State_Contrast,
											NULL);													
						break;
					case KEY_CANCEL:
						IBeepLong();
						OnDiscardChanges(pThis);
						TRAN(&GUI_State_Configuration);
				}	 
			}	
		break;
	}        
	return 1;
}

static void OnDiscardChanges(void *pThis){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	me->blContrast = initialContrast;
	contrastOnEsc = (uint8_t)-1;
	IBacklightSetContrast(me->blContrast);
	mustExit = TRUE;
}

static void OnSaveChanges(void *pThis){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	char *vector[2];
	uint16_t size[2];
	contrastOnEsc = (uint8_t)-1;
	vector[0] = (char*)&me->blContrast;
	vector[1] = 0;
	size[0] = 1;
	size[1] = 0;
	IBacklightSetContrast(me->blContrast);
	FileSystem_SaveContrast(vector, size,1);
	mustExit = TRUE;
}

