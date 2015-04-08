/*
 * drv_POW.c
 *
 *  Created on: 07/04/2015
 *      Author: raulMrello
 *
 */
 
#include "drv_POW.h"

//------------------------------------------------------------------------------------
//-- PRIVATE DEFINITIONS -------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- PRIVATE MEMBERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------

static void setStopMode(void);
static void restartClocks(void);
static SYS_TOPIC_DATA_T sysTopicData;
static Topic * sysTopic;

/** \fun setStopMode
 *  \brief Sets stop mode
 */
static void setStopMode(void);

/** \fun restartClocks
 *  \brief Restart clocks after exiting from stop mode
 */
static void restartClocks(void);

//------------------------------------------------------------------------------------
//-- IMPLEMENTATION  -----------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
static void setStopMode(void){
	sysTopicData.status &= ~SYS_CPU_RUN;
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
}

//------------------------------------------------------------------------------------
static void restartClocks(void){
	ErrorStatus HSEStartUpStatus;

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
 
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
 
	if(HSEStartUpStatus == SUCCESS){
 
		#ifdef STM32F10X_CL
		/* Enable PLL2 */ 
		RCC_PLL2Cmd(ENABLE);
 
		/* Wait till PLL2 is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET){ }
		#endif
 
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
 
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){ }
 
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08){ }
	}
	sysTopicData.status |= SYS_CPU_RUN;
}

//------------------------------------------------------------------------------------
void drv_POW_Init(Exception *e){
	/** Subscribe to SystemTopics update and attach callback function */
	sysTopic = SystemTopic_getRef("/sys", e);
	catch(e){
		return;
	}
	Topic_attach(sysTopic, 0, e);
	catch(e){
		return;
	}
	// sets default value for topic handler 
	sysTopicData.status |= SYS_CPU_RUN;
	sysTopicData.queries = SYS_NO_QUERIES;
}

//------------------------------------------------------------------------------------
void drv_POW_OnTopicUpdate(void * obj, TopicData * td){
	(void)obj;	// param not used
	//topic checking
	if(td->id == (int)sysTopic){
		SYS_TOPIC_DATA_T *topic = (SYS_TOPIC_DATA_T*)td->data;
		if((sysTopicData.status & SYS_CPU_RUN) == 0 && (topic->queries & SYS_CPU_SET_RUN) != 0){
			restartClocks();
		}
		else if((sysTopicData.status & SYS_CPU_RUN) != 0 && (topic->queries & SYS_CPU_SET_STOP) != 0){
			setStopMode();
		}
	}	
}

