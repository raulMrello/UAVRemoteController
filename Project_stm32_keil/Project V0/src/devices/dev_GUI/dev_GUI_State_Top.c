/*************************************************************************//**
 * @file    dev_GUI_State_Top.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine Top parent state implementation
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
static State OnEntry		(T_DEV_GUI * me, T_GUI_Message const * m);
static State OnExit			(T_DEV_GUI * me, T_GUI_Message const * m);
static State OnGolMsg  		(T_DEV_GUI * me, T_GUI_Message const * m);
static State OnTranMsg 		(T_DEV_GUI * me, T_GUI_Message const * m);
static State OnTimeout		(T_DEV_GUI * me, T_GUI_Message const * m);


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
 * @fun     GUI_State_Initial
 * @brief   Initialize internal properties
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_Initial(Object* pThis, Event const * e){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	(void)e;
	me->uid = me->runnable.tid;
	me->battLevel = 4;	
	me->isToutAllowed = FALSE;	
	PutImage(2, 20, me->logo, 1);
	IDraw();
	os_dly_wait(300);
	GUI_LoadSchemes();
	if(!me->fsReady){
		me->OnFormatExit = (StateHandler)GUI_State_Top;
		TRAN(&GUI_State_Formatting);
	}	

	TRAN(&GUI_State_Top);
}


/*************************************************************************//**
 * @fun     GUI_State_Top
 * @brief   Parent state
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_Top(Object* pThis, Event const * e){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	switch(e->sig){
		case ENTRY_SIG:
			return(OnEntry(me,(T_GUI_Message const *)e));

		case EXIT_SIG:
			return(OnExit(me,(T_GUI_Message const *)e));

		case EV_GOLMSG:
			return(OnGolMsg(me,(T_GUI_Message const *)e));

		case EV_TRANSITION:
			return(OnTranMsg(me,(T_GUI_Message const *)e));
					
		case TIMEOUT_SIG:	
			return(OnTimeout(me,(T_GUI_Message const *)e));
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
	GUI_RestoreNonVolatileConfig(me);
    GUI_LoadScreen(OnFormCreate, OnFormDisplay, OnFormCallback);
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
	// disables calendar events 
	IEnDisCalendarEvents(FALSE);
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
static State OnGolMsg(T_DEV_GUI * me, T_GUI_Message const * m){
	State retStat;
	// get a reference to the message in process
	me->msg = m->msg;
	if (me->msg.type == TYPE_KEYBOARD){ //CONTROL TICK TIMEOUT
		IBeepShort();
		me->auxCounter = 0;
		IBacklightOn();
	}
	retStat = GOLMsg(&me->msg);
	// paint changes if no pending transitions.
	if(retStat != RET_TRAN){
		while(!GOLDraw());
		IDrawMem();
	}
	return(retStat);
}

/*************************************************************************//**
 * @fun     OnTranMsg
 * @brief   Handles EV_TRANSITION events
 *
 * @param   This      	this object
 * @param	m			T_GUI_Message received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnTranMsg(T_DEV_GUI * me, T_GUI_Message const * m){
	StateHandler target;
	target = (StateHandler)((m->msg.param2 << 16)+m->msg.param1);
	TRAN(target);
}

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
static State OnTimeout(T_DEV_GUI * me, T_GUI_Message const * m){
	(void)m; // not used
	me->auxCounter++;
	// paint changes
	while(!GOLDraw());
	IDrawMem();
	HANDLED();
}

/****************************************************************************************************//**
 *                        GRAPHICS HANDLERS 
 ********************************************************************************************************/
#define MENU_OBJ_WINDOW_1   	ID_WINDOW1
#define MENU_OBJ_BUTTON_1   	ID_OBJ_BUTTON_1
#define MENU_OBJ_BUTTON_2   	2
#define MENU_OBJ_BUTTON_3   	3
#define MENU_OBJ_BUTTON_4   	4
#define LTEXT_ID				5
#define PICTURE_GPS				6
#define LAT_ID					7
#define LON_ID					8

static uint8_t batscanTmr = 0;
static uint8_t timeToStop = 3;
static uint8_t comesFromStop = 0;
static char  * ltime;
static char stime[10];
static char slat[10];
static char slon[11];
void * batIcon[] = {&bat00_23x12, &bat25_23x12, &bat50_23x12, &bat75_23x12, &bat100_23x12};
WINDOW * lwnd;
STATICTEXT * stext;
STATICTEXT * stLat;
STATICTEXT * stLon;
PICTURE *leftIcon;
static void ReloadGraphics(T_DEV_GUI *me);
static void EnterStopModeAndWakeup(T_DEV_GUI *me);
/*************************************************************************//**
 * @fun     OnFormCreate
 * @brief   Creates the user form and all the widgets contained on it. Even
 *			it initialize all graphics variables.
 ****************************************************************************/
static void OnFormCreate(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	// First operation is to freed init task stack
	if(me->initStack != 0){
		free(me->initStack);
		me->initStack = 0;
	}
	me->auxCounter = 0;
	comesFromStop = 0;
	timeToStop = 3;
	// don't allow timeout to TOP Transitions
	me->isToutAllowed = FALSE;
	ReloadGraphics(me);
}

/*************************************************************************//**
 * @fun     OnFormDisplay
 * @brief   Displays all the information required in this state
 ****************************************************************************/
static void OnFormDisplay(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	static uint8_t tilt=0;
	
	tilt = (uint8_t)((tilt+1)& 0x7); // values between 0..7
	if(tilt < 4){
		stime[2] = ':';
	}
	else{
		stime[2] = ' ';
	}
	if(tilt == 0){
		// cada segundo incrementa batscanTmr y lo acota a 20 segs.
		// cada 20 segundos inicia una medida
		if(++batscanTmr > 5){
			batscanTmr = 0;
			me->battLevel = IBatteryGetLevel();
			// updates battery charge icon from window object
			lwnd->pBitmap = batIcon[me->battLevel];
		}
		// cada 20 segundos lee la batería y chequea si se puso en hora
		else if(batscanTmr == 3){
			// enables battery scanning
			IBatteryStartScan();
		}
	}
	// if no battery then adapt screen and modify  auxcounter
	if(me->battLevel == 0){
		#if DEVICE_MODEL == GPS_SAT
			leftIcon->hdr.state |= PICT_HIDE;
			PictDraw(leftIcon);
		#endif
		strcpy(stime, "");
		if(tilt < 4){
			strcpy(slat, (XCHAR *)TEXTS[TXT_Cambiar][me->language]);
			strcpy(slon, (XCHAR *)TEXTS[TXT_Pilas][me->language]);
			StDraw(stLat);				
			StDraw(stLon);
		}
		else{
			SetState(stLat, ST_HIDE);				
			SetState(stLon, ST_HIDE);
			//strcpy(slat, "");
			//strcpy(slon, "");
		}		
		
		if(me->auxCounter < 230)
			me->auxCounter = 230;
	}
	// Check to enter stop mode
	if(timeToStop>0)
		timeToStop--;

	if (me->auxCounter>300) {
		EnterStopModeAndWakeup(me);
		return;
	}

	WndDraw(lwnd);
	StDraw(stext);				
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
static uint32_t OnFormCallback(void * pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	if(comesFromStop){
		comesFromStop = 0;
		return 1;
	}
  	switch(GetObjID(pObj)){
   		case MENU_OBJ_WINDOW_1:
			if (pMsg->type == TYPE_KEYBOARD){				
				switch (pMsg->param1){
				 	case KEY_DOWN:
						break;
					case KEY_UP:
						break;
					case KEY_ENTER:
						TRAN(&GUI_State_Menu);
					case KEY_CANCEL:
						if(ICaptureIsDown() && timeToStop == 0){
							EnterStopModeAndWakeup(me);
						}
						break;
				}	 
			}
			if (pMsg->type == TYPE_TIMER){
				IGetCalendar(&me->calendar);
				CTime_ToString(me->calendar.objRtc_sistema.time, stime);
				stime[5] = 0; //only HH:MM
			}
			if (pMsg->type == TYPE_BAT){
				me->battLevel = (uint8_t)pMsg->param1;				
			}
	
		break;
	}        
	return 1;
}


/*************************************************************************//**
 * @fun     ReloadGraphics
 * @brief   Reloads graphics objects after entering or stop wakeup.
 *
 * @param   objMsg     	this object
 ****************************************************************************/
static void ReloadGraphics(T_DEV_GUI *me){
	// not used: WINDOW* pOBJ_WINDOW_1;
	GOLFree();

	IGetCalendar(&me->calendar);
	CTime_ToString(me->calendar.objRtc_sistema.time, stime);
	stime[5] = 0; // only HH:MM
	ltime = &stime[0];
	me->battLevel = IBatteryGetLevel();
	lwnd = WndCreate(MENU_OBJ_WINDOW_1,       		// ID
              0,0,GetMaxX(),GetMaxY(), 	// dimension
              WND_DRAW |WND_TITLECENTER,                	// will be dislayed after creation
              batIcon[me->battLevel], //(void*)&OrbisMenuInvert,               	// icon
              (XCHAR *)TEXTS[TXT_MENU_PULSE_OK][me->language],		// set text 
              me->WindowsScheme);               // default GOL scheme 

	stext = StCreate(LTEXT_ID, 10, 16, GetMaxX()-4, GetMaxY()-22, ST_CENTER_ALIGN | ST_UPDATE | ST_DRAW, (XCHAR*)ltime, me->hugeScheme); 
	strcpy(slat,"");
	strcpy(slon,"");
	#if DEVICE_MODEL == GPS_SAT
	if(me->isGpsSaved){
		GPS_LatitudeToString(me->pGps, slat);
		GPS_LongitudeToString(me->pGps, slon);
	}
	else{
		strcpy(slat,"--º --' N");
		strcpy(slon,"---º --' E");

	}
	leftIcon = PictCreate(PICTURE_GPS, 4, GetMaxY()-20, 19, GetMaxY()-2, PICT_DRAW , 1, (void*)&gps_15x17, me->WindowsScheme);
	#endif
	stLat = StCreate(LAT_ID, 21, GetMaxY()-24, GetMaxX()-4, GetMaxY()-14, ST_UPDATE | ST_DRAW, (XCHAR*)slat, me->minScheme); 
	stLon = StCreate(LON_ID, 21, GetMaxY()-14, GetMaxX()-4, GetMaxY()-4, ST_UPDATE | ST_DRAW, (XCHAR*)slon, me->minScheme); 
	// enables calendar events (each 1 second)
	IEnDisCalendarEvents(TRUE);
	// resets timer for battery scan operations
	batscanTmr = 0;
}


/*************************************************************************//**
 * @fun     EnterStopAndWakeup
 * @brief   Carry out entering STOP mode operations and wakeup 
 *
 * @param   objMsg     	this object
 ****************************************************************************/
static void EnterStopModeAndWakeup(T_DEV_GUI *me){
	volatile uint32_t delay = 0;
	int8_t keyPressed = 0;
	IBatteryCancelScan();	
	IDisplayStandby();
	IBacklightOff();
	#if HARDWARE_RELEASE == HW_REV2
	IDisplayOff();
	IUnpowerRemote();
	#endif
	keyPressed = 0;
	/*##### STOP MODE ######*/
	comesFromStop = 1;
	timeToStop = 10;
	do{
		if(keyPressed != KEY_CANCEL)
			IEnterStopMode();
		for(delay=0; delay<2000000;delay++){
			delay = delay;
		}
		keyPressed = IGetKey();
	}while(keyPressed != KEY_CANCEL);

	/*##### STOP MODE ######*/
	comesFromStop = 1;
	
	IResartClocks();		// required after stop mode
	me->auxCounter = 0;		// restart counters
	#if HARDWARE_RELEASE == HW_REV2
	IPowerRemote();
	#endif
	IDisplayOn(me->blContrast);			// restart display
	ReloadGraphics(me);				// reloads graphics
}

