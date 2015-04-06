/*************************************************************************//**
 * @file    SYS_Link.c
 *
 * @brief   Módulo de enlace estático entre componentes. Define la asociación
 *			entre los mismos.
 *
 * @date    11.06.2010
 *
 * @author  Raúl M.
 ****************************************************************************/
#include "dev_GUI.h"
#include "dev_GPS.h"
#include "dev_keypad.h"
#include "dev_IR.h"
#include "dev_Buzzer.h"
#include "dev_LCD.h"
#include "dev_Calendar.h"
#include "dev_Bat.h"
#include "drv_TIM2.h"
#include "drv_TIM3.h"
#include "drv_UART1.h"
#include "drv_UART2.h"
#include "drv_LCD.h"
#include "drv_KEY.h"
#include "drv_IR.h"
#include "drv_GPS.h"
#include "drv_SPI2.h"
#include "drv_RTC.h"
#include "drv_POW.h"
#include "drv_ADC.h"

/****************************************************************************************************//**
 *                        drv_UART1 REQUIRED INTERFACES
 *
 * @brief	El driver UART1 es utilizado por <dev_IR> y por lo tanto las notificaciones de datos
 *			recibidos vía serie deben enviarse a dicho dispositivo.
 ********************************************************************************************************/
void (* const drv_UART1_Requires_IRecvEvent)(void) = dev_IR_NewRecvData;
void (* const drv_UART1_Requires_ISentEvent)(void) = dev_IR_NewSentData;

/****************************************************************************************************//**
 *                        drv_UART2 REQUIRED INTERFACES
 *
 * @brief	El driver UART2 es utilizado por <dev_GPS> y por lo tanto las notificaciones de datos
 *			recibidos vía serie deben enviarse a dicho dispositivo.
 ********************************************************************************************************/
void (* const drv_UART2_Requires_IRecvEvent)(void) = dev_GPS_NewRecvData;
void (* const drv_UART2_Requires_ISentEvent)(void) = dev_GPS_NewSentData;

/****************************************************************************************************//**
 *                        drv_KEY REQUIRED INTERFACES
 *
 * @brief	El driver drv_KEY notifica las pulsaciones de teclado al dispositivo <dev_KeyPad>.
 ********************************************************************************************************/
void (* const drv_KEY_Requires_IKeyEvent)(void) = dev_KeyPad_NewKey;

/****************************************************************************************************//**
 *                        drv_ADC REQUIRED INTERFACES
 *
 * @brief	El driver drv_ADC notifica datos escandeados al dispositivo <dev_Bat>.
 ********************************************************************************************************/
void (* const drv_ADC_Requires_IDataEvent)(void) = dev_Bat_NewValue;

/****************************************************************************************************//**
 *                        drv_RTC REQUIRED INTERFACES
 *
 * @brief	El driver drv_RTC notifica un nuevo segundo al dispositivo <dev_Calendar>.
 ********************************************************************************************************/
void (* const drv_RTC_Requires_ITimeEvent)(void) = dev_Calendar_NewTime;


/****************************************************************************************************//**
 *                        dev_Buzzer REQUIRED INTERFACES
 *
 * @brief	El dispositivo Buzzer utiliza al driver <drv_TIM2> para configurar el PWM asociado
 ********************************************************************************************************/
void (* const dev_Buzzer_Requires_IDriverInit)(void) 	= drv_TIM2_Init;
void (* const dev_Buzzer_Requires_IStart)(void) 		= drv_TIM2_on;
void (* const dev_Buzzer_Requires_IStop)(void) 			= drv_TIM2_off;

 
/****************************************************************************************************//**
 *                        dev_GPS REQUIRED INTERFACES
 *
 * @brief	El dispositivo <dev_GPS> utiliza como fuente de datos entrantes al driver UART2 y como
 *			gestor de datos recibidos al GUI, sobre el que publicará los mensajes GPS correspondientes.
 *			Para controlar el dispositivo físico, se utilizan varias líneas del driver <drv_GPIO>.
********************************************************************************************************/
void (* const dev_GPS_Requires_IDriverInit)(uint32_t baudrate) 				= drv_UART2_Init;
void (* const dev_GPS_Requires_IDriverEnableRx)(void) 						= drv_UART2_EnableRx;
void (* const dev_GPS_Requires_IDriverDisableRx)(void) 						= drv_UART2_DisableRx;
void (* const dev_GPS_Requires_IDriverEnable)(void) 						= drv_GPS_EN_as_output;
void (* const dev_GPS_Requires_IDriverTurnOn)(void) 						= drv_GPS_EN_On;
void (* const dev_GPS_Requires_IDriverTurnOff)(void) 						= drv_GPS_EN_Off;
int  (* const dev_GPS_Requires_IGetData)(void) 								= drv_UART2_GetChar;
void (* const dev_GPS_Requires_IDataReady)(uint8_t *pdata, uint16_t size) 	= dev_GUI_NewGpsData;
uint8_t	(* const dev_GPS_Requires_ISend)(uint8_t *ptr, uint8_t size)		= drv_UART2_SendMsg;



/****************************************************************************************************//**
 *                        dev_KeyPad REQUIRED INTERFACES
 *
 * @brief	El dispositivo <dev_KeyPad> usa el driver drv_KEY como entrada de teclas. Además postea mensajes
 *			en el componente dev_GUI.
********************************************************************************************************/
void 	(* const dev_KeyPad_Requires_IDriverInit)(void) 			= drv_KEY_Init;
int8_t	(* const dev_KeyPad_Requires_IGetKey)(void) 				= drv_KEY_GetKey;
void 	(* const dev_KeyPad_Requires_ISendKey)(uint32_t keycode)	= dev_GUI_NewKey;



/****************************************************************************************************//**
 *                        dev_Bat REQUIRED INTERFACES
 *
 * @brief	El dispositivo <dev_Bat>  usa el driver drv_ADC como entrada de datos. Además postea mensajes 
 *			en el componente dev_GUI.
 ********************************************************************************************************/
void 	 (* const dev_Bat_Requires_IDriverInit)(void)			= drv_ADC_Init;
void 	 (* const dev_Bat_Requires_IDriverScan)(void)			= drv_ADC_Scan;
void 	 (* const dev_Bat_Requires_IDriverCancel)(void) 		= drv_ADC_Cancel;
uint16_t (* const dev_Bat_Requires_IGetValue)(void)				= drv_ADC_GetValue;



/****************************************************************************************************//**
 *                        dev_IR REQUIRED INTERFACES
 *
 * @brief	El dispositivo <dev_IR>  usa el driver drv_UART1 como io de datos, así como el driver
 *			drv_IR para acceder al chip_enable externo. Además postea mensajes en el componente dev_GUI.
 ********************************************************************************************************/
void 	(* const dev_IR_Requires_IDriverInit)(uint32_t baudrate)			= drv_UART1_Init;
void 	(* const dev_IR_Requires_IDriverEnableRx)(void)						= drv_UART1_EnableRx;
void 	(* const dev_IR_Requires_IDriverDisableRx)(void)					= drv_UART1_DisableRx;
void 	(* const dev_IR_Requires_IDriverEnable)(void)						= drv_IR_EN_as_output;
void 	(* const dev_IR_Requires_IDriverTurnOn)(void)						= drv_IR_EN_On;
void 	(* const dev_IR_Requires_IDriverTurnOff)(void)						= drv_IR_EN_Off;
int 	(* const dev_IR_Requires_IGetData)(void)							= drv_UART1_GetChar;
uint8_t	(* const dev_IR_Requires_ISend)(uint8_t *ptr, uint8_t size)			= drv_UART1_SendMsg;
void 	(* const dev_IR_Requires_IDataReady)(uint8_t *pdata, uint16_t size)	= dev_GUI_NewIrData;



/****************************************************************************************************//**
 *                        dev_LCD REQUIRED INTERFACES
 *
 * @brief	El dispositivo <dev_LCD>  usa el driver drv_SPI2 como salida de datos, el drv_TIM3 como
 *			salida del backlight y el driver drv_LCD como controlador del chip físico
 ********************************************************************************************************/
void 	(* const dev_LCD_Requires_IDriverInit)(void)			= drv_SPI2_InitByDefault;
void 	(* const dev_LCD_Requires_IDriverDeinit)(void)			= drv_SPI2_Deinit;
int 	(* const dev_LCD_Requires_IWriteData)(int c)			= drv_SPI2_SendChar;
void 	(* const dev_LCD_Requires_ILightInit)(void)				= drv_TIM3_Init;
void 	(* const dev_LCD_Requires_ILightAt)(uint8_t val)		= drv_TIM3_SetDuty;
void 	(* const dev_LCD_Requires_ILightOff)(void)				= drv_TIM3_off;
void 	(* const dev_LCD_Requires_IBlAsOutput)(void)			= drv_LCD_BL_as_output;
void 	(* const dev_LCD_Requires_IBlOn)(void)					= drv_TIM3_on;//drv_LCD_BL_On;
void 	(* const dev_LCD_Requires_IBlOff)(void)					= drv_TIM3_off;//drv_LCD_BL_Off;
void 	(* const dev_LCD_Requires_IRstAsOutput)(void)			= drv_LCD_RST_as_output;
void 	(* const dev_LCD_Requires_IRstSet)(void)				= drv_LCD_RST_Set;
void 	(* const dev_LCD_Requires_IRstClear)(void)				= drv_LCD_RST_Clear;
void 	(* const dev_LCD_Requires_ICmdAsOutput)(void)			= drv_LCD_CMD_as_output;
void 	(* const dev_LCD_Requires_ICmdOn)(void)					= drv_LCD_CMD_On;
void 	(* const dev_LCD_Requires_ICmdOff)(void)				= drv_LCD_CMD_Off;
void 	(* const dev_LCD_Requires_ISelAsOutput)(void)			= drv_LCD_SEL_as_output;
void 	(* const dev_LCD_Requires_ISelActive)(void)				= drv_LCD_SEL_Active;
void 	(* const dev_LCD_Requires_ISelClear)(void)				= drv_LCD_SEL_Clear;
void 	(* const dev_LCD_Requires_IDelayMs)(uint16_t delay)		= drv_LCD_DelayMs;
void 	(* const dev_LCD_Requires_IClearDevice)(void)			= drv_LCD_ClearDevice;



/****************************************************************************************************//**
 *                        dev_Calendar REQUIRED INTERFACES
 *
 * @brief	El dispositivo <dev_Calendar> usa el driver drv_RTC como entrada de eventos. Además postea mensajes
 *			en el componente dev_GUI.
********************************************************************************************************/
void 	 (* const dev_Calendar_Requires_IDriverInit)(void)			= drv_RTC_Init;
void 	 (* const dev_Calendar_Requires_IDriverEnable)(void)		= drv_RTC_Enable;
void 	 (* const dev_Calendar_Requires_IDriverDisable)(void)		= drv_RTC_Disable;
uint32_t (* const dev_Calendar_Requires_IGetTime)(void)				= drv_RTC_GetTime;
void 	 (* const dev_Calendar_Requires_ISetTime)(uint32_t t)		= drv_RTC_SetTime;
void 	 (* const dev_Calendar_Requires_ISendTime)(uint32_t tEv)	= dev_GUI_NewTime;



/****************************************************************************************************//**
 *                        dev_GUI REQUIRED INTERFACES
 *
 * @brief	El dispositivo <dev_GUI> usa diferentes interfaces del resto de dispositivos
********************************************************************************************************/
uint8_t	 (* const dev_GUI_Requires_IBeepShort)(void)							= dev_Buzzer_BeepShort;
uint8_t	 (* const dev_GUI_Requires_IBeepLong)(void)								= dev_Buzzer_BeepLong;
uint8_t	 (* const dev_GUI_Requires_IBeepConfirm)(void)							= dev_Buzzer_BeepConfirm;
void 	 (* const dev_GUI_Requires_IEnDisCalendarEvents)(uint8_t endis)			= dev_Calendar_EnDisEvents;
void 	 (* const dev_GUI_Requires_ICaptureLocation)(void)						= dev_GPS_PowerOn;
void 	 (* const dev_GUI_Requires_IReleaseCapture)(void)						= dev_GPS_PowerOff;
uint8_t  (* const dev_GUI_Requires_ICaptureIsDown)(void)						= dev_GPS_IsReadyForPowerDown;
uint8_t	 (* const dev_GUI_Requires_ISendRemote)(uint8_t *pdu, uint8_t len)		= dev_IR_Send;
uint8_t	 (* const dev_GUI_Requires_IReadRemote)(uint8_t *pdu, uint8_t len)		= dev_IR_Read;
uint8_t	 (* const dev_GUI_Requires_ICancelRemote)(void)							= dev_IR_Cancel;
uint8_t	 (* const dev_GUI_Requires_IPowerRemote)(void)							= dev_IR_Power;
uint8_t	 (* const dev_GUI_Requires_IUnpowerRemote)(void)						= dev_IR_Unpower;
uint8_t	 (* const dev_GUI_Requires_ICheckRemoteReady)(void)						= dev_IR_IsWaitingReq;
void 	 (* const dev_GUI_Requires_IDraw)(void)									= dev_LCD_UpdateMem;
void 	 (* const dev_GUI_Requires_IDrawMem)(void)								= dev_LCD_DrawMem;
void 	 (* const dev_GUI_Requires_IDisplayOn)(uint8_t b)						= dev_LCD_TurnOn;
void 	 (* const dev_GUI_Requires_IDisplayOff)(void)							= dev_LCD_TurnOff;
void 	 (* const dev_GUI_Requires_IDisplayStandby)(void)						= dev_LCD_Standby;
void 	 (* const dev_GUI_Requires_IBacklightOn)(void)							= dev_LCD_On;
void 	 (* const dev_GUI_Requires_IBacklightOff)(void)							= dev_LCD_Off;
void 	 (* const dev_GUI_Requires_IBacklightPermit)(void)						= dev_LCD_PermitBacklight;
void 	 (* const dev_GUI_Requires_IBacklightForbid)(void)						= dev_LCD_ForbidBacklight;
uint8_t	 (* const dev_GUI_Requires_IBacklightGetContrast)(void)					= dev_LCD_GetContrast;
void 	 (* const dev_GUI_Requires_IBacklightSetContrast)(uint8_t c)			= dev_LCD_SetContrast;
void *	 (* const dev_GUI_Requires_IGetLogo)(void)								= Graphics_GetLogo;
void 	 (* const dev_GUI_Requires_IGetCalendar)(CCalendar *c)					= dev_Calendar_Get;
void 	 (* const dev_GUI_Requires_ISetCalendar)(CCalendar *c)					= dev_Calendar_Set;
uint32_t (* const dev_GUI_Requires_IGetCalendarCounter)(void)					= dev_Calendar_GetCounter;
void 	 (* const dev_GUI_Requires_IBatteryStartScan)(void)						= dev_Bat_StartScan;
uint8_t	 (* const dev_GUI_Requires_IBatteryGetLevel)(void)						= dev_Bat_GetLevel;
void 	 (* const dev_GUI_Requires_IBatteryCancelScan)(void)					= dev_Bat_CancelScan;
void 	 (* const dev_GUI_Requires_IEnterStopMode)(void)						= drv_POW_EnterStopMode;
void 	 (* const dev_GUI_Requires_IResartClocks)(void)							= drv_POW_ResartClocks;
int8_t 	 (* const dev_GUI_Requires_IGetKey)(void)						  	    = drv_KEY_GetKey;

