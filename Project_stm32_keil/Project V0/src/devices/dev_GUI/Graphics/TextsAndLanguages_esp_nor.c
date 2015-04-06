/*************************************************************************//**
 * @file    TextsAndLanguages.c
 * @mcu		STM32F1x
 * @brief   texts and languages to display
 *			THIS FILE IS GENERATED AUTOMATICALLY!!!!!!
 * @date    28.09.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include "TextsAndLanguages.h"
#include "SYS_Config.h"

#if DISTRIBUTION_MODEL == MODEL_ESP_NOR

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										TEXTS BY LANGUAGE	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/
/*************************************************************************//**
 * @var		TEXTS
 * @scope	public
 * @brief   Exported Array with texts in different languages
 ****************************************************************************/
const char * const TEXTS[][MAX_LANGUAGES] = {
	{"NO",					"NO"},
	{"SI",					"YES"},
	{"Volcar",				"Send"},
	{"Salvar",				"Save"},
	{"Borrar",				"Erase"},
	{"¿Volcar?",			"Send?"},
	{"GUARDAR...",			"SAVE..."},
	{"¿Borrar?",			"Delete?"},
	{"¿Guardar cambios?",	"Save changes?"},
	{"MENU: Pulse Ok...",	"MENU: press Ok.."},
	{"MENU",				"MENU"},
	{"EQUIPOS",				"DEVICES"},
	{"CONFIGURACION",		"CONFIG"},
	{"LOCALIZACION",		"LOCATIONS"},
	{"CAPTURA GPS",			"GPS CAPTURE"},
	{"LOCALIZ. GUARDADA",	"SAVED POSITION"},
	{"IDIOMA",				"LANGUAGE"},
	{"FECHA/HORA",			"DATE/TIME"},
	{"HORA",				"HOUR"},
	{"MINUTO",				"MINUTE"},
	{"DIA",					"DAY"},
	{"MES",					"MONTH"},
	{"AÑO",					"YEAR"},
	{"BORRAR MEMORIA",		"DELETE MEMORY"},
	{"VERSION",				"VERSION"},
	{"CONTRASTE",			"CONTRAST"},
	{"LEYENDO",				"READING"},
	{"Leído...Ok",			"Read...Ok"},
	{"Enviado...Ok",		"Sent...Ok"},
	{"¿Otro más?",			"Other one?"},
	{"Opciones: ",			"Options: "},
	{"VOLCANDO",			"SENDING"},
	{"GUARDAR como...",		"SAVE as..."},
	{"EQUIPOS GUARDADOS",	"SAVED DEVICES"},
	{"CIUDADES POR PAIS",	"CITIES BY COUNTRY"},
	{"Zona",	  			"Zone"},
	{"Ajustes",	  			"Settings"},
	{"Reloj",	  			"Clock"},
	{"Estaciones",			"Seasons"},
	{"Programa ", 			"Program "},
	{"Programas...",		"Programs..."},
	{"Ya existe",			"It exists"},
	{"¿Sobreescribir?",		"Overwrite?"},
	{"¿Reintentar?",		"Try again?"},
	{"¡Archivo guardado!",	"File saved!"},
	{"¡Memoria vacia!",		"Memory empty!"},
	{"Error guardando",		"Error saving"},
	{"Salir sin guardar",	"Exit witout saving"},
	{"Sincronización",		"Synchronization"},
	{"en progreso...",		"in progress..."},
	{"¿Sincr.Reloj?",		"Clock sync?"},
	{"¿Sincronizar?",		"Synchronize?"},
	{"Lat: ",				"Lat: "},
	{"Lon: ",				"Lon: "},
	{"Formateando...",		"Formatting..."},
	{"espere 60 seg.",		"wait 60 secs."},
	{"Cambiar",				"Replace"},
	{"Pilas",				"Batteries"},
	{"...Pulse Ok...",		"...Press Ok.."}
};

/*************************************************************************//**
 * @var		LSTBOX
 * @scope	public
 * @brief   Exported Array with texts for listbox options in different languages
 ****************************************************************************/
const char * const LSTBOX[][MAX_LANGUAGES] = {
	{" Equipos\n Configuración\n Localización\n",								" Devices\n Configuration\n Location\n"},
	{" Leer Equipo\n Equipo Guardado\n",										" Read Device\n Saved Device\n"},
	{" Fecha y Hora\n Idioma\n Contraste\n Borrar Memoria\n Versión\n",					" Date and Time\n Language\n Contrast\n Clear Memory\n Version\n"},
  #if DEVICE_MODEL == GPS_SAT
	{" Capturar coordenada GPS\n Enviar coordenada GPS\n Ciudades por país\n",	" GPS capture\n Send GPS coordinate\n Cities by country\n"},
  #else
	{" Ciudades por país\n",													" Cities by country\n"},
  #endif
	{" Español\n English\n",													" Español\n English\n"},
	{" Equipos guardados\n Formatear\n",										" Saved devices\n Format\n"}
};


static const uint8_t  languageList[] = {L_ESPANOL, L_INGLES, L_END};

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 									COUNTRIES, REGIONS AND CITIES	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

static const char * const countries = " ESPAÑA\n NORGE\n";
static const uint8_t  countryList[] = {EU_ESP, EU_NOR, COUNTRIES_END};

/****************************************************************************************************//**
 *                        	COUNTRY 0
 ********************************************************************************************************/
static const char * const regions_0 = " Andalucía\n Aragon\n Asturias\n Cantabria\n Castilla La Mancha\n Castilla León\n Cataluña\n Com. de Madrid\n Com. Valenciana\n Extremadura\n Galicia\n Islas Baleares\n Islas Canarias\n La Rioja\n Navarra\n Norte de Africa\n País Vasco\n Región de Murcia\n";


static const char * const cities_0[] = {	" Almeria\n Cádiz\n Cordoba\n Granada\n Huelva\n Jaén\n Málaga\n Sevilla\n",
											" Huesca\n Teruel\n Zaragoza\n",
											" Oviedo\n",
											" Santander\n",
											" Albacete\n Ciudad Real\n Cuenca\n Guadalajara\n Toledo\n",
											" Avila\n Burgos\n León\n Palencia\n Salamanca\n Segovia\n Soria\n Valladolid\n Zamora\n",
											" Barcelona\n Girona\n Lleida\n Tarragona\n",
											" Madrid\n",
											" Alacant\n Castelló\n Valencia\n",
											" Badajoz\n Cáceres\n",
											" A Coruña\n Lugo\n Ourense\n Pontevedra\n",
											" Mallorca\n",
											" Las Palmas\n Sta. Cruz Tenerife\n",
											" Logroño\n",
											" Pamplona\n",
											" Ceuta\n Melilla\n",
											" Bilbo\n Donosti\n Gasteiz\n",
											" Murcia\n"
										};

static const uint8_t codes_0[][9] 	= 	{	{3,10,14,19,21,23,30,43},
											{22,46,51},
											{34},
											{41},
											{1,13,15,20,47},
											{4,8,25,35,39,42,44,49,50},
											{6,18,26,45},
											{29},
											{2,11,48},
											{5,9},											   	  	
											{0,28,33,38},
											{36},
											{24,40},
											{27},
											{37},
											{12,31},
											{7,16,17},
											{32}
										};

/****************************************************************************************************//**
 *                        	COUNTRY 1
 ********************************************************************************************************/
static const char * const regions_1 = " Byer A a L\n Byer M a Z";
											
static const char * const cities_1[] = {	" Alesund\n Bergen\n Bodo\n Drammen\n Haugesund\n Krist.Sand\n Kristinas.\n Larvik\n Lillehamm.\n",
											" Mo i Rana\n Molde\n Oslo\n Roros\n Sarpsborg\n Stavanger\n Steinkjaer\n Trondheim\n"
										   };
static const uint8_t codes_1[][9] = {
											{16,0,1,2,3,4,5,6,7},
											{8,9,10,11,12,13,14,15}
										   };


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/*************************************************************************//**
 * @fun     GUI_GetCountries
 * @brief   Get countries array location
 *
 *
 * @return  array const addr
 ****************************************************************************/
char * GUI_GetCountries(void){
	return((char *)countries);
}

/*************************************************************************//**
 * @fun     GUI_GetRegions
 * @brief   Get regions from a countryID
 *
 *
 * @return  array const addr
 ****************************************************************************/
char * GUI_GetRegions(uint8_t countryID){
	switch(countryID){
		case 0:
			return((char *)regions_0);
		case 1:
			return((char *)regions_1);
	}
	return((char *)0);	
}

/*************************************************************************//**
 * @fun     GUI_GetCities
 * @brief   Get cities form regionID and  from a countryID
 *
 *
 * @return  array const addr
 ****************************************************************************/
char * GUI_GetCities(uint8_t countryID, uint8_t region_ID){
	switch(countryID){
		case 0:
			return((char *)cities_0[region_ID]);
		case 1:
			return((char *)cities_1[region_ID]);
	}
	return((char *)0);	
}

/*************************************************************************//**
 * @fun     GUI_GetCityCode
 * @brief   Get city codes form cityID, regionID and  from a countryID
 *
 *
 * @return  array const addr
 ****************************************************************************/
uint8_t GUI_GetCityCode(uint8_t countryID, uint8_t region_ID, uint8_t city_ID){
	switch(countryID){
		case 0:
			return(codes_0[region_ID][city_ID]);
		case 1:
			return(codes_1[region_ID][city_ID]);
	}
	return((uint8_t)-1);	
}

/*************************************************************************//**
 * @fun     GUI_GetCountryCode
 * @brief   Get country GLOBAL code from countryID
 * @param	countryID	country id
 *
 * @return  countryCode
 ****************************************************************************/
uint8_t GUI_GetCountryCode(uint8_t countryID){	
	return(countryList[countryID]);	
}

/*************************************************************************//**
 * @fun     GUI_GetCountryCode
 * @brief   Get country GLOBAL code from countryID
 * @param	countryID	country id
 *
 * @return  languageCode
 ****************************************************************************/
uint8_t GUI_GetLanguageCode(uint8_t langID){	
	return(languageList[langID]);	
}

#endif 

