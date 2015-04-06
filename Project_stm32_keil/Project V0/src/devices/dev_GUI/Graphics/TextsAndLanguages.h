/*************************************************************************//**
 * @file    dev_GUI_hsm.h
 * @mcu		STM32F1x
 * @brief   GUI Device State Machine protected header
 *			THIS FILE IS GENERATED AUTOMATICALLY!!!!!!
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __TEXTSANDLANGUAGES_H__
#define __TEXTSANDLANGUAGES_H__

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>
#include <SYS_Config.h>

/****************************************************************************************************//**
 *                        PUBLIC TYPES
 ********************************************************************************************************/

char * 	GUI_GetCountries	(void);
uint8_t	GUI_GetCountryCode	(uint8_t countryID);
char * 	GUI_GetRegions		(uint8_t countryID);
char * 	GUI_GetCities		(uint8_t countryID, uint8_t region_ID);
uint8_t GUI_GetCityCode		(uint8_t countryID, uint8_t region_ID, uint8_t city_ID);
uint8_t GUI_GetLanguageCode	(uint8_t langID);
int8_t  GUI_GetCountryGMT	(uint8_t countryID);


/*************************************************************************//**
 * @def		DISTRIBUTION_MODEL
 * @scope	public
 * @brief   Country id numbers and default selected country for the device
 ****************************************************************************/

#if DISTRIBUTION_MODEL == MODEL_ESP_POR_ITA
#include "TextsAndLanguages_esp_por_ita.h"

#elif DISTRIBUTION_MODEL == MODEL_NOR_SVE
#include "TextsAndLanguages_nor_sve.h"

#elif DISTRIBUTION_MODEL == MODEL_EU0
#include "TextsAndLanguages_EU0.h"

#endif

/*************************************************************************//**
 * @enum	E_Countries
 * @scope	public
 * @brief   Country codes to pair MANDO and DATA
 ****************************************************************************/
 typedef enum{
 	COUNTRIES_FIRST = 2, //0 y 1 reservados. Dan error al codificar manchester
	EU_ESP = COUNTRIES_FIRST,
	EU_ITA,
	EU_POR,
	EU_NOR,
	EU_SVE,
	COUNTRIES_END = 255
 }E_Countries;

/*************************************************************************//**
 * @enum	E_TextCodes
 * @scope	public
 * @brief   ID codes to reference each text according with the selected language
 ****************************************************************************/
typedef enum{
	TXT_NO = 0,
	TXT_SI,
	TXT_Volcar,
	TXT_Salvar,
	TXT_Borrar,
	TXT_Volcar_,
	TXT_GUARDAR___,
	TXT_Borrar_,
	TXT_Guardar_cambios_,
	TXT_MENU_PULSE_OK,
	TIT_MENU,				
	TIT_EQUIPOS,			
	TIT_CONFIGURACION,		
	TIT_LOCALIZACION,			
	TIT_CAPTURA_GPS,	
	TIT_LOCALIZ_GUARDADA,
	TIT_IDIOMA,
	TIT_HORAFECHA,
	TIT_HORA,
	TIT_MINUTO,
	TIT_DIA,
	TIT_MES,
	TIT_ANIO,
	TIT_BORRAR_MEMORIA,
	TIT_VERSION,
	TIT_CONTRASTE,
	TIT_LEER_EQUIPO,
	TXT_Leido_Ok,
	TXT_Enviado_Ok,
	TXT_Otro_mas_,
	TXT_Opciones_,
	TIT_VOLCANDO,
	TIT_GUARDAR_Como___,
	TIT_EQUIPOS_GUARDADOS,
	TIT_CIUDADES_POR_PAIS,
	TXT_Zona,
	TXT_Ajustes,
	TXT_Reloj,
	TXT_Estaciones,
	TXT_Vacaciones,
	TXT_Programa_,
	TXT_Programas___,
	TXT_Ya_existe,
	TXT_Sobreescribir_,
	TXT_Reintentar_,
	TXT_Archivo_guardado_,
	TXT_Memoria_vacia_,
	TXT_Error_guardando,
	TXT_Salir_sin_guardar,
	TXT_Sincronizacion,
	TXT_en_progreso,
	TXT_Sincr_reloj_,
	TXT_Sincronizar_,
	TXT_Lat_,
	TXT_Lon_,
	TXT_Formateando,
	TXT_Elborradodura,
	TXT_Espere_60seg,
	TXT_Cambiar,
	TXT_Pilas,
	TXT_pulse_ok
}E_TextCodes;

/*************************************************************************//**
 * @enum	E_ListboxCodes
 * @scope	public
 * @brief   ID codes to reference each listbox option according with the selected language
 ****************************************************************************/
typedef enum{
	LBX_MENU = 0,
	LBX_EQUIPOS,
	LBX_CONFIGURACION,		
	LBX_LOCALIZACION,			
	LBX_IDIOMA,
	LBX_BORRAR_MEMORIA,
}E_ListboxCodes;

#endif


