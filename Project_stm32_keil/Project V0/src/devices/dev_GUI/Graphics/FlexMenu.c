/*************************************************************************//**
 * @file    FlexMenu.c
 * @mcu		STM32F1x
 * @brief   Complex Menu with listbox and dialog box to confirm option selection
 * @date    15.06.2010
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
#define MENUBOX_WND_ID   	ID_WINDOW1
#define Memory_OBJ_BUTTON_1   	ID_OBJ_BUTTON_1
#define Memory_OBJ_BUTTON_2   	2
#define Memory_OBJ_BUTTON_3   	3
#define Memory_OBJ_BUTTON_4   	4
#define MENUBOX_LBX_ID			5

#define MAX_LIST_TEXT_SIZE		21
#define TOKENS_FROM_FILE		1

/* event handlers */
static void (*OnLeftAction)(void *pThis);
static void (*OnRightAction)(void *pThis);
static void (*OnSelectedAccept)(void *pThis, char * pText);

static StateHandler GoToState;
static StateHandler * TranState;

/* graphic private variables for dialogbox execution */
static XCHAR * menuTitle;
static XCHAR * lbxSource;
static XCHAR * dialogTitle;
static XCHAR * dialogBody;
static XCHAR * dialogLeftOption;
static XCHAR * dialogRightOption;
static LISTBOX* pOBJ_listbox;
static uint8_t	mfeFlags = 0;
static uint16_t strFwd = 0;
static uint16_t strRew = 0;
static uint16_t strNow = 0;


static XCHAR   lbxText[4*MAX_LIST_TEXT_SIZE];

static uint8_t GetTokForward(uint16_t *posFwd, char *dest, char *source, uint8_t numtok, uint8_t isFile);
static uint8_t GetTokRewind(uint16_t *posRew, char *dest, char *source, uint8_t numtok, uint8_t isFile);
static uint8_t CheckTokForward(char *source, uint8_t numtok, uint8_t isFile);
/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/



/*************************************************************************//**
 * @fun     ReloadGraphics
 * @brief   (private) Reloads graphic objects to the screen
 *
 * @param   pThis     	GUI reference
 * @param   pLbxText    listbox options
 * @param   selected    preselected option
 *
 * @retval  0:			token error
 * @retval  >0:			number of read tokens
 ****************************************************************************/
static void ReloadGraphics(void *pThis, XCHAR *pLbxText, uint8_t selected){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;

	GOLFree();

	WndCreate(MENUBOX_WND_ID,       		// ID
              0,0,GetMaxX(),GetMaxY(), 	// dimension
              WND_DRAW |WND_TITLECENTER,                	// will be dislayed after creation
              NULL, //(void*)&OrbisMemoryInvert,               	// icon
              menuTitle,	   				// set text 
              me->WindowsScheme);               // default GOL scheme 
   
  	pOBJ_listbox = LbCreate(MENUBOX_LBX_ID,        	                // ID
              0,15,GetMaxX(),GetMaxY(),            // dimension
              LB_DRAW|LB_FOCUSED|LB_SINGLE_SEL,   // will be dislayed after creation
              pLbxText,
              me->altScheme);    

			  
	LbSetFocusedItem(pOBJ_listbox,selected);
	LbChangeSel(pOBJ_listbox,pOBJ_listbox->pFocusItem);		  									 

}

/*************************************************************************//**
 * @fun     FlexMenu_Create
 * @brief   Creates the user form and all the widgets contained on it. Even
 *			it initialize all graphics variables.
 *			If dlo == dro == 0 then no dialog will be shown, in stead it will
 *			NotifyTransition to tts[selectedOption]
 *
 * @param   pThis      	this object as void
 * @param	mt			Menubox title
 * @param	lbo			Listbox option texts
 * @param	ftr			File to read from
 * @param	osa			On Selected Action pointer
 * @param	db			Dialog body text
 * @param	dlo			Dialog left button text
 * @param	dro			Dialog right button text
 * @param	ola			Dialog left button action callback
 * @param	ora			Dialog right button action callback
 * @param	gts			Pointer to the next state to go on exit
 * @param	tts			Pointer to the list of states to TRAN by listbox option
 *
 * @return  n/a
 ****************************************************************************/
void FlexMenu_Create(	void * pThis, XCHAR * mt, XCHAR *lbo, XCHAR *ftr, void(*osa)(void*,char*), XCHAR *db, XCHAR *dlo, 
						 XCHAR *dro, void(*ola)(void*), void(*ora)(void*), StateHandler gts,	StateHandler * tts){

	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	
	mfeFlags = 0;
	menuTitle = mt;
	dialogBody = db;
	dialogLeftOption = dlo;
	dialogRightOption = dro;
	OnLeftAction = ola;
	OnRightAction = ora;
	OnSelectedAccept = osa;
	GoToState = gts;
	TranState = tts;
	me->selectedOption = 0;		       
	
	// check if listbox options comes from flash array or filesystem file
	if(lbo == 0 && ftr != 0){
		mfeFlags |= TOKENS_FROM_FILE;
		lbxSource = ftr;
	}
	else{
		mfeFlags &= ~TOKENS_FROM_FILE;
		lbxSource = lbo;
	}
	// get first 4 tokens. Initialize rew-fwd position counters
	strRew = 0;
	strFwd = 0;
	strNow = 0;
	GetTokForward(&strFwd, (XCHAR*)lbxText, lbxSource, 4, (mfeFlags & TOKENS_FROM_FILE));		
								
	// load graphics objects
	ReloadGraphics(pThis, lbxText, 0);
}


/*************************************************************************//**
 * @fun     FlexMenu_Callback
 * @brief   (public) Executes all the pending operations according with the reception
 *			of the GOL_MSG preprocessed by this state.
 *
 * @param   pThis     	GUI reference
 * @param   objMsg     	this object
 * @param	pObj		object who receives the message
 * @param	pMsg		pointer to the message to be processed
 *
 * @return  1:if finished and[if required] transition to other states.
 ****************************************************************************/
uint32_t FlexMenu_Callback(void *pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg){
	T_DEV_GUI *me = (T_DEV_GUI*)pThis;
  	int retval;
  	uint8_t number_items;
  	OBJ_HEADER  *header_obj;
	XCHAR *dt;
	// if no memory available, then exits
	if(strncmp(lbxSource+1, (XCHAR *)TEXTS[TXT_Memoria_vacia_][me->language],(strlen(lbxSource)-2))==0){
		IBeepLong();
		TRAN(GoToState);// Pantalla anterior
	}
  	switch(GetObjID(pObj)){
   		case MENUBOX_WND_ID:
			if (pMsg->type == TYPE_KEYBOARD){				
			   	header_obj = GOLFindObject(MENUBOX_LBX_ID);	
				switch (pMsg->param1){
				 	case KEY_DOWN:
						number_items = LbGetCount((LISTBOX*)header_obj);
						retval = LbGetFocusedItem((LISTBOX*)header_obj);
						retval++;
						if (retval >= number_items){
							if(number_items == 4){
								if(me->selectedOption >= 3 && CheckTokForward((char*)(lbxSource+strFwd), 4, (mfeFlags & TOKENS_FROM_FILE)) < number_items){
									return 1;
								}
								// get next 4 tokens. Initialize rew-fwd position counters
								strNow = strFwd;
								number_items = GetTokForward(&strFwd, (XCHAR*)lbxText, (char*)(lbxSource+strFwd), 4, (mfeFlags & TOKENS_FROM_FILE));		
								if((number_items > 0 && me->selectedOption < 4) || (number_items == 4 && me->selectedOption >= 4)){
									strRew = strNow;
									//update listbox content and selected option
									ReloadGraphics(pThis, lbxText, 0);
									retval--;
									me->selectedOption++;								
								}
							}
							else{
								retval = number_items-1;
							}
						}
						else{
							me->selectedOption++;
						}
						LbSetFocusedItem((LISTBOX*)header_obj,retval);
						LbChangeSel((LISTBOX*)header_obj,((LISTBOX*)header_obj)->pFocusItem);
						GOLRedraw((LISTBOX*)header_obj);
					break;

					case KEY_UP:						
						retval = LbGetFocusedItem((LISTBOX*)header_obj);
						retval--;
						if (retval<0){
							retval=0;
							// get prev 4 tokens. Initialize rew-fwd position counters
							strNow = strRew;
							number_items = GetTokRewind(&strRew, (XCHAR*)lbxText, (char*)(lbxSource+strNow), 4, (mfeFlags & TOKENS_FROM_FILE));		
							if(number_items > 0){
								strFwd = strNow;
								//update listbox content and selected option
								ReloadGraphics(pThis, lbxText, 0);
								me->selectedOption--;
							}
							else if(me->selectedOption == 1){	// fix first element correction
								me->selectedOption = 0;
							}
						}
						else{
							me->selectedOption--;
						}
						LbSetFocusedItem((LISTBOX*)header_obj,retval);
						LbChangeSel((LISTBOX*)header_obj,((LISTBOX*)header_obj)->pFocusItem);
						GOLRedraw((LISTBOX*)header_obj);
					break;
					case KEY_ENTER:
						me->selectedText = pOBJ_listbox->pFocusItem->pText;
						// if no dialog must be shown ensuring no pointer lost.
						if(dialogLeftOption == 0 && dialogRightOption == 0){
							if(OnSelectedAccept != 0){
								OnSelectedAccept(pThis, pOBJ_listbox->pFocusItem->pText);
								return 1;
							}
							else{
								TRAN(TranState[me->selectedOption]);
							}
						}
						// else dialog is present
						dt = dialogTitle;
						if(dialogTitle == 0){
							dt = pOBJ_listbox->pFocusItem->pText;
						}
						DialogBoxEx_create( pThis,0, 
											dt,
											dialogBody, 
											OnLeftAction, 
											OnRightAction, 
											0, 0,
											dialogLeftOption,
											dialogRightOption,
											GoToState,NULL);
					break;
					case KEY_CANCEL:
						IBeepLong();
						TRAN(GoToState);// Pantalla anterior
				}	 
			}	
		break;
	}        
	return 1;
}


/*************************************************************************//**
 * @fun     GetTokForward
 * @brief   (private) Gets <numtok> consecutive texts from a given index. It can 
 *			get it from flash array or filesystem file.
 *
 * @param   posFwd     	(out) position of <source> from which get forward elements
 * @param   dest     	(out) destination buffer with listbox format
 * @param   source     	original buffer with no format
 * @param	numtok		number of tokens to get
 * @param	isFile		flag to check if get from filesystem file or not
 *
 * @retval  0:			token error
 * @retval  >0:			number of read tokens
 ****************************************************************************/
static uint8_t GetTokForward(uint16_t *posFwd, char *dest, char *source, uint8_t numtok, uint8_t isFile){
	uint8_t nTok,i,j;
	char * pRead;

	/* 
		IMPORTANTE: En el caso de recoger datos desde el filesystem, habría que
		desarrollar el if siguiente en lugar de devolver 0.
	*/
	if(isFile)
		return 0;

	// set read pointer over the first selected element and check if its invalid
	pRead = source;
	if(*pRead != ' '){
		return 0;
	}
	
	// get next tokens and format as: " TOK1\n TOK2\n TOK3\n TOK4\n"
	nTok = 0;
	i = 0;
	j = 0;
	while(nTok < numtok){
		dest[i] = pRead[j];
		if(dest[i] == 0){			
			break;
		}
		if(dest[i] == '\n'){			
			nTok++;
			if(nTok == 1){
				// update position for further forward
				*posFwd += (i+1);
			}
		}
		i++;			
		j++;
	}
	if(nTok == numtok){
		dest[i] = 0;
	}
	return(nTok);
}


/*************************************************************************//**
 * @fun     CheckTokForward
 * @brief   (private) Checks <numtok> consecutive texts from a given index. It can 
 *			check it from flash array or filesystem file.
 *
 * @param   source     	original buffer with no format
 * @param	numtok		number of tokens to get
 * @param	isFile		flag to check if get from filesystem file or not
 *
 * @retval  0:			token error
 * @retval  >0:			number of read tokens
 ****************************************************************************/
static uint8_t CheckTokForward(char *source, uint8_t numtok, uint8_t isFile){
	uint8_t nTok,i,j;
	char * pRead;

	/* 
		IMPORTANTE: En el caso de recoger datos desde el filesystem, habría que
		desarrollar el if siguiente en lugar de devolver 0.
	*/
	if(isFile)
		return 0;

	// set read pointer over the first selected element and check if its invalid
	pRead = source;
	if(*pRead != ' '){
		return 0;
	}
	
	// get next tokens and format as: " TOK1\n TOK2\n TOK3\n TOK4\n"
	nTok = 0;
	i = 0;
	j = 0;
	while(nTok < numtok){
		if(pRead[j] == 0){			
			break;
		}
		if(pRead[j] == '\n'){			
			nTok++;
		}
		i++;			
		j++;
	}
	return(nTok);
}


/*************************************************************************//**
 * @fun     GetTokRewind
 * @brief   (private) Gets <numtok> consecutive texts from a given index. It can 
 *			get it from flash array or filesystem file.
 *
 * @param   posRew     	(out) position of <source> from which get rewinded elements
 * @param   dest     	(out) destination buffer with listbox format
 * @param   source     	original buffer with no format
 * @param	numtok		number of tokens to get
 * @param	isFile		flag to check if get from filesystem file or not
 *
 * @retval  0:			token error
 * @retval  >0:			number of read tokens
 ****************************************************************************/
static uint8_t GetTokRewind(uint16_t *posRew, char *dest, char *source, uint8_t numtok, uint8_t isFile){
	uint8_t nTok,i,j;
	uint16_t posAux = (*posRew - 1);
	char * pRead;
	char * pEnd;

	/* 
		IMPORTANTE: En el caso de recoger datos desde el filesystem, habría que
		desarrollar el if siguiente en lugar de devolver 0.
	*/
	if(isFile)
		return 0;

	// set read and end pointers over the first selected element and check if token doesnt exist and return error code. 
	nTok = 0;
	pEnd = (char*)(source-1);
	pRead = (char *)(source-2);
	
	// if preambled stream is not valid, exit
	if(*pEnd == 0){
		return 0;
	}
	while(*pRead != 0){		
		pRead--;
		posAux--;
		if(*pRead == '\n'){
			pRead++;
			break;
		}		
	}
	//check if token is the first of the stream and update pointer
	if(posAux == 0 && *pRead == 0){
		pRead++;
	}
	// check if token doesnt valid and return error code
	if(*pRead != ' '){
		return 0;
	}

	// save position
	*posRew -= (uint16_t)(source-pRead);
	
	// get next tokens and format as: " TOK1\n TOK2\n TOK3\n TOK4\n"
	nTok = 0;
	i = 0;
	j = 0;
	while(nTok < numtok){
		dest[i] = pRead[j];
		if(dest[i] == 0){			
			break;
		}
		if(dest[i] == '\n'){			
			nTok++;
		}
		i++;			
		j++;
	}
	if(nTok == numtok){
		dest[i] = 0;
	}
	return(nTok);
}
