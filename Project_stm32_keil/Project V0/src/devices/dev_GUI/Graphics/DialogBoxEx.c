/*************************************************************************//**
 * @file    DialogBoxEx.c
 * @mcu		STM32F1x
 * @brief   Dialogbox implementation
 * @date    28.09.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "dev_GUI_hsm.h"

/****************************************************************************************************//**
 *                        PRIVATE TYPES
 ********************************************************************************************************/
/* object ID codes */
#define OBJ_WINDOW_1		ID_WINDOW1
#define DB_BUTTON_RIGHT		ID_OBJ_BUTTON_1
#define DB_BUTTON_LEFT	  	2
#define OBJ_STATICTEXT_1 	6

/* event handlers */
static uint32_t DialogBoxEx_callback(void *pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg);
static void 	DialogBoxEx_display	(void *pThis);
static void 	DialogBoxEx_remove	(void *pThis);
static void (*fun_right)(void *pThis) 	= NULL;
static void (*fun_esc)(void *pThis) 	= NULL;
static void (*fun_left)	(void *pThis)	= NULL;
static void (*fun_up)(void *pThis) 	= NULL;
static void (*fun_down)(void *pThis) 	= NULL;
static StateHandler GoToState = NULL;
/* graphic private variables for dialogbox execution */
static OBJ_HEADER *dbListSaved;
void *ptr_prev_callback = NULL;
void *ptr_prev_display = NULL;
static uint8_t tilt=0;
STATICTEXT * stText;


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     DialogBoxEx_create
 * @brief   Creates a basic dialogbox
 *
 * @param   pThis      		this object as void
 * @param	type			not used. Allows different behaviours.
 * @param	text_w			Dialog title
 * @param	text			Dialog body text
 * @param	ptr_fun_left	Handler of Left button pressing
 * @param	ptr_fun_right	Handler of Right button pressing
 * @param	leftText		Text for left button
 * @param	rightText		Text for right button
 * @param	gts				state handler on ESC button pressing
 * @param	eef				execute exit function on ESC button
 *
 * @return  n/a
 ****************************************************************************/
void DialogBoxEx_create(	void *pThis, 
							uint8_t type, 
							XCHAR *text_w, 
							XCHAR *text, 
							void (*ptr_fun_left)(void *), 
							void (*ptr_fun_right)(void *),
							void (*ptr_fun_up)(void *), 
							void (*ptr_fun_down)(void *),
							XCHAR *leftText,
							XCHAR *rightText,
							StateHandler gts,
							void (*ptr_fun_esc)(void *)){

	T_DEV_GUI * me = (T_DEV_GUI*)pThis;
	
	// salvamos la pantalla actual (solo funiones Callback y display)
	ptr_prev_callback = me->OnCallbackForm;	
	me->OnCallbackForm = (void*)DialogBoxEx_callback;

	ptr_prev_display = me->OnDisplayForm;
	me->OnDisplayForm = (void*)DialogBoxEx_display;

	fun_up = ptr_fun_up;
	fun_down = ptr_fun_down;
	fun_right = ptr_fun_right;
	fun_left = ptr_fun_left;
	fun_esc  = ptr_fun_esc;
	GoToState = gts;

	// Guardamos la lista de objetos gráficos y creamos nueva lista
	dbListSaved = GOLGetList();
	GOLNewList();

	// Windows id
	WndCreate(OBJ_WINDOW_1,15,7,112,58,WND_DRAW | WND_TITLECENTER,NULL,text_w,me->WindowsScheme);
	// Statictext
	stText = StCreate(OBJ_STATICTEXT_1,17,22,110,32,ST_CENTER_ALIGN | ST_DRAW,text,me->altScheme);

	// Buttons Acepta/cancela
	BtnCreate(DB_BUTTON_LEFT,20,35,61,55,0,16384,NULL,leftText ,me->altScheme);
	BtnCreate(DB_BUTTON_RIGHT,66,35,107,55,0,BTN_PRESSED|BTN_DRAW,NULL,rightText ,me->altScheme);

	tilt = 0;
}	

/*************************************************************************//**
 * @fun     DialogBoxEx_display
 * @brief   Drawing common implementation
 *
 * @param   pThis      		this object as void
 *
 * @return  n/a
 ****************************************************************************/
void DialogBoxEx_display(void *pThis){
	BUTTON *pB_acepta;
	BUTTON *pB_cancel;

	pB_cancel = (BUTTON*)GOLFindObject(DB_BUTTON_LEFT);
	pB_acepta = (BUTTON*)GOLFindObject(DB_BUTTON_RIGHT);
	
	tilt = (uint8_t)((tilt+1)& 0x7); // values between 0..7
	if(tilt < 4){
		if(GetState (pB_cancel,BTN_PRESSED)){
			SetState(pB_cancel,BTN_DRAW);
		}
		else{
			SetState(pB_acepta,BTN_DRAW);
		}
	}
	else{
		if(GetState (pB_cancel,BTN_PRESSED)){
			SetState(pB_cancel,BTN_HIDE);
		}
		else{
			SetState(pB_acepta,BTN_HIDE);
		}
	}
}

/*************************************************************************//**
 * @fun     DialogBoxEx_callback
 * @brief   Message handling common implementation
 *
 * @param   pThis      		this object as void
 * @param	pObj			Object handling the message
 * @param	pMsg			Pointer to the message in process
 *
 * @return  0: if message processing is finished
 ****************************************************************************/
uint32_t DialogBoxEx_callback(void *pThis, uint32_t objMsg, OBJ_HEADER *pObj, GOL_MSG *pMsg){
	BUTTON *pB_acepta;
	BUTTON *pB_cancel;

	pB_cancel = (BUTTON*)GOLFindObject(DB_BUTTON_LEFT);
	pB_acepta = (BUTTON*)GOLFindObject(DB_BUTTON_RIGHT);
	// check object which is processing the message
	switch(GetObjID(pObj)) {
		case DB_BUTTON_RIGHT:
			if (pMsg->type == TYPE_KEYBOARD){	   	
				switch (pMsg->param1){
			 		case KEY_UP:
						if(fun_up != 0){
							fun_up(pThis);
							StDraw(stText);

						}
						break;
					case KEY_DOWN:
						if(fun_down != 0){
							fun_down(pThis);
							StDraw(stText);
						}
						break;
					case KEY_LEFT:
					case KEY_RIGHT:
						 // set PRESSED CANCEL
						 if(GetState (pB_acepta,BTN_PRESSED)){
						 	SetState(pB_cancel,BTN_PRESSED|BTN_DRAW);
						 	ClrState(pB_acepta,BTN_PRESSED);
						 	SetState(pB_acepta,BTN_DRAW);
						}
						break;

					case KEY_CANCEL:
						DialogBoxEx_remove(pThis);
						// execute ESC function
						if(fun_esc != 0){
							fun_esc(pThis);
						}					
						if(GoToState != NULL){
							NotifyTransition(GoToState);
						}
						break;
					
					case KEY_ENTER:
						DialogBoxEx_remove(pThis);
						if(fun_right != NULL){	
							fun_right(pThis);	// lanzamos la funcion
							fun_right = NULL;
							fun_left = NULL;
						}
						break;
			 	}
			}
			break;

		case DB_BUTTON_LEFT:
			if (pMsg->type == TYPE_KEYBOARD){	   	
				switch (pMsg->param1){
			 		case KEY_UP:
						if(fun_up != 0){
							fun_up(pThis);
							StDraw(stText);
						}
						break;
					case KEY_DOWN:
						if(fun_down != 0){
							fun_down(pThis);
							StDraw(stText);
						}
						break;
			 		case KEY_LEFT:
					case KEY_RIGHT:
						 // set PRESSED CANCEL
						if(GetState (pB_cancel,BTN_PRESSED)){
						 	SetState(pB_acepta,BTN_PRESSED|BTN_DRAW);
							 ClrState(pB_cancel,BTN_PRESSED);
							 SetState(pB_cancel,BTN_DRAW);
						}
						break;

					case KEY_CANCEL:
						DialogBoxEx_remove(pThis);
						// execute ESC function
						if(fun_esc != 0){
							fun_esc(pThis);
						}					
						if(GoToState != NULL){
							NotifyTransition(GoToState);
						}
						break;

					case KEY_ENTER:
						DialogBoxEx_remove(pThis);
						if(fun_left != NULL){
							fun_left(pThis);// lanzamos la funcion
							fun_right = NULL;
							fun_left = NULL;
						}
						break;
				}
			}
			break;
	}
	return 0;
}

/*************************************************************************//**
 * @fun     DialogBoxEx_remove
 * @brief   Dialog remove function
 *
 * @param   pThis      		this object as void
 *
 * @return  0: if message processing is finished
 ****************************************************************************/
void DialogBoxEx_remove(void *pThis){
	T_DEV_GUI * me = (T_DEV_GUI*)pThis;

	SetColor(WHITE); //(pLb2->hdr.pGolScheme->CommonBkColor);					// set the color
	Bar(0,0,GetMaxX(),GetMaxY());								// hide the pull down menu
	GOLFree();   												// remove the pull down menu
	GOLSetList(dbListSaved);										// set active list back to saved list
	GOLRedrawRec(0,0,GetMaxX(),GetMaxY());	
	
	// restablece los parametros
	me->OnCallbackForm = ptr_prev_callback;	
	me->OnDisplayForm = ptr_prev_display;
}


