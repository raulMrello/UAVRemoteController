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

#if DISTRIBUTION_MODEL == MODEL_NOR_SVE

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
	{"NEI",					"NEJ",						"NO"},
	{"JA",					"JA",						"YES"},
	{"Send",				"Skicka",					"Send"},
	{"Lagre",				"Spara",					"Save"},
	{"Slett ",				"Radera",					"Erase"},
	{"Send?",				"Skicka",					"Send?"},
	{"LAGRE...",			"SPARA...",					"SAVE..."},
	{"Slett?",				"Radera?",					"Delete?"},
	{"Lagre endringer?",	"Spara andringar?",			"Save changes?"},
	{"MENY: trykk Ok..",	"MENY: tryck pa Ok..",		"MENU: press Ok.."},
	{"MENY",				"MENY",						"MENU"},
	{"ENHETER",				"ENHERTER",					"DEVICES"},
	{"KONFIG",				"KONFIG",					"CONFIG"},
	{"STEDER",				"PLATSER",					"LOCATIONS"},
	{"GPS FANGST",			"GPS FANGA",				"GPS CAPTURE"},
	{"LAGREDE POSISJONEN",	"SPARADE POSITION",			"SAVED POSITION"},
	{"SPRÅK",				"SPRAK",					"LANGUAGE"},
	{"DATO/TID",			"DATUM/TID",				"DATE/TIME"},
	{"TID",					"TIMME",					"HOUR"},
	{"MINUTT",				"MINUTT",					"MINUTE"},
	{"DAG",					"DAG",						"DAY"},
	{"MÅNED",				"MANAD",					"MONTH"},
	{"ÅR",					"AR",						"YEAR"},
	{"SLETTE MINNET",		"TA BORT MINNE",			"DELETE MEMORY"},
	{"VERSJON",				"VERSION",					"VERSION"},
	{"KONTRAST",			"KONTRAST",					"CONTRAST"},
	{"LESING",				"LASNING",					"READING"},
	{"Les...Ok",			"Las...Ok",					"Read...Ok"},
	{"Sendt...Ok",			"Skickas...Ok",				"Sent...Ok"},
	{"Andre?",				"Andra en?",				"Other one?"},
	{"Opsjoner: ",			"Alternativ:",				"Options: "},
	{"SENDING",				"SANDANDE",					"SENDING"},
	{"LAGRE som...",		"SPARA som...",				"SAVE as..."},
	{"LAGREDE ENHETER",		"SPARADE ENHETER",			"SAVED DEVICES"},
	{"BYER ETTER LAND",		"STADER PER LAND",			"CITIES BY COUNTRY"},
	{"Sone",				"Zon",	  					"Zone"},
	{"Innstilling",			"Konfig",	  				"Settings"},
	{"Klokke",				"Tid",	  					"Clock"},
	{"Arstid",				"Sasong",					"Seasons"},
	{"Ferie",				"Status",					"Holidays"},
	{"Program ",			"Program ", 				"Program "},
	{"Programs...",			"Programs...",				"Programs..."},
	{"Det finnes ",			"Den finns",				"It exists"},
	{"overskriv?",			"Skriva over?",				"Overwrite?"},
	{"Prov pa nytt?",		"Forsok igen?",				"Try again?"},
	{"Fil lagret!",			"Fil som sparats!",			"File saved!"},
	{"Minne tomt!",			"Minne tom!",				"Memory empty!"},
	{"Feil ved lagring",	"Fel att spara",			"Error saving"},
	{"Avslutt uten a lagre","Avsluta utan att spara",	"Exit without saving"},
	{"Synkronisering ",		"Synkronisering",			"Synchronization"},
	{"pagar...",			"pagar...",					"in progress..."},
	{"Klokke synch?",		"Klocka synk?",				"Clock sync?"},
	{"Synkronizer?",		"Synkronisera?",			"Synchronize?"},
	{"Brd: ",				"Lat: ",					"Lat: "},
	{"Lng: ",				"Lon: ",					"Lon: "},
	{"Formatering...",		"Formatering...",			"Formatting..."},
	{"venter 60 sek.",		"vanta 60 sek.",			"wait 60 secs."},
	{"Erstatte",			"Ersatta",					"Replace"},
	{"Batteri",				"Batteri",					"Batteries"},
	{"...Trykk Ok..",		"...Press Ok..",			"...Press Ok.."}
};

/*************************************************************************//**
 * @var		LSTBOX
 * @scope	public
 * @brief   Exported Array with texts for listbox options in different languages
 ****************************************************************************/
const char * const LSTBOX[][MAX_LANGUAGES] = {
	{" Enheter\n Konfigurasjon\n Plassering\n",								" Enheter\n Konfiguration\n Plats\n",							" Devices\n Configuration\n Location\n"},
	{" Les Enhet\n Lagret enhet\n",											" Las Enhet\n Sparade Enhet\n",									" Read Device\n Saved Device\n"},
	{" Dato og Klokkeslett\n Sprak\n Kontrast\n Tomme minnet\n Versjon\n",	" Datum och Tid\n Sprak\n Kontrast\n Ren Minne\n Version\n",	" Date and Time\n Language\n Contrast\n Clear Memory\n Version\n"},
  #if DEVICE_MODEL == GPS_SAT
  	{" GPS fangst\n Send GPS koordinere\n Byer etter land\n",				" GPS fanga\n Skicka GPS samordna\n Stader efter land\n",		" GPS capture\n Send GPS coordinate\n Cities by country\n"},
  #else
  	{" Byer etter land\n",													" Stader efter land\n",											" Cities by country\n"},
  #endif
	{" Norske\n Sverige\n Engelsk\n",										" Norska\n Svenska\n Engelska\n",								" Norsk\n Svenska\n English\n"},
	{" Lagrede enheter\n Format\n",											" Sparade produkter\n Format\n",								" Saved devices\n Format\n"}
};


static const uint8_t  languageList[] = {L_NORUEGO, L_SUECO, L_INGLES, L_END};

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 									COUNTRIES, REGIONS AND CITIES	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

static const char * const countries = "\n NORGE\n SVERIGE\n";
static const uint8_t  countryList[] = {EU_NOR, EU_SVE, COUNTRIES_END};
static const int8_t   countryGMT[] =  {	  1,	  1,	  -1};

/****************************************************************************************************//**
 *                        	COUNTRY 0 (NORGE)
 ********************************************************************************************************/
static const char * const regions_0 = " Byer fra A til L\n Byer fra M til Z\n";


static const char * const cities_0[] = {	" Alesund\n Bergen\n Drammen\n Haugesund\n Kristiansand\n Kristiansund\n Larvik\n Lillehammer\n",
											" Molde\n Oslo\n Roros\n Sarpsborg\n Stavanger\n Steinkjaer\n Trondheim\n"
										};

static const uint8_t codes_0[][8] 	= 	{	{0,1,2,3,4,5,6,7},
											{8,9,10,11,12,13,14}
										};

/****************************************************************************************************//**
 *                        	COUNTRY 1
 ********************************************************************************************************/
static const char * const regions_1 = " Stader fran A till L\n Stader fran M till Z\n";
											
static const char * const cities_1[] = {	" Gavle\n Goteborg\n Halmstad\n Helsinborg\n Jonkoping\n Kalmar\n Karlstad\n Kristianstad\n Linkoping\n",
											" Malmo\n Mora\n Norrkoping\n Ostersund\n Skelleftea\n Stockholm\n Sundsvall\n Umea\n Uppsala\n Vaxjo\n"
										   };
static const uint8_t codes_1[][10] = {
											{0,1,2,3,4,5,6,7,8},
											{9,10,11,12,13,14,15,16,17,18}
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
	return((char *)((uint32_t)countries)+1);
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
 * @fun     GUI_GetCountryGMT
 * @brief   Get country GMT correction
 * @param	countryID	country id
 *
 * @return  GMT correction
 ****************************************************************************/
int8_t GUI_GetCountryGMT(uint8_t countryID){	
	return(countryGMT[countryID]);	
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

