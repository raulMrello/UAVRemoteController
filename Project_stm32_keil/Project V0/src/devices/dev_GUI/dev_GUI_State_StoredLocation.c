/*************************************************************************//**
 * @file    dev_GUI_State_StoredCities.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine implementation for StoredCities form.
 * @date    15.06.2010
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

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     GUI_State_StoredCities
 * @brief   StoredCities state must show a StoredCities list with all the functions allowable
 *			by the device
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_StoredLocation(Object* pThis, Event const * e){
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
	char *vectors[2];
	uint16_t sizes[2];	
	(void)m;	// not used
	vectors[0] = (char*)me->pGps;
	sizes[0] = sizeof(STRGps);
	vectors[1] = 0;
	sizes[1] = 0;
	// read gps.txt file and send location data
	if(FileSystem_ReadLocation((char*)"gps.txt",vectors,sizes)== 0){
		GUI_FormatLocationToSend(me);	
		TRAN(&GUI_State_SendLocation);
	}
	TRAN(&GUI_State_Location);
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
	// dealloc reserved memory
	HANDLED();
}

/****************************************************************************************************//**
 *                        GRAPHICS HANDLERS 
 ********************************************************************************************************/


