/*************************************************************************//**
 * @file    dev_GPS.c
 * @mcu		STM32F1x
 * @brief   GPS Device Manager
 * @date    11.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "dev_GPS_hsm.h"

#define GPS_STACK_SIZE			400	// tamaño en bytes
static volatile unsigned long long 	GPS_Stack[(GPS_STACK_SIZE/8)];

 
/****************************************************************************************************//**
 *                        PRIVATE MEMBERS
 ********************************************************************************************************/

// structura de datos GPS
static T_DEV_Gps This;

typedef struct{
    uint8_t    type;                   //!< Type of input device.
    uint8_t    uiEvent;                //!< The generic events for input device.
    uint16_t   param1;                 //!< Parameter 1 meaning is dependent on the type of input device.
    uint16_t   param2;                 //!< Parameter 2 meaning is dependent on the type of input device.
} GPS_MSG;

static void GPS_Task (Object *pThis);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		dev_GPS_Init
 * @brief	Provided interface used by the initial system loading to carry out GPS Device initialization
 * @param	priority	task priority
 * @return	
 **********************************************************************************/
uint8_t dev_GPS_Init(uint8_t priority){		
	ErrorCode err = ERR_OK;
	
	IDriverInit(4800);//drv_UART2_Init(4800);
	// configuracion de pines
	IDriverEnable(); //drv_GPIO_GPS_EN_as_output();	
	IDriverTurnOn(); //EN_line = 0

	// Initialize IRunnable interface. If succesful, an OS_TID is assigned.
	err = IRunnable_Init   (&This.runnable, 
							&This, 
							&This.runnable.tid, 
							0, 
							0, 
							(void*)&GPS_Stack, 
							(U16)GPS_STACK_SIZE,
							priority,
							&GPS_Task,
							&GPS_State_Initial);

	memcpy((uint8_t *)&This.gps,(uint8_t *)&gps_init,sizeof(STRGps));
	return((uint8_t)err);
}

/****************************************************************************************************//**
 * @fun		dev_GPS_NewRecvData
 * @brief	Provided interface used by the driver to notify new incoming data from the physical dev.
 * @return	n/a
 **********************************************************************************/
void dev_GPS_NewRecvData(void){
	isr_evt_set (EVF_NEWDATA, This.runnable.tid);
}

/****************************************************************************************************//**
 * @fun		dev_GPS_NewSentData
 * @brief	Provided interface used by the driver to notify new outgoing data from the physical dev.
 * @return	n/a
 **********************************************************************************/
void dev_GPS_NewSentData(void){
}

/****************************************************************************************************//**
 * @fun		dev_GPS_GetStatus
 * @brief	Provided interface to be used by the GUIManager to check the status of the device manager
 * @return	??
 **********************************************************************************/
uint8_t dev_GPS_GetStatus (void){
	return((uint8_t)This.ctrlFlags);
}

/****************************************************************************************************//**
 * @fun		dev_GPS_PowerOn
 * @brief	Provided interface to be used by the GUIManager to turn On the GPS physical device
 * @return	??
 **********************************************************************************/
void dev_GPS_PowerOn(void){
	isr_evt_set (EVF_POWERON, This.runnable.tid);
}


/****************************************************************************************************//**
 * @fun		dev_GPS_PowerOff
 * @brief	Provided interface to be used by the GUIManager to turn Off the GPS physical device
 * @return	??
 **********************************************************************************/
void dev_GPS_PowerOff(void){
	isr_evt_set (EVF_POWEROFF, This.runnable.tid);
}


/****************************************************************************************************//**
 * @fun		dev_GPS_IsReadyForPowerDown
 * @brief	Provided interface to check if gps is stopped
 * @return	1 if stopped
 **********************************************************************************/
uint8_t	dev_GPS_IsReadyForPowerDown (void){
	if(This.ctrlFlags > STAT_INITIATED){
		return 0;
	}
	return 1;
}

/****************************************************************************************************//**
 * @fun		GPS_Task
 * @brief	Task for this device
 * @return	
 **********************************************************************************/
static void GPS_Task (Object *pThis){
	T_DEV_Gps * me = (T_DEV_Gps*)pThis;
    uint16_t recvflag;
	
	// Starts the state machine
    StateMachine_Init(me);       

	for(;;){
		OS_RESULT errno;
		const Event newDataEvt = {EV_NEWDATA};
		const Event powerOnEvt = {EV_POWERON};
		const Event powerOffEvt = {EV_POWEROFF};
		errno = os_evt_wait_or(EVF_GPS_ALLFLAGS, me->timeout);
		// if timeout, launch timeout event
		if(errno == OS_R_TMO){
			StateMachine_Dispatch(me, (Event const *)&timeoutEvt);
		}
		// if EVF_DATARECV or EVF_CANCELLED flag, check and dispatch
		else{
			recvflag = os_evt_get();			// get event received
			if((recvflag & EVF_NEWDATA) != 0){	// process cancel request with high priority
				os_evt_clr(EVF_NEWDATA, me->runnable.tid);
				StateMachine_Dispatch(me, (Event const *)&newDataEvt);
			}
			if((recvflag & EVF_POWERON) != 0){	// process data sent notification
				os_evt_clr(EVF_POWERON, me->runnable.tid);
				StateMachine_Dispatch(me, (Event const *)&powerOnEvt);
			}
			if((recvflag & EVF_POWEROFF) != 0){	// process data received notification
				os_evt_clr(EVF_POWEROFF, me->runnable.tid);
				StateMachine_Dispatch(me, (Event const *)&powerOffEvt);
			}
		}
	}
}




