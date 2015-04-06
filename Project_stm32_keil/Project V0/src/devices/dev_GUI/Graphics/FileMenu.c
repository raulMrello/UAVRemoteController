/*************************************************************************//**
 * @file    dev_GUI_State_SaveDevice.h
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
#define ID_FILENAME				5

/* graphic private variables for  execution */
typedef struct{
	STATICTEXT *lname;
	XCHAR 		sname[16];
	XCHAR 		stilt[16];
	XCHAR 		slabel[16];
	XCHAR *		path;
	XCHAR *		menuTitle;
	uint8_t	**	pData;
	uint16_t *	szData;
	XCHAR 		letra[2];
	uint8_t 	wordIndex;
	uint8_t		mustOverwrite;
	uint8_t 	cmFlags;
}T_FileMenu;

static T_FileMenu *	This;
static StateHandler GoToState;

/* cmflags types */
#define CMFLAGS_ERRSAVING		1	
#define CMFLAGS_FILESAVED		2
#define CMFLAGS_FILEPRELOADED	4
#define CMFLAGS_MUSTEXIT		8
#define CMFLAGS_MUSTOVERWRITE	16	


static void SaveFile(void *pThis);
static void Exit(void *pThis);

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     FileMenu_Create
 * @brief   Creates the user form and all the widgets contained on it. Even
 *			it initialize all graphics variables.
 *
 * @param   pThis      	this object as void
 * @param	mt			Menubox title
 * @param	filename	preselected filename to save
 * @param	pData		pointer to the list of pointers to data buffers to save
 * @param	szData		pointer to the list of data buffer sizes 
 * @param   path		full path for file saving
 * @param	gts			Statehandler to exit on error
 *
 * @return  n/a
 ****************************************************************************/
void FileMenu_Create(void * pThis, XCHAR * mt, char * filename, uint8_t **pData, uint16_t *pSize, XCHAR * path, uint8_t ovrWrt, StateHandler gts){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	// not used: WINDOW* pOBJ_WINDOW_1;
	GOLFree();
	// else try something
	if(gts == 0){
		return;
	}
	// else alloc memory, create menu and launch send callback
	This = malloc(sizeof(T_FileMenu));
	if(This == 0){
		return;
	}
	//update pointer for future deallocation
	me->mustBeFreed = This;
	This->menuTitle = mt;
	This->pData = pData;
	This->szData = pSize;
	This->path = path;
	This->mustOverwrite = ovrWrt;
	This->cmFlags = 0;
	GoToState = gts;

	WndCreate(MENU_OBJ_WINDOW_1,       		// ID
              0,0,GetMaxX(),GetMaxY(), 	// dimension
              WND_DRAW |WND_TITLECENTER,                	// will be dislayed after creation
              NULL, 
              This->menuTitle,	   				// set text 
              me->WindowsScheme);               // default GOL scheme 

	if(filename == 0){
		This->wordIndex = 0;
		strcpy(This->letra, "A");
		strcpy(This->sname, "A");
		strcpy(This->stilt, "");
	}
	else{
		This->cmFlags = CMFLAGS_FILEPRELOADED;
		strcpy(This->sname, filename);
	}
	strcpy(This->slabel, This->sname);
	This->lname = StCreate(ID_FILENAME, 4, 20, GetMaxX()-4, 36, ST_UPDATE | ST_DRAW, This->slabel, me->altScheme); 
	me->statCounter = 0;
}

/*************************************************************************//**
 * @fun     FileMenu_Display
 * @brief   Display screen updates and manages Flags.
 *
 * @param   pThis      	this object as void
 *
 * @return  n/a
 ****************************************************************************/
void FileMenu_Display(void * pThis){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	if(This->cmFlags == CMFLAGS_MUSTEXIT){
		IBeepLong();
		NotifyTransition(GoToState);		
		return;
	}
	else if(This->cmFlags == CMFLAGS_FILEPRELOADED){		
		if(This->mustOverwrite || !(FileSystem_CheckFile(This->path, This->sname))){
			SaveFile(pThis);
		}
		else {
			DialogBoxEx_create(	pThis,
								0, 
								(XCHAR*)TEXTS[TXT_Ya_existe][me->language],
								(XCHAR*)TEXTS[TXT_Sobreescribir_][me->language], 
								NULL, 
								SaveFile, 
								0, 0,
								(XCHAR*)TEXTS[TXT_NO][me->language], 
								(XCHAR*)TEXTS[TXT_SI][me->language],
								GoToState,NULL);							
		}
		This->cmFlags = 0;
		return;
	}

	else if(This->cmFlags == (CMFLAGS_MUSTEXIT | CMFLAGS_FILESAVED)){
		if(++me->statCounter >= 16){
			IBeepLong();
			NotifyTransition(GoToState);		
		}
		return;
	}
	if(This->cmFlags & CMFLAGS_ERRSAVING){
		DialogBoxEx_create(	pThis,
							0, 
							(XCHAR*)TEXTS[TXT_Error_guardando][me->language],
							(XCHAR*)TEXTS[TXT_Reintentar_][me->language], 
							NULL, 
							SaveFile, 
							0, 0,
							(XCHAR*)TEXTS[TXT_NO][me->language], 
							(XCHAR*)TEXTS[TXT_SI][me->language],
							GoToState,NULL);
	}
	else if(This->cmFlags & CMFLAGS_FILESAVED){
		strcpy(This->slabel, (XCHAR*)TEXTS[TXT_Archivo_guardado_][me->language]);
		This->cmFlags |= CMFLAGS_MUSTEXIT;
		me->statCounter = 0;
	}
	else{
		me->statCounter = (me->statCounter+1)&7;
		if(me->statCounter < 4){
			strcpy(This->slabel, This->sname);	
		}
		else{
			strcpy(This->slabel, This->stilt);	
		}
	}
	SetState(This->lname, ST_DRAW);
}

/*************************************************************************//**
 * @fun     FileMenu_Callback
 * @brief   Process keyboard events to real-time feedback
 *
 * @param   objMsg     	this object
 * @param	pObj		object who receives the message
  * @param	pMsg		pointer to the message to be processed
*
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
uint32_t FileMenu_Callback(void *pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg){
	// not used:  LISTBOX* pLb;
  	// not used:  LISTITEM* l_item;
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
	me->auxCounter = 0;
	me->statCounter = 0;
  	switch(GetObjID(pObj)){
   		case MENU_OBJ_WINDOW_1:
			if (pMsg->type == TYPE_KEYBOARD){				
				switch (pMsg->param1){
				 	case KEY_DOWN:
						if(This->letra[0] == 'A')		This->letra[0] = '9';
						else if(This->letra[0] == '0')	This->letra[0] = '_';
						else if(This->letra[0] == '_')	This->letra[0] = '-';
						else if(This->letra[0] == '-')	This->letra[0] = '.';
						else if(This->letra[0] == '.')	This->letra[0] = 'z';
						else if(This->letra[0] == 'a')	This->letra[0] = 'Z';
						else							This->letra[0]--;
						This->sname[This->wordIndex] = 	This->letra[0];
					break;
					case KEY_UP:
						if(This->letra[0] == 'Z')		This->letra[0] = 'a';
						else if(This->letra[0] == 'z')	This->letra[0] = '.';
						else if(This->letra[0] == '.')	This->letra[0] = '-';
						else if(This->letra[0] == '-')	This->letra[0] = '_';
						else if(This->letra[0] == '_')	This->letra[0] = '0';
						else if(This->letra[0] == '9')	This->letra[0] = 'A';
						else							This->letra[0]++;
						This->sname[This->wordIndex] = 	This->letra[0];
					break;
					case KEY_RIGHT:
						This->wordIndex++;
						if(This->wordIndex < 5){
							This->letra[0] = 'A';
							strcpy(This->stilt,This->sname);
							strcat(This->sname, This->letra);
							break;
						}
						else{
							This->wordIndex = 4;
							break;
						}
						// Si el nombre tiene 10 caracters, continúa en KEY_OK para salvarlo
					case KEY_ENTER:
						if(FileSystem_CheckFile(This->path, This->sname)){
							DialogBoxEx_create(	pThis,
												0, 
												(XCHAR*)TEXTS[TXT_Ya_existe][me->language],
												(XCHAR*)TEXTS[TXT_Sobreescribir_][me->language], 
												NULL, 
												SaveFile, 
												0, 0,
												(XCHAR*)TEXTS[TXT_NO][me->language], 
												(XCHAR*)TEXTS[TXT_SI][me->language],
												GoToState,NULL);							
						}
						else{
							SaveFile(pThis);
						}
						break;
					case KEY_LEFT:
						if(This->wordIndex > 0){
							This->sname[This->wordIndex] = 0;
							This->wordIndex--;
							This->stilt[This->wordIndex]=0;
							break;
						}
						// SI wordIndex == 0 continúa en KEY_CANCEL con el ExitBox.
					case KEY_CANCEL:
						DialogBoxEx_create(	pThis, 
											0, 
											(XCHAR*)This->sname,
											(XCHAR*)TEXTS[TXT_Salir_sin_guardar][me->language], 
											NULL, 
											Exit, 
											0, 0,
											(XCHAR*)TEXTS[TXT_NO][me->language], 
											(XCHAR*)TEXTS[TXT_SI][me->language],
											GoToState,NULL);
						break;
				}	 
			}	
		break;
	}        
	return 1;
}

/*************************************************************************//**
 * @fun     Exit
 * @brief   Exit from this menu
 *
 * @param   pThis      	this object as void
 *
 * @return  n/a
 ****************************************************************************/
static void Exit(void *pThis){
	This->cmFlags = CMFLAGS_MUSTEXIT;
}

/*************************************************************************//**
 * @fun     SaveFile
 * @brief   Save file adding pData buffers
 *
 * @param   pThis      	this object as void
 *
 * @return  n/a
 ****************************************************************************/
static void SaveFile(void *pThis){
	if(FileSystem_SaveFile(This->path, This->sname, (char**)This->pData, This->szData, 1) != 0){
		This->cmFlags |= CMFLAGS_ERRSAVING;
		return;
	}
	This->cmFlags &= ~CMFLAGS_ERRSAVING;
	This->cmFlags |= CMFLAGS_FILESAVED;
}


