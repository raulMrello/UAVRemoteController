/*************************************************************************//**
 * @file    dev_LCD.c
 * @mcu		STM32F1x
 * @brief   LCD and Backlight device manager
 * @date    14.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <RTL.h>
#include <stdio.h>
#include "dev_LCD.h"


/****************************************************************************************************//**
 *                        	REQUIRED INTERFACES
 ********************************************************************************************************/
#define IDriverInit() 				dev_LCD_Requires_IDriverInit()
#define IDriverDeinit()				dev_LCD_Requires_IDriverDeinit()
#define IWriteData(c) 				dev_LCD_Requires_IWriteData (c)
#define ILightInit()				dev_LCD_Requires_ILightInit ()
#define ILightAt(val)				dev_LCD_Requires_ILightAt	(val)
#define ILightOff()					dev_LCD_Requires_ILightOff	()
#define IBlAsOutput()				dev_LCD_Requires_IBlAsOutput()
#define IBlOn()						dev_LCD_Requires_IBlOn()
#define IBlOff()					dev_LCD_Requires_IBlOff()

#define IRstAsOutput()				dev_LCD_Requires_IRstAsOutput()
#define IRstSet()					dev_LCD_Requires_IRstSet()
#define IRstClear()					dev_LCD_Requires_IRstClear()
#define ICmdAsOutput()				dev_LCD_Requires_ICmdAsOutput()
#define ICmdOn()					dev_LCD_Requires_ICmdOn()
#define ICmdOff()					dev_LCD_Requires_ICmdOff()
#define ISelAsOutput()				dev_LCD_Requires_ISelAsOutput()
#define ISelActive()				dev_LCD_Requires_ISelActive()
#define ISelClear()					dev_LCD_Requires_ISelClear()
#define IDelayMs(m)					dev_LCD_Requires_IDelayMs(m)
#define IClearDevice()				dev_LCD_Requires_IClearDevice()



/****************************************************************************************************//**
 *                        	PRIVATE MEMBERS
 ********************************************************************************************************/

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

unsigned char LCD_MenMirror[8][128];  // Copia de la memoria 1024Byte
//BYTE LCD_FlagUpdate[16][8];  // Flag de actualización
unsigned char LCD_FlagUpdate = FALSE;

typedef enum{
	BACKLIGHT_OFF = 0,
	BACKLIGHT_LOW  = 25,
	BACKLIGHT_MED  = 50,
	BACKLIGHT_HIG  = 75,
	BACKLIGHT_MAX  = 100
}E_LCD_Backlight;

static E_LCD_Backlight blContrast = BACKLIGHT_MAX;
static uint8_t isBacklightForbidden = FALSE;

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		dev_LCD_Init
 * @brief	Provided interface to setup the device in 16bit access
 * @return	n/a
 **********************************************************************************/
void dev_LCD_Init(uint8_t blight){

	#if HARDWARE_RELEASE == HW_REV2
	IDriverInit();
	#endif
	// at init stage backlight is allowed
	isBacklightForbidden = FALSE;

	// COnfiguracion del Hardware
  	IRstAsOutput();
  	ICmdAsOutput();
  	ISelAsOutput();
  	ILightInit();//IBlAsOutput();

	// Valores pir defecto de las salidas
	ISelClear();
	IRstClear();
	if(blight == 0xFF){
		ILightAt(BACKLIGHT_MAX);
	}
	else{
		ILightAt(blight);//IBlOn();
	}
	IBlOn();
	// Inicializacion de los registros del LCD

  	ISelClear(); 	//= 1;
  	IRstSet(); 	//= 0;
  	IDelayMs(10);                               // Hace un reset de 1ms
  	IRstClear(); 	//= 1;
  
  	IDelayMs(500);
  
  
  	ISelActive(); 	//= 0;   // CS
  	ICmdOn();	// = 0;   // COMMAND
  	IWriteData(0xFA); //Set Adv. Program Control 0
  	IWriteData(0x93);
  	ISelClear();	// = 1;

  
  	ISelActive();	// = 0;   // CS
  	ICmdOn();	// = 0;   // COMMAND/DATA
  	IWriteData(0x24); //Set VLCD Resistor Ratio 100b
  	ISelClear();	// = 1;

  	//IDelayMs(5);    

  	ISelActive();	// = 0;   // CS
  	ICmdOn();	// = 0;   // COMMAND/DATA
  	IWriteData(0x81); //Set Electronic Volume
  	IWriteData(0x30); //Set Electronic Volume
 	ISelClear();	// = 1;

 	//IDelayMs(5);

 	ISelActive();	// = 0;   // CS
 	ICmdOn();  // COMMAND/DATA
  	IWriteData(0xA2); //Set LCD Bias Ratio
  	ISelClear();	// = 1;

  	//IDelayMs(5);
      
  	ISelActive();	// = 0;   // CS
  	ICmdOn();   // COMMAND/DATA
  	IWriteData(0x2F); //Set Power Control
  	ISelClear();
  
 	// IDelayMs(5);
  
  	/*  
  	ISelActive;   // CS
  	ICmdOn;   // COMMAND/DATA
 	IWriteData(0xA5); //Set All Pixel ON
  	ISelClear;
 	 */  
  	//IDelayMs(5);  
  
  	/*ISelActive;   // CS
  	ICmdOn;   // COMMAND/DATA
  	IWriteData(0xA7); //Set Inverse Display
  	ISelClear;
  		*/
    
  	//IDelayMs(5);  
  
  	ISelActive();	//= 0;   // CS
  	ICmdOn();   // COMMAND/DATA
  	IWriteData(0xAF); //Set Display Enable
  	ISelClear();

   	//__RESET_WATCHDOG();
  	// Escribimos la RAM del Display
  	IClearDevice();

  	dev_LCD_UpdateMem();
  
}

/****************************************************************************************************//**
 * @fun		dev_LCD_UpdateMem
 * @brief	Provided interface to update physical device RAM zone
 * @return	n/a
 **********************************************************************************/
void dev_LCD_UpdateMem(void){
 
    unsigned char x,y;
     //__RESET_WATCHDOG();
  
    ISelActive();	// = SELECT;
    for(y=0;y<8;y++){
        ICmdOn();	// = LCD_COMMAND;
        // SetAddress
        IWriteData(0x00); //Set Column Address L
        IWriteData(0x10); //Set Column Address H
        IWriteData(0xB0 | y); //Set Page Address

        
        for(x=0;x<128;x++){
          ICmdOff();	// = LCD_DATA;
          IWriteData(LCD_MenMirror[y][x]);
        }
    }
    ISelClear();	// = UNSELECT;
    
}

/****************************************************************************************************//**
 * @fun		dev_LCD_UpdateMem
 * @brief	Provided interface to update to draw the raw mem.
 * @return	n/a
 **********************************************************************************/
void dev_LCD_DrawMem(void){
  if( LCD_FlagUpdate){
    LCD_FlagUpdate = FALSE;
    dev_LCD_UpdateMem();
  }
}

/****************************************************************************************************//**
 * @fun		dev_LCD_Weakup
 * @brief	Provided interface to wakeup a physical device from standby mode
 * @return	n/a
 **********************************************************************************/
void dev_LCD_Weakup(void){
}

/****************************************************************************************************//**
 * @fun		dev_LCD_Standby
 * @brief	Provided interface to switch the physical device in standby mode
 * @return	n/a
 **********************************************************************************/
void dev_LCD_Standby(void){


	IBlOff();

	ISelActive();	//= 0;   // CS
	ICmdOn();   // COMMAND/DATA
	IWriteData(0xAE); //Set Display Disable
	ISelClear();

  	ISelActive();   // CS
  	ICmdOn();   // COMMAND/DATA
  	IWriteData(0xA5); //Set All Pixel ON
  	ISelClear();
 
}

/****************************************************************************************************//**
 * @fun		dev_LCD_TurnOn
 * @brief	Provided interface to switch all the pixels on
 * @return	n/a
 **********************************************************************************/
void dev_LCD_TurnOn(uint8_t blight) {

	#if HARDWARE_RELEASE == HW_REV2
	dev_LCD_Init(blight);
	#else
	dev_LCD_On();	// switch on backlight if permitted
   	#endif

  	ISelActive();   // CS
  	ICmdOn();   // COMMAND/DATA
  	IWriteData(0xA4); //Set All Pixel OFF
  	ISelClear();

	ISelActive();	//= 0;   // CS
	ICmdOn();   // COMMAND/DATA
	IWriteData(0xAF); //Set Display Enable
	ISelClear();


}

/****************************************************************************************************//**
 * @fun		dev_LCD_TurnOfff
 * @brief	Provided interface to switch all the pixels on
 * @return	n/a
 **********************************************************************************/
void dev_LCD_TurnOff(void) {

	#if HARDWARE_RELEASE == HW_REV2
	IRstSet();
	ICmdOn();
	ISelActive();
	IDriverDeinit();
	#endif
}

/****************************************************************************************************//**
 * @fun		dev_LCD_Off
 * @brief	Provided interface to switch all the pixels off
 * @return	n/a
 **********************************************************************************/
void dev_LCD_Off(void){
	IBlOff();
	
}

/****************************************************************************************************//**
 * @fun		dev_LCD_On
 * @brief	Provided interface to switch all the pixels on
 * @return	n/a
 **********************************************************************************/
void dev_LCD_On(void){
	if(!isBacklightForbidden){
		IBlOn();
	}	
}


/****************************************************************************************************//**
 * @fun		dev_LCD_PermitBacklight
 * @brief	Provided interface to permit backlight switching on
 * @return	n/a
 **********************************************************************************/
void dev_LCD_PermitBacklight(void){
	isBacklightForbidden = FALSE;
}


/****************************************************************************************************//**
 * @fun		dev_LCD_ForbidBacklight
 * @brief	Provided interface to forbid backlight switching on
 * @return	n/a
 **********************************************************************************/
void dev_LCD_ForbidBacklight(void){
	isBacklightForbidden = TRUE;
}


/****************************************************************************************************//**
 * @fun		dev_LCD_GetContrast
 * @brief	Provided interface to get the actual contrast ratio 0..100
 * @return	n/a
 **********************************************************************************/
uint8_t dev_LCD_GetContrast(void){
	return ((uint8_t)blContrast);
	
}

/****************************************************************************************************//**
 * @fun		dev_LCD_SetContrast
 * @brief	Provided interface to set the actual contrast ratio 0..100
 * @return	n/a
 **********************************************************************************/
void dev_LCD_SetContrast(uint8_t contrast){
	blContrast = (E_LCD_Backlight)contrast;
	ILightAt(blContrast);	
	IBlOn();
}


