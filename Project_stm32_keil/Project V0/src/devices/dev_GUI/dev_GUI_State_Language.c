/*************************************************************************//**
 * @file    dev_GUI_State_Language.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine implementation for MENU form.
 * @date    15.06.2010
 * @author  Ra�l M.
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
static void	 	OnFormDisplay	(void * pThis);
static void 	OnSaveLanguage	(void * pThis);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     GUI_State_Language
 * @brief   Menu state must show a menu list with all the functions allowable
 *			by the device
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_Language(Object* pThis, Event const * e){
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
    GUI_LoadScreen(OnFormCreate, OnFormDisplay, FlexMenu_Callback);
	me->timeout = GUI_MBX_TIMEOUT; 	// a safety time till a msg is received
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

/****************************************************************************************************//**
 *                        GRAPHICS HANDLERS 
 ********************************************************************************************************/

static const StateHandler Transitions[] = {GUI_State_Configuration, GUI_State_Configuration};

/*************************************************************************//**
 * @fun     OnFormCreate
 * @brief   Creates the user form and all the widgets contained on it. Even
 *			it initialize all graphics variables.
 ****************************************************************************/
static void OnFormCreate(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	FlexMenu_Create(	pThis, 
						(XCHAR *)TEXTS[TIT_IDIOMA][me->language], 
						(XCHAR *)LSTBOX[LBX_IDIOMA][me->language], 
						(XCHAR *)0,
						NULL, 
						(XCHAR *)TEXTS[TXT_Guardar_cambios_][me->language], 
						(XCHAR *)TEXTS[TXT_NO][me->language], 
						(XCHAR *)TEXTS[TXT_SI][me->language], 
						NULL,
						OnSaveLanguage,
						GUI_State_Configuration,
						(StateHandler *)Transitions);

}

/*************************************************************************//**
 * @fun     OnFormDisplay
 * @brief   Displays all the information required in this state
 ****************************************************************************/
static void OnFormDisplay(void * pThis){
	(void)pThis;
}

/*************************************************************************//**
 * @fun     OnFormDisplay
 * @brief   Displays all the information required in this state
 ****************************************************************************/
static void OnSaveLanguage(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	char *vector[2];
	uint16_t size[2];
	vector[0] = (char*)&me->language;
	vector[1] = 0;
	size[0] = 1;
	size[1] = 0;
  	me->language = me->selectedOption;
	FileSystem_SaveLanguage(vector, size,1); //must overwrite
	NotifyTransition(&GUI_State_Configuration);
}


