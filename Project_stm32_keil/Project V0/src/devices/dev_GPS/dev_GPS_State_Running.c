/*************************************************************************//**
 * @file    dev_GPS_State_Running.h
 * @mcu		STM32F1x
 * @brief   GPS Device State Machine <Running> state implementation
 * @date    13.07.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        	REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <RTL.h>
#include <string.h>
#include "dev_GPS_hsm.h"		// includes drv_IR.h


/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/

/*		Event handling routines		*/
static State OnEntry	(T_DEV_Gps * me, Event const * m);
static State OnExit		(T_DEV_Gps * me, Event const * m);
static State OnNewData	(T_DEV_Gps * me, Event const * m);
static State OnPowerOn 	(T_DEV_Gps * me, Event const * m);
static State OnPowerOff	(T_DEV_Gps * me, Event const * m);
static State OnTimeout	(T_DEV_Gps * me, Event const * m);

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     GPS_State_Running
 * @brief   Force a driver PowerDown sequence:
 *
 * @param   pThis      	this object as void
 * @param	e			Events received by this state
 *
 * @return  n/a
 ****************************************************************************/
State GPS_State_Running(Object* pThis, Event const * e){
	T_DEV_Gps * me = (T_DEV_Gps*)pThis;
	switch(e->sig){
		case ENTRY_SIG:
			return(OnEntry(me,(Event const *)e));

		case EXIT_SIG:
			return(OnExit(me,(Event const *)e));

		case EV_NEWDATA:
			return(OnNewData(me,(Event const *)e));

		case EV_POWERON:
			return(OnPowerOn(me,(Event const *)e));

		case EV_POWEROFF:	
			return(OnPowerOff(me,(Event const *)e));
		
		case TIMEOUT_SIG:	
			return(OnTimeout(me,(Event const *)e));

	}
    SUPER(&GPS_State_Top);
}



/****************************************************************************************************//**
 *                        EVENT HANDLERS in TOP state
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     OnEntry
 * @brief   Handles ENTRY_SIG events. Starts an hibernate sequence
 *
 * @param   This      	this object
 * @param	m			Event received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnEntry(T_DEV_Gps * me, Event const * m){
	(void)m;	// not used
	#if HARDWARE_RELEASE == HW_REV2
	me->timeout = GPS_ASSERTED_TIMEOUT;
	#else
	me->timeout = GPS_DEASSERTED_TIMEOUT;
	#endif
	me->ctrlFlags |= STAT_INITSEQUENCE; 
	memcpy((uint8_t *)&me->gps,(uint8_t *)&gps_init,sizeof(STRGps));
	IDriverTurnOn();	// to ensure low level on pin
	me->ctrlFlags &= ~STAT_WAITFIRSTRECV;
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnExit
 * @brief   Handles EXIT_SIG events
 *
 * @param   This      	this object
 * @param	m			Event received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnExit(T_DEV_Gps * me, Event const * m){
	(void)me; (void)m;	// not used
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnPowerOn
 * @brief   powers on the gps device
 *
 * @param   This      	this object
 * @param	m			Event received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnPowerOn(T_DEV_Gps * me, Event const * m){
	(void)me; (void)m;	// not used
	HANDLED();
}

/*************************************************************************//**
 * @fun     OnPowerOff
 * @brief   powers off the gps device
 *
 * @param   This      	this object
 * @param	m			Event received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnPowerOff(T_DEV_Gps * me, Event const * m){
	(void)m;	// not used
	IDriverDisableRx();
	TRAN(&GPS_State_Disabling);	 // start power off sequence
}

/*************************************************************************//**
 * @fun     OnNewData
 * @brief   Handles incoming data events
 *
 * @param   This      	this object
 * @param	m			Event received by this state
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnNewData(T_DEV_Gps * me, Event const * m){
	int data;
	me->timeout = 0xFFFF;	// stop timeout events
	do{
  		data = IGetData();//drv_UART2_GetChar();
		if(data != -1){
			if(GPS_GetData((uint8_t)data) != 0){	
				if((me->ctrlFlags & STAT_WAITFIRSTRECV)==0){
					me->ctrlFlags |= STAT_WAITFIRSTRECV;
					// send data through the required interface	ABP enabling
/*					char abp_enable[26];
					strcpy(abp_enable,(char*)NMEA_ABP_ENABLE);
					abp_enable[23] = 0x0D;//<CR>
					abp_enable[24] = 0x0A;//<LF>
					abp_enable[25] = 0; // not required for send */
					ISend((uint8_t*)NMEA_ABP_ENABLE, 25);
				}
			}
			GPS_CheckFrame(&me->gps, IDataReady);
		}	
	}while(data != -1);
	HANDLED();
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
 * @param	m			Event in process <me->msg>
 *
 * @return  Handled event codes and[if required] transition to other states.
 ****************************************************************************/
static State OnTimeout(T_DEV_Gps * me, Event const * m){
	(void)m;	// not used
	#if HARDWARE_RELEASE == HW_REV2
	me->ctrlFlags &= ~STAT_INITSEQUENCE;
	IDriverEnableRx();
	#else
	if(me->ctrlFlags & STAT_INITSEQUENCE){
		IDriverTurnOff();
		me->timeout = GPS_ASSERTED_TIMEOUT;
		me->ctrlFlags &= ~STAT_INITSEQUENCE;
		me->ctrlFlags |= STAT_POWSEQUENCE;
		HANDLED();
	}
	me->timeout = GPS_DEASSERTED_TIMEOUT;	
	if(me->ctrlFlags & STAT_POWSEQUENCE){
		me->ctrlFlags &= ~STAT_POWSEQUENCE;
		me->ctrlFlags |= STAT_WAITDATA;
		IDriverTurnOn();		
	}
	else if(me->ctrlFlags & STAT_WAITDATA){
		me->ctrlFlags &= ~STAT_WAITDATA;
		IDriverEnableRx();
	}
	// this case only will be entered if no powerup sequence executed correctly.
	else{
		TRAN(&GPS_State_Running);	// retry to powerup again
	}
	#endif
	HANDLED();
}




