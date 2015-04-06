/*************************************************************************//**
 * @file    dev_GUI_State_CaptureLocation.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine implementation for LOCATION form.
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        	REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "dev_GUI_hsm.h"		

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
 * @fun     GUI_State_CaptureLocation
 * @brief   Menu state must show a menu list with all the functions allowable
 *			by the device
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_CaptureLocation(Object* pThis, Event const * e){
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
	// disables State_Top timeout returning during GPS scannig
	me->isToutAllowed = FALSE;
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
	(void)m;	// not used
	me->isToutAllowed = TRUE;
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
#define ID_LISTBOX1				5
#define ID_LABEL_PDU			5
#define ID_LABEL_PERCENT		6
#define ID_LABEL_PRESSOK		7

static CRtc utc;
static STATICTEXT *ltxt1;
static STATICTEXT *ltxt2;
static WINDOW *lwnd;
static XCHAR stxt1[24];
static XCHAR stxt2[24];
static XCHAR sclock[17];
static uint8_t cmFlags;
static uint32_t timeoutToExit = 0;
static uint32_t cntReady;

#define CMFLAGS_INPROGRESS		0
#define CMFLAGS_GPSREADY		1
#define CMFLAGS_USERACCEPTED	2
#define CMFLAGS_WAITUSER		3
#define CMFLAGS_WAITOPTIONS		4
#define CMFLAGS_MUSTEXIT		255



static void OnIncrHour(void *pThis);
static void OnDecrHour(void *pThis);
static void OnUpdateClock(void *pThis);
static void OnSaveLocation(void *pThis);
static void OnSendLocation(void *pThis);
static void OnExitForced(void *pThis);
static void ReloadGraphics(void * pThis);

/*************************************************************************//**
 * @fun     OnFormCreate
 * @brief   Creates the user form and all the widgets contained on it. Even
 *			it initialize all graphics variables.
 ****************************************************************************/
static void OnFormCreate(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;

	me->rwPdu = &commPdu;
	me->rwPdu->progs = &progs[0][0];

	// initialize private data
	*me->pGps = gps_init;
	cmFlags = CMFLAGS_INPROGRESS;
  	strcpy(stxt1, (XCHAR *)TEXTS[TXT_Sincronizacion][me->language]);
	strcpy(stxt2, (XCHAR *)TEXTS[TXT_en_progreso][me->language]);
	ReloadGraphics(pThis);
	timeoutToExit = 0;

	// starts GPS reading
	IBacklightForbid();
	IBacklightOff();

	ICaptureLocation();
}

static void ReloadGraphics(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	GOLFree();

	lwnd = WndCreate(MENU_OBJ_WINDOW_1,       		// ID
              0,0,GetMaxX(),GetMaxY(), 	// dimension
              WND_DRAW |WND_TITLECENTER,                	// will be dislayed after creation
              NULL, //(void*)&OrbisMenuInvert,               	// icon
              (XCHAR *)TEXTS[TIT_CAPTURA_GPS][me->language],	   				// set text 
              me->WindowsScheme);               // default GOL scheme 
   

	ltxt1 = StCreate(ID_LABEL_PDU, 4, 20, GetMaxX()-4, 32, ST_CENTER_ALIGN | ST_UPDATE | ST_DRAW, &stxt1[0], me->altScheme); 
	ltxt2 = StCreate(ID_LABEL_PERCENT, 4, 32, GetMaxX()-4, 44, ST_CENTER_ALIGN | ST_UPDATE | ST_DRAW, &stxt2[0], me->altScheme); 
}

/*************************************************************************//**
 * @fun     OnFormDisplay
 * @brief   Displays all the information required in this state
 ****************************************************************************/
static void OnFormDisplay(void * pThis){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	
	// tilt txt2 while GPS is synchronizing
	switch(cmFlags){
		case CMFLAGS_INPROGRESS:
			timeoutToExit++;
			if(timeoutToExit > 3000){
				NotifyTransition(&GUI_State_Location);
			}
			me->statCounter = (me->statCounter+1)&7;
			if(me->statCounter < 4){
				strcpy(stxt1, (XCHAR*)TEXTS[TXT_Sincronizacion][me->language]);	
				strcpy(stxt2, (XCHAR*)TEXTS[TXT_en_progreso][me->language]);	
			}
			else{
				strcpy(stxt1, "");	
				strcpy(stxt2, "");	
			}
			SetState(ltxt1, ST_DRAW);
			SetState(ltxt2, ST_DRAW);
			break;
		case CMFLAGS_GPSREADY:
			timeoutToExit = 0;			
			strcpy(stxt1, (XCHAR*)TEXTS[TXT_Lat_][me->language]);
			GPS_LatitudeToString(me->pGps, &stxt1[5]);
			strcpy(stxt2, (XCHAR*)TEXTS[TXT_Lon_][me->language]);
			GPS_LongitudeToString(me->pGps, &stxt2[5]);
			StCreate(ID_LABEL_PRESSOK, 4, GetMaxY()-14, GetMaxX()-4, GetMaxY()-2, ST_CENTER_ALIGN | ST_UPDATE | ST_DRAW, (XCHAR *)TEXTS[TXT_pulse_ok][me->language], me->altScheme); 			
			SetState(ltxt1, ST_DRAW);
			SetState(ltxt2, ST_DRAW);
			cmFlags = CMFLAGS_WAITUSER;
			break;
		case CMFLAGS_USERACCEPTED:
			timeoutToExit = 0;
			//reloads windows
			ReloadGraphics(pThis);
			WndDraw(lwnd);
			DialogBoxEx_create(	pThis,
								0, 
								(XCHAR *)TEXTS[TIT_CAPTURA_GPS][me->language],
								(XCHAR *)TEXTS[TXT_Opciones_][me->language], 
								OnSaveLocation, 
								OnSendLocation, 
								0, 0,
								(XCHAR *)TEXTS[TXT_Salvar][me->language], 
								(XCHAR *)TEXTS[TXT_Volcar][me->language],
								GUI_State_Location,
								NULL);
			cmFlags = CMFLAGS_WAITOPTIONS;
			break;
		case CMFLAGS_MUSTEXIT:
			if(!ICaptureIsDown())
				break;
			NotifyTransition(&GUI_State_Location);
			break;
	}
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
				 	case KEY_DOWN:
						break;
					case KEY_UP:
						break;
					case KEY_ENTER:
						if(cmFlags == CMFLAGS_WAITUSER){
							// check integriy: "HHMMSS", "DDMMAA"
							if(CRtc_CheckStringIntegrity(&utc, me->pGps->hora, me->pGps->fecha)  == 0){
								uint32_t cntElapsed, cntNow;	
								cntElapsed = IGetCalendarCounter() - cntReady;								
								cntNow = CCalendar_GetInSeconds(&me->calendar) + cntElapsed;
								CCalendar_GetFromSeconds(&me->calendar, cntNow);
								cntReady = IGetCalendarCounter();

								//CCalendar_Init(&me->calendar);
								//CCalendar_SetFromUTC(&me->calendar, &utc);
								CTime_ToString(me->calendar.objRtc_sistema.time, sclock);
								sclock[5] = ' ';
								CDate_ToString(me->calendar.objRtc_sistema.date, &sclock[6]);
								DialogBoxEx_create(	pThis,
													0, 
													(XCHAR *)TEXTS[TXT_Sincr_reloj_][me->language],
													(XCHAR *)sclock, 
													NULL, 
													OnUpdateClock, 
													OnIncrHour, OnDecrHour,
													(XCHAR *)TEXTS[TXT_NO][me->language], 
													(XCHAR *)TEXTS[TXT_SI][me->language],
													NULL,NULL);
							}
							cmFlags = CMFLAGS_USERACCEPTED;
						}
						break;
					case KEY_CANCEL:
						if(cmFlags != CMFLAGS_MUSTEXIT){
							DialogBoxEx_create(	pThis, 
											0, 
											(XCHAR *)TEXTS[TIT_CAPTURA_GPS][me->language],
											(XCHAR*)TEXTS[TXT_Salir_sin_guardar][me->language], 
											NULL, 
											OnExitForced, 
											0, 0,
											(XCHAR*)TEXTS[TXT_NO][me->language], 
											(XCHAR*)TEXTS[TXT_SI][me->language],
											NULL,
											NULL);
						}
						
				}	 
			}
			else if (pMsg->type == TYPE_GPS){
				uint8_t * pData;
				uint16_t  size;
				if(cmFlags == CMFLAGS_INPROGRESS){
					size = pMsg->uiEvent;	// get coded data size
					pData = (uint8_t*)((pMsg->param2 << 16)+(pMsg->param1)); // get coded data buffer address
					// process only if size is correct and disable GPS hardware
					if(size == sizeof(STRGps)){
						memcpy(me->pGps, pData, size);
						// check if gps_stream correct
						if(me->pGps->estado == GPS_OK){
							IReleaseLocation();							
							cntReady = IGetCalendarCounter();
							CRtc_CheckStringIntegrity(&utc, me->pGps->hora, me->pGps->fecha);
							CCalendar_Init(&me->calendar);
							CCalendar_SetFromUTC(&me->calendar, &utc, 1);
							IBeepConfirm();
							cmFlags = CMFLAGS_GPSREADY;											
						}
					}
				}
			}	
		break;
	}        
	return 1;
}

/*************************************************************************//**
 * @fun     OnExitForced
 * @brief   Set exit flag to terminate this mode
 *
 * @return  
 ****************************************************************************/
static void OnExitForced(void *pThis){
	IBeepLong();
	IReleaseLocation();
	IBacklightPermit();
	IBacklightOn();

	cmFlags = CMFLAGS_MUSTEXIT;
}

/*************************************************************************//**
 * @fun     OnUpdateClock
 * @brief   Executes callback for SYNC_CLOCK button on dialog box
 *
 * @return  
 ****************************************************************************/
static void OnUpdateClock(void *pThis){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	uint32_t cntElapsed, cntNow;	
	cntElapsed = IGetCalendarCounter() - cntReady;								
	cntNow = CCalendar_GetInSeconds(&me->calendar) + cntElapsed;
	CCalendar_GetFromSeconds(&me->calendar, cntNow);
	ISetCalendar(&me->calendar);
}

/*************************************************************************//**
 * @fun     OnIncrHour
 * @brief   Executes callback for INCR UMT to local time
 *
 * @return  
 ****************************************************************************/
static void OnIncrHour(void *pThis){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	CCalendar_AddOneHour(&me->calendar);
	CTime_ToString(me->calendar.objRtc_sistema.time, sclock);
	sclock[5] = ' ';
	CDate_ToString(me->calendar.objRtc_sistema.date, &sclock[6]);
}

/*************************************************************************//**
 * @fun     OnDecrHour
 * @brief   Executes callback for DECR UMT to local time
 *
 * @return  
 ****************************************************************************/
static void OnDecrHour(void *pThis){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	CCalendar_SubOneHour(&me->calendar);
	CTime_ToString(me->calendar.objRtc_sistema.time, sclock);
	sclock[5] = ' ';
	CDate_ToString(me->calendar.objRtc_sistema.date, &sclock[6]);
}

/*************************************************************************//**
 * @fun     OnSendLocation
 * @brief   Executes callback for SYNC_CLOCK button on dialog box
 *
 * @return  
 ****************************************************************************/
static void OnSendLocation(void *pThis){
	GUI_FormatLocationToSend(pThis);
	NotifyTransition(&GUI_State_SendLocation);
}


/*************************************************************************//**
 * @fun     OnSavedLocation
 * @brief   Executes callback for OK button on dialog box
 *
 * @return  
 ****************************************************************************/

static void OnSaveLocation(void *pThis){
	char *vectors[2];
	uint16_t sizes[2];
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;

	vectors[0] = (char*)me->pGps;
	vectors[1] = 0;
	sizes[0] = sizeof(STRGps);
	sizes[1] = 0;
	if(FileSystem_SaveLocation("gps.txt",vectors,sizes,1)==0){
		me->isGpsSaved = 1;
	}
	NotifyTransition(&GUI_State_Location);
}
