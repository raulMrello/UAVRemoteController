/*************************************************************************//**
 * @file    dev_GUI_State_StoredDevice.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine implementation for StoredDevice form.
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


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     GUI_State_StoredDevice
 * @brief   StoredDevice state must show a StoredDevice list with all the functions allowable
 *			by the device
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GUI_State_StoredDevice(Object* pThis, Event const * e){
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
#define MAX_LIST_TEXT_SIZE				21

static XCHAR *Options_StoredDevice_txt;
static uint8_t numFiles = 0;

static void OnDeleteDevice(void *pThis);
static void OnSendDevice(void *pThis);

/*************************************************************************//**
 * @fun     OnFormCreate
 * @brief   Creates the user form and all the widgets contained on it. Even
 *			it initialize all graphics variables.
 ****************************************************************************/
static void OnFormCreate(void * pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	// not used: WINDOW* pOBJ_WINDOW_1;
	me->rwPdu = &commPdu;
	me->rwPdu->progs = &progs[0][0];

	// get number of files into DEVICE folder to allocate memory
	numFiles = FileSystem_ListDevices(NULL);
	if(numFiles == 0){
		// if no memory assigned, then exit. Else continue
		Options_StoredDevice_txt = malloc(MAX_LIST_TEXT_SIZE);
		if(Options_StoredDevice_txt == 0){
			NotifyTransition(GUI_State_Devices);
			return;
		}
		strcpy(Options_StoredDevice_txt, " ");
		strcat(Options_StoredDevice_txt, (XCHAR *)TEXTS[TXT_Memoria_vacia_][me->language]);
		strcat(Options_StoredDevice_txt, "\n");
	}
    else{
		// if no memory assigned, then exit. Else continue
		Options_StoredDevice_txt = malloc(numFiles * MAX_LIST_TEXT_SIZE);
		if(Options_StoredDevice_txt == 0){
			NotifyTransition(GUI_State_Devices);
			return;
		}
		// load devices into the preallocated memory
		numFiles = FileSystem_ListDevices(Options_StoredDevice_txt);
	}
	
	// create FlexMenu assigning params
	FlexMenu_Create(	pThis, 								
						(XCHAR *)TEXTS[TIT_EQUIPOS_GUARDADOS][me->language], 
						(XCHAR *)Options_StoredDevice_txt, 
						(XCHAR *)0,
						NULL, 
						(XCHAR *)TEXTS[TXT_Opciones_][me->language], 
						(XCHAR *)TEXTS[TXT_Borrar][me->language], 
						(XCHAR *)TEXTS[TXT_Volcar][me->language], 
						OnDeleteDevice,
						OnSendDevice,
						GUI_State_Devices,
						(StateHandler *)NULL);
}

/*************************************************************************//**
 * @fun     OnFormDisplay
 * @brief   Displays all the information required in this state
 ****************************************************************************/
static void OnFormDisplay(void * pThis){
	(void)pThis;
}

/*************************************************************************//**
 * @fun     OnDeleteDevice
 * @brief   Handles ENTER pressing button, according with the deepness status.
 * @param	pThis	GUI object
 ****************************************************************************/
static void OnDeleteDevice(void *pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	char filename[16];
	char *eof;
	strcpy(filename, (me->selectedText+1));
	eof = strchr(filename, '\n');
	if(eof != 0){
		*eof = 0;
	}
	FileSystem_DeleteDevice(filename);
	// reload graphics after deleting
	free(Options_StoredDevice_txt);
	OnFormCreate(pThis); 
}

/*************************************************************************//**
 * @fun     OnSendDevice
 * @brief   Read device param content and tran to Send state.
 * @param	pThis	GUI object
 ****************************************************************************/
static void OnSendDevice(void *pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	char filename[16];
	char *eof;
	char *vectors[3];
	uint16_t sizes[3];
	strcpy(filename, (me->selectedText+1));
	eof = strchr(filename, '\n');
	if(eof != 0){
		*eof = 0;
	}
	vectors[0] = (char*)&me->rwPdu->Field[0][0];
	sizes[0] = 4*22;
	vectors[1] = (char*)me->rwPdu->progs;
	sizes[1] = MAX_PROGS*22;
	vectors[2] = (char*)0;
	sizes[2] = 0;
	FileSystem_ReadDevice(filename, vectors, sizes);
	NotifyTransition(&GUI_State_SendDevice);
}

