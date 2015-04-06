/*************************************************************************//**
 * @file    dev_GUI.c
 * @mcu		STM32F1x
 * @brief   GUI Device Manager
 * @date    15.07.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        	REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "dev_GUI_hsm.h"		// includes dev_IR.h


/****************************************************************************************************//**
 *                        	PROTECTED DATA
 ********************************************************************************************************/
uint8_t 	progs[MAX_PROGS][22];
T_PDU_Full 	commPdu;
static STRGps gps;


/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/

static T_DEV_GUI 		This;
static GOL_SCHEME   *	altScheme;                                 // alternative style scheme
static GOL_SCHEME   *	minScheme;                                 // alternative style scheme
static GOL_SCHEME	*	WindowsScheme;
static GOL_SCHEME	*	hugeScheme;
static SCREEN_STATES   	screenState;
static void (*OnCreateForm)(void *pThis)=NULL;
static void (*OnDisplayForm)(void *pThis)=NULL;
static uint32_t (*OnCallbackForm)(void *pThis, uint32_t objMsg, OBJ_HEADER *pObj,GOL_MSG *pMsg)=NULL;
extern const FONT_FLASH Arial_24_num; 	// huge time digits
extern const FONT_FLASH Arial_14; 		// title font
extern const FONT_FLASH Arial_12; 		// small font


static void GUI_Task	(Object *pThis);
#define GUI_STACK_SIZE	2048	// tamaño en bytes
static volatile unsigned long long GUI_Stack[(GUI_STACK_SIZE/8)];


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		dev_GUI_Init
 * @brief	Provided interface used by the initial system loading to carry out GUI Device initialization
 * @return	
 **********************************************************************************/
uint8_t dev_GUI_Init(uint8_t priority, uint8_t isFSready, void *initStack){
	ErrorCode err = ERR_OK;
	// reference to main logo to show and language selection predefined
	This.logo = IGetLogo();
	This.language = DEFAULT_LANGUAGE;
	This.fsReady = isFSready;
	This.initStack = initStack;
	This.mustBeFreed = 0;
	This.pGps = &gps;
	This.OnFormatExit = 0;
	// Initialize IRunnable interface. If succesful, an OS_TID is assigned.
	err = IRunnable_Init   (&This.runnable, 
							&This, 
							&This.runnable.tid, 
							sizeof(T_GUI_Message), 
							10, 
							(void*)&GUI_Stack, 
							(U16)GUI_STACK_SIZE,
							priority,
							&GUI_Task,
							&GUI_State_Initial);
	return((uint8_t)err);
}

/****************************************************************************************************//**
 * @fun		dev_GUI_NewGpsData
 * @brief	Provided interface to receive data from  GPS
 *
 * @param	pdata	pointer to the data buffer received
 * @param	size	length of the data buffer
 **********************************************************************************/
void dev_GUI_NewGpsData(uint8_t *pdata, uint16_t size){
	GOL_MSG msg;
	uint32_t address = (uint32_t)pdata;
	msg.type = TYPE_GPS;
	msg.uiEvent = (uint8_t)size;
	msg.param1 = (uint16_t)(address & 0xFFFF); 
	msg.param2 = (uint16_t)((address>>16) & 0xFFFF); 
	dev_GUI_SendMsg(&msg);
}

/****************************************************************************************************//**
 * @fun		dev_GUI_NewIrData
 * @brief	Provided interface to receive data from  IR
 *
 * @param	pdata	pointer to the data buffer received
 * @param	size	length of the data buffer
 **********************************************************************************/
void dev_GUI_NewIrData(uint8_t *pdata, uint16_t size){
	GOL_MSG msg;
	uint32_t address = (uint32_t)pdata;
	msg.type = TYPE_IR;
	msg.uiEvent = (uint8_t)size;
	msg.param1 = (uint16_t)(address & 0xFFFF); 
	msg.param2 = (uint16_t)((address>>16) & 0xFFFF); 
	dev_GUI_SendMsg(&msg);
/*
	if(This.isReceiving){
		ComMenu_OnReceivedData(&This, pdata, size);
		//GUI_NewData_Reading_Hook(&This, pdata, size);
	}
	else{
		ComMenu_OnSentData(&This, pdata, size);
		//GUI_NewData_Writing_Hook(&This, pdata, size);
	}
*/
}

/****************************************************************************************************//**
 * @fun		dev_GUI_NewKey
 * @brief	Provided interface to receive keystroke codes from device KeyPad 
 *
 * @param	keycode	keystroke event code
 **********************************************************************************/
void dev_GUI_NewKey	(uint32_t keycode){
	GOL_MSG msg;
	msg.type = TYPE_KEYBOARD;
	msg.uiEvent = EVENT_PRESS;
	msg.param1 = (uint16_t)keycode; 
	dev_GUI_SendMsg(&msg);
}

/****************************************************************************************************//**
 * @fun		dev_GUI_NewTime
 * @brief	Provided interface to receive realtime event codes from device Calendar
 *
 * @param	timecode	time event code
 **********************************************************************************/
void dev_GUI_NewTime(uint32_t timecode){
	GOL_MSG msg;
	msg.type = TYPE_TIMER;
	msg.uiEvent = EVENT_SET;
	msg.param1 = (uint16_t)timecode; 
	dev_GUI_SendMsg(&msg);
}

/****************************************************************************************************//**
 * @fun		dev_GUI_SendMsg
 * @brief	Provided interface to be used by different modules to post GOL_MSG messages
 *
 * @param	msg				pointer to the GOL_MSG to post
 *
 * @retval	0				succesfull
 * @retval	ERR_ARGS		argument checking errors
 * @retval	ERR_MEM			memory allocation errors
 * @retval	ERR_MBOX_FULL	mbx is full
 **********************************************************************************/
int8_t dev_GUI_SendMsg(GOL_MSG *msg){
	T_GUI_Message *guiMsg;

	// arg checking
	if(msg == 0)
		return((uint8_t)ERR_ARGS);
	
	// message allocation
	guiMsg = _alloc_box(This.runnable.inbox.memPool);
	if(guiMsg == 0)
		return ((uint8_t)ERR_MEM);
	
	// message building except TYPE_TRANSITION
	if(msg->type == TYPE_TRANSITION){
		guiMsg->super.sig = EV_TRANSITION;
	}
	else{
		guiMsg->super.sig = EV_GOLMSG;
	}	
	memcpy(&guiMsg->msg, msg, sizeof(GOL_MSG));
	
	// mailbox checking
	if(os_mbx_check(This.runnable.inbox.mailBox) == 0)
		return((uint8_t)ERR_MBOX_FULL);

	// message sending. Waits forever	
	os_mbx_send(This.runnable.inbox.mailBox, guiMsg, 0xffff);
	return((uint8_t)ERR_OK);
}

/****************************************************************************************************//**
 * @fun		GUI_Task
 * @brief	Task for this device. Launches the initial state of the HSM and afterwards waits for 
 *			incoming messages (os_mbx_wait). If timeout is overrun then a timeout event is dispatched to the
 *			HSM. In other case, the proper event is dispatched.
 * @return	
 **********************************************************************************/
static void GUI_Task(Object *pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	T_GUI_Message * msg;
    // Starts the state machine
    StateMachine_Init(me);       

	// paint first time
	while(!GOLDraw());
	IDrawMem();
	for(;;){
		OS_RESULT errno;
		errno = os_mbx_wait(me->runnable.inbox.mailBox, (void**)&msg, me->timeout);
		// if timeout, launch timeout event
		if(errno == OS_R_TMO){
			StateMachine_Dispatch(me, (Event const *)&timeoutEvt);
		}
		// if msg is valid : !NULL and !timeout event
		// dispatch message to the state machine and then free box preallocation
		else if(msg != 0 && errno != OS_R_TMO){
			StateMachine_Dispatch(me, (Event const *)msg);
			_free_box(me->runnable.inbox.memPool, msg);
		}				
	}
}

/*************************************************************************//**
 * @fun     GOLMsgCallback
 * @brief   This function must be implemented by user. GOLMsg() function calls it each
 *          time the valid message for the object received
 *
 * @param   objMsg 	translated message for the object,
 * @param   pObj 	pointer to the object
 * @param   pMsg 	pointer to the non-translated, raw GOL message
 *
 * @return	if the function returns non-zero the message will be processed by default
 ****************************************************************************/
uint16_t GOLMsgCallback(uint16_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg){
   if (pMsg->type == TYPE_KEYBOARD){
	}

	// beep if button is pressed
    if(objMsg == BTN_MSG_PRESSED) {
        //Beep();
    }
    else {
        if(GetObjType(pObj) == OBJ_RADIOBUTTON) {
            //Beep();
        }
        else if(GetObjType(pObj) == OBJ_CHECKBOX) {
            //Beep();
        }
    }
    // process messages for demo screens
    switch(screenState) {    
		case GUI_DISPLAY:
			OnCallbackForm = (uint32_t (*)(void *, uint32_t , OBJ_HEADER *,GOL_MSG *))This.OnCallbackForm;
			return OnCallbackForm(&This, objMsg, pObj,pMsg);
        default:
			 return (1);	// process message by default
    }
}


/*************************************************************************//**
 * @fun     GOLDrawCallback
 * @brief   This function must be implemented by user. GOLDraw() function calls it each
 *          time when GOL objects drawing is completed. User drawing should be done here.
 *          GOL will not change color, line type and clipping region settings while
 *
 * @return  if the function returns non-zero the draw control will be passed to GOL
 ****************************************************************************/
uint16_t GOLDrawCallback(void) {
    switch(screenState) {
	   	case GUI_CREATE:
			OnCreateForm = (void (*)(void *))This.OnCreateForm;
			OnCreateForm(&This);
			screenState = GUI_DISPLAY;
			return 1;
		case GUI_DISPLAY:
			// Check to return to Top state
			if(This.isToutAllowed && This.auxCounter > MAX_TIMEOUT_TO_TOP){
				NotifyTransition(GUI_State_Top);
			}

		   	OnDisplayForm = (void (*)(void *))This.OnDisplayForm;
			OnDisplayForm(&This);		   	
		   	return 1;
	}
    return (1); // release drawing control to GOL
}


/****************************************************************************************************//**
 * @fun		NotifyTransition
 * @brief	Protected interface to request TRAN operation from DisplayCallback functions. 
 *
 * @param	keycode	keystroke event code
 **********************************************************************************/
void NotifyTransition (StateHandler target){
	GOL_MSG msg;
	uint32_t address = (uint32_t)target;
	msg.type = TYPE_TRANSITION;
	msg.uiEvent = EVENT_INVALID;
	msg.param1 = (uint16_t)(address & 0xFFFF); 
	msg.param2 = (uint16_t)((address>>16) & 0xFFFF); 
	dev_GUI_SendMsg(&msg);
}


/****************************************************************************************************//**
 * @fun		GUI_LoadScreen
 * @brief	Load screen parameters and callbacks 
 *
 * @param	keycode	keystroke event code
 **********************************************************************************/
void GUI_LoadScreen (void (*create_func)(void *pThis),void (*display_func)(void *pThis),uint32_t (*callback_func)(void *pThis,uint32_t objMsg, OBJ_HEADER *pObj,GOL_MSG *pMsg)){
		 
		 GOLFree();
		 if(This.mustBeFreed != 0){
		 	free(This.mustBeFreed);
			This.mustBeFreed = 0;
		 }
		 IBacklightPermit();
		 screenState = GUI_CREATE;
		 OnCallbackForm = callback_func;
		 OnCreateForm = create_func;
		 OnDisplayForm = display_func;
		 This.OnCallbackForm = (void*)callback_func;
		 This.OnCreateForm = (void*)create_func;
		 This.OnDisplayForm = (void*)display_func;		 		  
}


/****************************************************************************************************//**
 * @fun		GUI_LoadSchemes
 * @brief	Load default schemes 
 *
 * @param	keycode	keystroke event code
 **********************************************************************************/
void GUI_LoadSchemes(void){

    WindowsScheme = GOLCreateScheme(); 		// create alternative 1 style scheme
    WindowsScheme->Color0 = BLACK; 		
    WindowsScheme->Color1 = BLACK;
    WindowsScheme->ColorDisabled = WHITE;
    WindowsScheme->TextColor0 = WHITE; //WHITE;
    WindowsScheme->TextColor1 = WHITE; //
    WindowsScheme->TextColorDisabled = WHITE;
    WindowsScheme->EmbossDkColor = BLACK; 		
    WindowsScheme->EmbossLtColor = BLACK;    
    WindowsScheme->CommonBkColor = WHITE;
    WindowsScheme->pFont = (void*)&Arial_14;    

    hugeScheme = GOLCreateScheme(); 		// create alternative 1 style scheme
    hugeScheme->Color0 = WHITE; 		
    hugeScheme->Color1 = BLACK;
    hugeScheme->ColorDisabled = GRAY1;
    hugeScheme->TextColor0 = BLACK; //WHITE;
    hugeScheme->TextColor1 = WHITE; //
    hugeScheme->TextColorDisabled = WHITE;
    hugeScheme->EmbossDkColor = BLACK; 		
    hugeScheme->EmbossLtColor = BLACK;    
    hugeScheme->CommonBkColor = WHITE;
    hugeScheme->pFont = (void*)&Arial_24_num;//Arial_28_num;    

    altScheme = GOLCreateScheme(); 		// create alternative 1 style scheme
    altScheme->Color0 = WHITE; 		
    altScheme->Color1 = BLACK;
    altScheme->ColorDisabled = GRAY1;
    altScheme->TextColor0 = BLACK; //WHITE;
    altScheme->TextColor1 = WHITE; //
    altScheme->TextColorDisabled = WHITE;
    altScheme->EmbossDkColor = BLACK; 		
    altScheme->EmbossLtColor = BLACK;    
    altScheme->CommonBkColor = WHITE;
    altScheme->pFont = (void*)&Arial_12;    

    minScheme = GOLCreateScheme(); 		// create alternative 1 style scheme
    minScheme->Color0 = WHITE; 		
    minScheme->Color1 = BLACK;
    minScheme->ColorDisabled = GRAY1;
    minScheme->TextColor0 = BLACK; //WHITE;
    minScheme->TextColor1 = WHITE; //
    minScheme->TextColorDisabled = WHITE;
    minScheme->EmbossDkColor = BLACK; 		
    minScheme->EmbossLtColor = BLACK;    
    minScheme->CommonBkColor = WHITE;
    minScheme->pFont = (void*)&Arial_14;    

	This.WindowsScheme = (void *)WindowsScheme;
	This.altScheme = (void*)altScheme;
	This.minScheme = (void*)minScheme;
	This.hugeScheme = (void*)hugeScheme;
}


/****************************************************************************************************//**
 * @fun		GUI_RestoreNonVolatileConfig
 * @brief	(protected) Restore default config from filesystem. If something fails, overwrite with default values. 
 *
 * @param	me	GUI_object
 *
 * @return 0: if restore Ok. Else, some config was overwritten
 **********************************************************************************/
uint8_t GUI_RestoreNonVolatileConfig(T_DEV_GUI * me){
	char *	 vector[22];
	uint16_t size[22];
	uint8_t	 errno = 0;

	vector[0] = (char*)&me->language;
	vector[1] = 0;
	size[0] = 1;
	size[1] = 0;
	// Load initial config parameters
	// Language
	if(FileSystem_ReadLanguage(vector, size)!=0){
		me->language = DEFAULT_LANGUAGE;
		FileSystem_SaveLanguage(vector, size, 0); //must overwrite
		errno |= 1;
	}
	// backlight contrast
	vector[0] = (char*)&me->blContrast;
	size[0] = 1;
	if(FileSystem_ReadContrast(vector, size)!=0){
		me->blContrast = 99;
		FileSystem_SaveContrast(vector, size, 1); //must overwrite
		errno |= 1;
	}
	IBacklightSetContrast(me->blContrast);
	// gps location
	me->isGpsSaved = 0;
	if(FileSystem_CheckLocation("gps.txt")){
		vector[0] = (char*)me->pGps;
		vector[1] = 0;
		size[0] = sizeof(STRGps);
		size[1] = 0;
		if(FileSystem_ReadLocation("gps.txt",vector,size) == 0){
			me->isGpsSaved = 1;
		}
	}

	return(errno);
}

/*************************************************************************//**
 * @fun     GUI_FormatLocationToSend
 * @brief   Executes callback for SEND button on dialog box
 *
 * @return  
 ****************************************************************************/
void GUI_FormatLocationToSend(void *pThis){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	uint16_t lonGra;

	me->rwPdu = &commPdu;
	me->rwPdu->progs = &progs[0][0];

	// actualiza la zona
	memset((char*)&me->rwPdu->Field[0][0],13,0);
	me->rwPdu->Field[0][1] = (uint8_t)129;	//GMT = 1
	if(me->pGps->Posicion.latitudC[0] == 'S'){
		me->rwPdu->Field[0][2] = 128-(10*(me->pGps->Posicion.latitud[0]-48)+(me->pGps->Posicion.latitud[1]-48));
	}
	else{
		me->rwPdu->Field[0][2] = 128+1+(10*(me->pGps->Posicion.latitud[0]-48)+(me->pGps->Posicion.latitud[1]-48));		// LATG = GRA+128+1
	}
	me->rwPdu->Field[0][3] = 128+(10*(me->pGps->Posicion.latitud[2]-48)+(me->pGps->Posicion.latitud[3]-48));	// LATM = MIN+128
	me->rwPdu->Field[0][4] = 0+128;		// LATS = SEC+128
	if(me->pGps->Posicion.longitudC[0] == 'E'){
		lonGra = 1+((100*(me->pGps->Posicion.longitud[0]-48))+(10*(me->pGps->Posicion.longitud[1]-48))+(me->pGps->Posicion.longitud[2]-48)) - 32768;
	}
	else{
		lonGra = 32768-((100*(me->pGps->Posicion.longitud[0]-48))+(10*(me->pGps->Posicion.longitud[1]-48))+(me->pGps->Posicion.longitud[2]-48));
	}		
	me->rwPdu->Field[0][5] = (uint8_t)(lonGra >> 8); 		//=0x7f
	me->rwPdu->Field[0][6] = (uint8_t)(lonGra & 0xff); 		//=0xfd ---> 0x7ffd = LONG = GRA-32768 = 32765 = -3º 
	me->rwPdu->Field[0][7] = 128+(10*(me->pGps->Posicion.longitud[3]-48)+(me->pGps->Posicion.longitud[4]-48));	// LONM = MIN+128
	me->rwPdu->Field[0][8] = 0+128;		// LONS = SEC+128

	// actualiza el reloj
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
}

/*************************************************************************//**
 * @fun     GUI_FormatClockToSend
 * @brief   formats calendar into a raw data buffer to send
 *
 * @return  
 ****************************************************************************/
void GUI_FormatClockToSend(void *pThis){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	uint8_t clockFlags = me->rwPdu->Field[2][1];
	me->rwPdu = &commPdu;
	me->rwPdu->progs = &progs[0][0];
	// actualiza el reloj
	IGetCalendar(&me->calendar);
	memset((char*)&me->rwPdu->Field[2][0],10,0);
	me->rwPdu->Field[2][0] = 3;								//PDU_WRITE_CLOCK
	if(me->discardSeasonFlags){
		clockFlags = (uint8_t)me->calendar.status;			//flags
	}
	//solo actualiza el flag invierno/verano del reloj interno
	else{
		if(me->calendar.status & CCalendar_STAT_ESVER)
			clockFlags |= CCalendar_STAT_ESVER; 	
		else
			clockFlags &= ~CCalendar_STAT_ESVER; 			
	}
	me->rwPdu->Field[2][1] = clockFlags;
	me->rwPdu->Field[2][2] = me->calendar.objRtc_sistema.time.seg;
	me->rwPdu->Field[2][3] = me->calendar.objRtc_sistema.time.min;
	me->rwPdu->Field[2][4] = me->calendar.objRtc_sistema.time.hor;
	me->rwPdu->Field[2][5] = me->calendar.objRtc_sistema.time.sem;
	me->rwPdu->Field[2][6] = me->calendar.objRtc_sistema.date.dia;
	me->rwPdu->Field[2][7] = me->calendar.objRtc_sistema.date.mes;
	me->rwPdu->Field[2][8] = me->calendar.objRtc_sistema.date.ani;
}

