/*************************************************************************//**
 * @file    dev_GUI_State_DateTime.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine implementation for LOCATION form.
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
static uint32_t OnFormCallback	(void * pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     GUI_State_DateTime
 * @brief   Menu state must show a menu list with all the functions allowable
 *			by the device
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_DateTime(Object* pThis, Event const * e){
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
	// get the actual time and date
	IGetCalendar(&me->calendar);
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
#define ID_LABEL_TIME			5
#define ID_LABEL_DATE			6

static STATICTEXT *lhou;
static STATICTEXT *ldp1;
static STATICTEXT *lmin;
static STATICTEXT *ldp2;
static STATICTEXT *lsec;

static STATICTEXT *lday;
static STATICTEXT *ltb1;
static STATICTEXT *lmon;
static STATICTEXT *ltb2;
static STATICTEXT *lyea;

static WINDOW * lwnd; 

static XCHAR stime[9];
static XCHAR shou[3];
static XCHAR smin[3];
static XCHAR ssec[3];

static XCHAR sdate[11];
static XCHAR sday[3];
static XCHAR smon[3];
static XCHAR syea[5];

typedef enum {
	SETUP_HOU = 0,
	SETUP_MIN,
	SETUP_DAY,
	SETUP_MON,
	SETUP_YEA,
	SETUP_EXIT
}E_DateTime_Status;

static E_DateTime_Status status;
static uint8_t tilt = 0;


static void OnDiscardChanges (void *pThis);
static void OnSaveChanges	 (void *pThis);
static void DecrementVariable(T_DEV_GUI *me);
static void IncrementVariable(T_DEV_GUI *me);

/*************************************************************************//**
 * @fun     OnFormCreate
 * @brief   Creates the user form and all the widgets contained on it. Even
 *			it initialize all graphics variables.
 ****************************************************************************/
static void OnFormCreate(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	// not used: WINDOW* pOBJ_WINDOW_1;
	GOLFree();

	status = SETUP_HOU;
	lwnd = WndCreate(MENU_OBJ_WINDOW_1,       		// ID
              0,0,GetMaxX(),GetMaxY(), 	// dimension
              WND_DRAW |WND_TITLECENTER,                	// will be dislayed after creation
              NULL, //(void*)&OrbisMenuInvert,             	// icon
              (XCHAR *)TEXTS[TIT_HORA + status][me->language],	   									// set text 
              me->WindowsScheme);               			// default GOL scheme 
   
	CTime_ToString(me->calendar.objRtc_sistema.time, &stime[0]);
	shou[0] = stime[0]; shou[1] = stime[1]; shou[2] = 0;
	smin[0] = stime[3]; smin[1] = stime[4]; smin[2] = 0;
	ssec[0] = '0'; ssec[1] = '0'; ssec[2] = 0;
	//ssec[0] = stime[6]; ssec[1] = stime[7]; ssec[2] = 0;

  	CDate_ToString(me->calendar.objRtc_sistema.date, &sdate[0]);
	sday[0] = sdate[0]; sday[1] = sdate[1]; sday[2] = 0;
	smon[0] = sdate[3]; smon[1] = sdate[4]; smon[2] = 0;
	syea[0] = sdate[6]; syea[1] = sdate[7]; syea[2] = sdate[8]; syea[1] = sdate[9]; syea[2] = 0;

	lhou = StCreate(ID_LABEL_TIME, 30, 20, 45, 32, ST_UPDATE | ST_DRAW, &shou[0], me->altScheme); 
	ldp1 = StCreate(ID_LABEL_TIME, 45, 20, 50, 32, ST_UPDATE | ST_DRAW, ":", me->altScheme); 
	lmin = StCreate(ID_LABEL_TIME, 55, 20, 70, 32, ST_UPDATE | ST_DRAW, &smin[0], me->altScheme); 
	ldp2 = StCreate(ID_LABEL_TIME, 70, 20, 80, 32, ST_UPDATE | ST_DRAW, ":", me->altScheme); 
	lsec = StCreate(ID_LABEL_TIME, 80, 20, GetMaxX()-10, 32, ST_UPDATE | ST_DRAW, &ssec[0], me->altScheme); 

	lday = StCreate(ID_LABEL_DATE, 20, 40, 35, 52, ST_UPDATE | ST_DRAW, &sday[0], me->altScheme); 
	ltb1 = StCreate(ID_LABEL_DATE, 35, 40, 40, 52, ST_UPDATE | ST_DRAW, "/", me->altScheme); 
	lmon = StCreate(ID_LABEL_DATE, 45, 40, 60, 52, ST_UPDATE | ST_DRAW, &smon[0], me->altScheme); 
	ltb2 = StCreate(ID_LABEL_DATE, 60, 40, 70, 52, ST_UPDATE | ST_DRAW, "/", me->altScheme); 
	lyea = StCreate(ID_LABEL_DATE, 70, 40, GetMaxX()-10, 52, ST_UPDATE | ST_DRAW, &syea[0], me->altScheme); 

	tilt = 0;
}

/*************************************************************************//**
 * @fun     OnFormDisplay
 * @brief   Displays all the information required in this state
 ****************************************************************************/
static void OnFormDisplay(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	tilt = (uint8_t)((tilt+1)& 0x7); // values between 0..7

	if(status < SETUP_EXIT){
		WndSetText(lwnd, (XCHAR *)TEXTS[TIT_HORA + status][me->language]);
		WndDraw(lwnd);
	}
		
	if(tilt > 3){
		switch(status){
			case SETUP_HOU:
				shou[0] = 0;
				break;
			case SETUP_MIN:
				smin[0] = 0;
				break;
			case SETUP_DAY:
				sday[0] = 0;
				break;
			case SETUP_MON:
				smon[0] = 0;
				break;
			case SETUP_YEA:
				syea[0] = 0;
				break;
			case SETUP_EXIT:
				IBeepLong();
				NotifyTransition(&GUI_State_Configuration);
				break;
		}
	}
	else{
		CTime_ToString(me->calendar.objRtc_sistema.time, &stime[0]);
		shou[0] = stime[0]; shou[1] = stime[1]; shou[2] = 0;
		smin[0] = stime[3]; smin[1] = stime[4]; smin[2] = 0;
		ssec[0] = '0'; ssec[1] = '0'; ssec[2] = 0;
		//ssec[0] = stime[6]; ssec[1] = stime[7]; ssec[2] = 0;

	  	CDate_ToString(me->calendar.objRtc_sistema.date, &sdate[0]);
		sday[0] = sdate[0]; sday[1] = sdate[1]; sday[2] = 0;
		smon[0] = sdate[3]; smon[1] = sdate[4]; smon[2] = 0;
		syea[0] = sdate[6]; syea[1] = sdate[7]; syea[2] = sdate[8]; syea[3] = sdate[9]; syea[4] = 0;
	}
	SetState(lhou,ST_DRAW);
	SetState(ldp1,ST_DRAW);				
	SetState(lmin,ST_DRAW);				
	SetState(ldp2,ST_DRAW);				
	SetState(lsec,ST_DRAW);				

	SetState(lday,ST_DRAW);
	SetState(ltb1,ST_DRAW);				
	SetState(lmon,ST_DRAW);				
	SetState(ltb2,ST_DRAW);				
	SetState(lyea,ST_DRAW);				
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
				tilt = 0;				
			   	switch (pMsg->param1){
				 	case KEY_DOWN:
						DecrementVariable(me);
						break;
					case KEY_UP:
						IncrementVariable(me);
						break;
					case KEY_ENTER:
						DialogBoxEx_create( pThis,
											0, 
											(XCHAR *)TEXTS[TIT_HORAFECHA][me->language],
											(XCHAR *)TEXTS[TXT_Guardar_cambios_][me->language], 
											OnDiscardChanges, OnSaveChanges, 
											0, 0,
											(XCHAR *)TEXTS[TXT_NO][me->language],
											(XCHAR *)TEXTS[TXT_SI][me->language],
											NULL,
											NULL);							
						break;

					case KEY_RIGHT:
						status = (E_DateTime_Status)((status < SETUP_YEA)? (status+1) : (status));
						break;
					case KEY_LEFT:
						status = (E_DateTime_Status)((status > SETUP_HOU)? (status-1) : (status));
						break;
					case KEY_CANCEL:
						status = SETUP_EXIT;
						break;
				}	 
			}	
		break;
	}        
	return 1;
}

/*************************************************************************//**
 * @fun     DecrementVariable
 * @brief   Decrements the variable in selection
 *
 * @return  
 ****************************************************************************/
static void DecrementVariable(T_DEV_GUI *me){
	switch(status){
		case SETUP_HOU:
			me->calendar.objRtc_sistema.time.hor = (me->calendar.objRtc_sistema.time.hor > 0)? (me->calendar.objRtc_sistema.time.hor-1) : 23;
			break;
		case SETUP_MIN:
			me->calendar.objRtc_sistema.time.min = (me->calendar.objRtc_sistema.time.min > 0)? (me->calendar.objRtc_sistema.time.min-1) : 59;
			break;
		case SETUP_DAY:
			CDate_DecDia(&me->calendar.objRtc_sistema.date);
			break;
		case SETUP_MON:
			me->calendar.objRtc_sistema.date.mes = (me->calendar.objRtc_sistema.date.mes > 1)? (me->calendar.objRtc_sistema.date.mes-1) : 12;
			break;
		case SETUP_YEA:
			me->calendar.objRtc_sistema.date.ani = (me->calendar.objRtc_sistema.date.ani > 10)? (me->calendar.objRtc_sistema.date.ani-1) : 99;
			break;
	}
	CDate_CheckIntegrity(&me->calendar.objRtc_sistema.date);

}

/*************************************************************************//**
 * @fun     IncrementVariable
 * @brief   Increments the variable in selection
 *
 * @return  
 ****************************************************************************/
static void IncrementVariable(T_DEV_GUI *me){
	switch(status){
		case SETUP_HOU:
			me->calendar.objRtc_sistema.time.hor = (me->calendar.objRtc_sistema.time.hor < 23)? (me->calendar.objRtc_sistema.time.hor+1) : 0;
			break;
		case SETUP_MIN:
			me->calendar.objRtc_sistema.time.min = (me->calendar.objRtc_sistema.time.min <59)? (me->calendar.objRtc_sistema.time.min+1) : 0;
			break;
		case SETUP_DAY:
			CDate_IncDia(&me->calendar.objRtc_sistema.date);
			break;
		case SETUP_MON:
			me->calendar.objRtc_sistema.date.mes = (me->calendar.objRtc_sistema.date.mes < 12)? (me->calendar.objRtc_sistema.date.mes+1) : 1;
			break;
		case SETUP_YEA:
			me->calendar.objRtc_sistema.date.ani = (me->calendar.objRtc_sistema.date.ani < 99)? (me->calendar.objRtc_sistema.date.ani+1) : 10;
			break;
	}
	CDate_CheckIntegrity(&me->calendar.objRtc_sistema.date);
}

static void OnDiscardChanges(void *pThis){
	status = SETUP_EXIT;
}

static void OnSaveChanges(void *pThis){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	ISetCalendar(&me->calendar);
	status = SETUP_EXIT;
}
