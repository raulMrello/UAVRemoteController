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
#include "dev_GUI_hsm.h"		// includes drv_IR.h

/****************************************************************************************************//**
 *                        PRIVATE TYPES
 ********************************************************************************************************/
/* object ID codes */
#define MENU_OBJ_WINDOW_1   	ID_WINDOW1
#define MENU_OBJ_BUTTON_1   	ID_OBJ_BUTTON_1
#define MENU_OBJ_BUTTON_2   	2
#define MENU_OBJ_BUTTON_3   	3
#define MENU_OBJ_BUTTON_4   	4
#define ID_LABEL				5
#define ID_PROGRESS_BAR			6


/* graphic private variables for  execution */
typedef struct{
	STATICTEXT  *label;
	PROGRESSBAR *progress;
	XCHAR 		*menuTitle;
	XCHAR 		*pLabel;
	uint8_t 	 iProgress;
	uint8_t 	 maxProgress;
	uint8_t 	*pData;
	uint8_t		 data[22];
	uint8_t 	 szData;
	uint8_t 	 pduReq[2];
	uint8_t 	 pduSize;
	XCHAR		*dlgTitleText;	
	XCHAR		*dlgBodyText;	
	XCHAR		*dlgLeftText;
	XCHAR		*dlgRightText;
	uint8_t		(*OnStartAction)(void*,char**,uint8_t*,uint8_t*,uint8_t**,uint8_t*);
	void 		(*OnLeftSelect)(void *);
	void 		(*OnRightSelect)(void *);
	uint8_t 	cmFlags;
}T_ComMenu;

static T_ComMenu *This;
static StateHandler GoToState;

/* cmflags types */
#define CMFLAGS_MUSTSTART	0
#define CMFLAGS_PENDING		1
#define CMFLAGS_MUSTEXIT	2
#define CMFLAGS_SEND_EOT	4
#define CMFLAGS_SHOWDIALOG	255

/* private routines */
static void StartWrite				(void *pThis);
static void StartRead				(void *pThis);
static void OnReceivedData			(void * pThis, uint8_t *pdata, uint16_t size);
static void OnSentData				(void * pThis, uint8_t *pdata, uint16_t size);
static void ExecutePendingActions	(void * pThis);

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     ComMenu_Create
 * @brief   Creates the user form and all the widgets contained on it. Even
 *			it initialize all graphics variables.
 *			If dlo == dro == 0 then no dialog will be shown, in stead it will
 *			NotifyTransition to tts[selectedOption]
 *
 * @param   pThis      	this object as void
 * @param	mt			Menubox title
 * @param	osa			On Start Action pointer callback
 * @param	mp			Max number of elements to be read/sent
 * @param   rw			Read or Write type of operation
 * @param	dtt			Dialog title text
 * @param	dbt			Dialog body text
 * @param	dlt			Dialog left text
 * @param	drt			Dialog right text
 * @param	dla			Dialog left Action callback
 * @param	dra			Dialog right Action callback
 * @param	gts			Statehandler to exit on error
 *
 * @return  n/a
 ****************************************************************************/
void ComMenu_Create(void * pThis, XCHAR * mt, uint8_t(*osa)(void*,char**,uint8_t*,uint8_t*,uint8_t**,uint8_t*), 
						uint8_t mp, uint8_t rw, XCHAR *dtt, XCHAR *dbt, XCHAR *dlt, XCHAR *drt, 
							void (*dla)(void *), void (*dra)(void *), StateHandler gts){


	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	// not used: WINDOW* pOBJ_WINDOW_1;
	GOLFree();

	// if no send callback and no gts, returns
	if((osa == 0 && gts == 0) || mp == 0){
		return;
	}
	// else try something
	if(osa == 0 && gts != 0){
		IBeepLong();
		NotifyTransition(gts);
		return;
	}
	// else alloc memory, create menu and launch send callback
	This = malloc(sizeof(T_ComMenu));
	if(This == 0){
		return;
	}
	//update pointer for future deallocation
	me->mustBeFreed = This;
	This->menuTitle = mt;
	This->OnStartAction = osa;
	This->maxProgress = mp;
	me->isReceiving = rw;
	This->dlgTitleText = dtt;
	This->dlgBodyText = dbt;
	This->dlgLeftText = dlt;
	This->dlgRightText = drt;
	This->OnLeftSelect = dla;
	This->OnRightSelect = dra;
	This->iProgress = 0;
	This->cmFlags = 0;
	This->pduReq[1] = 255;	//next prog to download, initially erased
	GoToState = gts;

	IBacklightForbid();
	IBacklightOff();

	WndCreate(MENU_OBJ_WINDOW_1,       		// ID
              0,0,GetMaxX(),GetMaxY(), 	// dimension
              WND_DRAW |WND_TITLECENTER,                	// will be dislayed after creation
              NULL, 
              This->menuTitle,	   				// set text 
              me->WindowsScheme);               // default GOL scheme 

	// Start trasnmission
	This->cmFlags = This->OnStartAction(pThis, &This->pLabel, &This->pduReq[0], &This->pduSize, &This->pData, &This->szData);
	me->isDataReady = TRUE;

	This->label = StCreate(ID_LABEL, 4, 20, GetMaxX()-4, 36, ST_CENTER_ALIGN | ST_UPDATE | ST_DRAW, This->pLabel, me->altScheme); 
	This->progress = PbCreate(ID_PROGRESS_BAR, 10, 40,GetMaxX()-10, 52, PB_DRAW, This->iProgress, This->maxProgress, me->altScheme);
	
}

/*************************************************************************//**
 * @fun     ComMenu_Display
 * @brief   Display screen updates and manages Flags.
 *
 * @param   pThis      	this object as void
 *
 * @return  n/a
 ****************************************************************************/
void ComMenu_Display(void * pThis){
	
	SetState(This->label, ST_DRAW);
	PbDraw(This->progress);

	ExecutePendingActions(pThis);
}

/*************************************************************************//**
 * @fun     ComMenu_Callback
 * @brief   Process keyboard events to real-time feedback
 *
 * @param   objMsg     	this object
 * @param	pObj		object who receives the message
  * @param	pMsg		pointer to the message to be processed
*
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
uint32_t ComMenu_Callback(void *pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	me->auxCounter = 0;
  	
  	switch(GetObjID(pObj)){
   		case MENU_OBJ_WINDOW_1:
			if (pMsg->type == TYPE_KEYBOARD){				
				switch (pMsg->param1){
				 	case KEY_DOWN:
					case KEY_UP:
					case KEY_ENTER:
					break;
					case KEY_CANCEL:
						IBeepLong();
						ICancelRemote();
						TRAN(GoToState);
				}	 
			}	
			if (pMsg->type == TYPE_IR){				
				uint8_t * pData;
				uint16_t  size;
				size = pMsg->uiEvent;	// get coded data size
				pData = (uint8_t*)((pMsg->param2 << 16)+(pMsg->param1)); // get coded data buffer address
				if(me->isReceiving){
					OnReceivedData(me, pData, size);
				}
				else{
					OnSentData(me, pData, size);
				}
			}	
		break;
	}        
	return 1;
}

/*************************************************************************//**
 * @fun     ExecutePendingActions
 * @brief   Process pending actions to carry out during IR communications
 *
 * @param   pThis      	this object as void
 *
 * @return  n/a
 ****************************************************************************/
static void ExecutePendingActions(void * pThis){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;

	// if pending and dataready confirmation, check for further data to send
	if(This->cmFlags == CMFLAGS_PENDING && me->isDataReady){
		// Request new trasnmission
		This->cmFlags = This->OnStartAction(pThis, &This->pLabel, &This->pduReq[0], &This->pduSize, &This->pData, &This->szData);
		if(This->cmFlags == CMFLAGS_SHOWDIALOG){
			This->cmFlags = CMFLAGS_SEND_EOT;
		}
		StSetText(This->label, This->pLabel);
	}

	if(This->cmFlags == CMFLAGS_SEND_EOT){
		uint8_t mac_pdu_EOT[2] = {7,0}; // 7 = PDU_EOT, 0 = relleno
		ISendRemote(mac_pdu_EOT, 2);
		This->cmFlags = CMFLAGS_SHOWDIALOG;
		return;
	}

	if(This->cmFlags == CMFLAGS_SHOWDIALOG){
		This->cmFlags = CMFLAGS_MUSTEXIT;
		ICancelRemote();
		IBacklightPermit();
		IBacklightOn();
		IBeepConfirm();

		DialogBoxEx_create( pThis,
							0, 
							This->dlgTitleText,
							This->dlgBodyText, 
							This->OnLeftSelect, 
							This->OnRightSelect, 
							0, 0,
							This->dlgLeftText,
							This->dlgRightText,
							GoToState,NULL);
		return;
	}

	// if send condition and hardware is ready...
	if(This->cmFlags == CMFLAGS_MUSTSTART && me->isDataReady && ICheckRemoteReady()){
		if(me->isReceiving){
			StartRead(pThis);
		}
		else{
			StartWrite(pThis);
		}
		return;
	}

}

/*************************************************************************//**
 * @fun     OnReceivedData
 * @brief   Process received data. Invoked by GUI_Interface
 *
 * @param   pThis      	this object as void
 *
 * @return  n/a
 ****************************************************************************/
static void OnReceivedData(void * pThis, uint8_t *pdata, uint16_t size){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	// check size to avoid buffer overrun
	if(size <= This->szData){
		// adhoc processing for program burst mode
		// if burst stream: *pada == PDU_PROGRAM = 1
		if((*pdata & 0x0F) == 1){
			// if program number is 255 then retrive burst information
			if(*(pdata+1) == 255 && *(pdata+2)<MAX_PROGS){
				This->pduReq[1] = *(pdata+2); 							// next program to download
				This->iProgress = This->maxProgress - (*(pdata+3));	   	// max progress for num_programs
			}
			// if program number is in range, retrieve next and save
			else if(*(pdata+1) < MAX_PROGS){
				// if reserved1 >0 and <MAX_PROGS retrieve program
				if(*(pdata+4) > 0 && *(pdata+4) < MAX_PROGS){
					This->pduReq[1] = *(pdata+4);
				}
				// else finish
				else{
					This->cmFlags = CMFLAGS_SEND_EOT;//CMFLAGS_SHOWDIALOG;
				}
				*(pdata+4) = 0;
				memcpy((char*)(This->pData + (22*(*(pdata+1)))), pdata, size);
			}
			// else error, then finish
			else{
				This->iProgress = This->maxProgress;
				This->cmFlags = CMFLAGS_SEND_EOT;//CMFLAGS_SHOWDIALOG;
			}
		}
		// process other non program packages
		else{
			memcpy(This->pData, pdata, size);
		}

	}
	me->isDataReady = 1;
	ExecutePendingActions(pThis);

}



/*************************************************************************//**
 * @fun     OnSentData
 * @brief   Notify sent data. Invoked by GUI_Interface
 *
 * @param   pThis      	this object as void
 *
 * @return  n/a
 ****************************************************************************/
static void OnSentData(void * pThis, uint8_t *pdata, uint16_t size){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	(void)pdata;
	(void)size;
	me->isDataReady = 1;
	ExecutePendingActions(pThis);

}

/*************************************************************************//**
 * @fun     StartWrite
 * @brief   Start communications for writting.
 *
 * @param   pThis      	this object as void
 *
 * @return  n/a
 ****************************************************************************/
static void StartWrite(void *pThis){	
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	This->iProgress++;
	PbSetPos(This->progress, This->iProgress);
	me->auxCounter = 0;
	me->isDataReady = 0;
	This->cmFlags = CMFLAGS_PENDING;
	ISendRemote(This->pData,This->szData);
}

/*************************************************************************//**
 * @fun     StartRead
 * @brief   Start communications for reading.
 *
 * @param   pThis      	this object as void
 *
 * @return  n/a
 ****************************************************************************/
static void StartRead(void *pThis){	
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	This->iProgress++;
	PbSetPos(This->progress, This->iProgress);
	me->auxCounter = 0;
	me->isDataReady = 0;
	This->cmFlags = CMFLAGS_PENDING;
	IReadRemote(This->pduReq, This->pduSize);
}
