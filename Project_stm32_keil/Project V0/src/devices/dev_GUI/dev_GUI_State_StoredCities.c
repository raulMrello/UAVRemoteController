/*************************************************************************//**
 * @file    dev_GUI_State_StoredLocation.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine implementation for StoredLocation form.
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
static void  	OnFormCreate	(void * pThis);
static void	 	OnFormDisplay	(void * pThis);

static char * title;


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     GUI_State_StoredLocation
 * @brief   StoredLocation state must show a StoredLocation list with all the functions allowable
 *			by the device
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_StoredCities(Object* pThis, Event const * e){
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
	// dealloc reserved memory
	if(title != 0){
		free(title);
	}
	HANDLED();
}

/****************************************************************************************************//**
 *                        GRAPHICS HANDLERS 
 ********************************************************************************************************/

#define DEEP_COUNTRY	0
#define DEEP_REGION		1u
#define DEEP_CITY		2u
#define DEEP_FILESYSTEM	255u

static const StateHandler Transitions[] = {GUI_State_StoredCities, GUI_State_StoredCities};

static void OnSelectedLocation(void * pThis, char * pText);
static void OnAcceptedLocation(void * pThis);

static uint8_t selCountry, selRegion, selCity;


/*************************************************************************//**
 * @fun     OnFormCreate
 * @brief   Creates the user form and all the widgets contained on it. Even
 *			it initialize all graphics variables.
 ****************************************************************************/
static void OnFormCreate(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	// point to external protected data
	me->rwPdu = &commPdu;
	me->rwPdu->progs = &progs[0][0];
	// alloc memory for title
	title = malloc(32);
	if(title == 0){
		NotifyTransition(&GUI_State_Menu);
	}
	// reset deepness to choose country
	me->deepness = DEEP_COUNTRY;
	selCountry = (uint8_t)-1;
	selRegion = (uint8_t)-1;
	selCity = (uint8_t)-1;

	FlexMenu_Create(	pThis, 								
						(XCHAR *)TEXTS[TIT_CIUDADES_POR_PAIS][me->language], 
						(XCHAR *)GUI_GetCountries(), 
						(XCHAR *)0,
						OnSelectedLocation, 
						(XCHAR *)0, 
						(XCHAR *)0, 
						(XCHAR *)0, 
						NULL,
						NULL, 
						GUI_State_Location,
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
 * @fun     OnSelectedLocation
 * @brief   Handles ENTER pressing button, according with the deepness status.
 * @param	pThis	GUI object
 * @param	pText	Selected object text description
 ****************************************************************************/
static void OnSelectedLocation(void * pThis, char * pText){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	uint8_t eos = (uint8_t)(strchr(pText,'\n')-pText);
	// update title with pText pointer
	memcpy(title, pText, eos);	
	title[eos] = 0;
	// check deepness for further actions
	switch(me->deepness){
		case DEEP_COUNTRY:
			selCountry = me->selectedOption;
			FlexMenu_Create(	pThis, 
								title, 
								GUI_GetRegions(selCountry), 
								(XCHAR *)0,
								OnSelectedLocation, 
								(XCHAR *)0, 
								(XCHAR *)0, 
								(XCHAR *)0, 
								NULL,
								NULL, 
								GUI_State_StoredCities,
								(StateHandler *)Transitions);
			me->deepness++;
			break;
		case DEEP_REGION:
			selRegion = me->selectedOption;
			FlexMenu_Create(	pThis, 
								title, 
								GUI_GetCities(selCountry, selRegion), 
								(XCHAR *)0,
								NULL, 
								(XCHAR *)TEXTS[TXT_Volcar_][me->language], 
								(XCHAR *)TEXTS[TXT_NO][me->language], 
								(XCHAR *)TEXTS[TXT_SI][me->language], 
								NULL,
								OnAcceptedLocation, 
								GUI_State_StoredCities,
								(StateHandler *)Transitions);
			me->deepness++;
			break;
		case DEEP_CITY:
			break;
		case DEEP_FILESYSTEM:
			selCountry = (uint8_t)-1;
			selRegion = (uint8_t)-1;
			selCity = (uint8_t)-1;
			break;
	}
}

/*************************************************************************//**
 * @fun     OnAcceptedLocation
 * @brief   Handles accepted location actions
 ****************************************************************************/
static void OnAcceptedLocation(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;

	// update selected city
	selCity = me->selectedOption;
	// on accepted city, tran to State_SendLocation
	if(selCountry != (uint8_t)-1 && selRegion != (uint8_t)-1 && selCity != (uint8_t)-1){
		memset((char*)&me->rwPdu->Field[0][0],13,0);
		me->rwPdu->Field[0][1] = (uint8_t)0xff;
		me->rwPdu->Field[0][2] = selCountry;
		me->rwPdu->Field[0][3] = GUI_GetCityCode(selCountry, selRegion, selCity);
		me->rwPdu->Field[0][4] = GUI_GetCountryCode(selCountry);
	}
	// actualiza el reloj
	IGetCalendar(&me->calendar);
	memset((char*)&me->rwPdu->Field[2][0],10,0);
	me->rwPdu->Field[2][0] = 3;								//PDU_WRITE_CLOCK
	me->rwPdu->Field[2][1] = (uint8_t)me->calendar.status;			//flags
	me->rwPdu->Field[2][2] = me->calendar.objRtc_sistema.time.seg;
	me->rwPdu->Field[2][3] = me->calendar.objRtc_sistema.time.min;
	me->rwPdu->Field[2][4] = me->calendar.objRtc_sistema.time.hor;
	me->rwPdu->Field[2][5] = me->calendar.objRtc_sistema.time.sem;
	me->rwPdu->Field[2][6] = me->calendar.objRtc_sistema.date.dia;
	me->rwPdu->Field[2][7] = me->calendar.objRtc_sistema.date.mes;
	me->rwPdu->Field[2][8] = me->calendar.objRtc_sistema.date.ani;
	NotifyTransition(&GUI_State_SendLocation);
}


