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

#if DISTRIBUTION_MODEL == MODEL_ESP_POR_ITA

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
	{"NO",					"NO",					"NO",					"NAO"},
	{"SI",					"YES",					"SI",					"SIM"},
	{"Volcar",				"Send",					"Inviare",				"Enviar"},
	{"Salvar",				"Save",					"Salvare",				"Guardar"},
	{"Borrar",				"Erase",				"Cancel.",				"Apagar"},
	{"¿Volcar?",			"Send?",				"Inviare?",				"Enviar?"},
	{"GUARDAR...",			"SAVE...",				"SALVARE...",			"GUARDAR..."},
	{"¿Borrar?",			"Delete?",				"Cancel.?",				"Apagar?"},
	{"¿Guardar cambios?",	"Save changes?",		"Salvare?",				"Guardar alteraçoes?"},
	{"MENU: Pulse Ok...",	"MENU: press Ok..",		"MENU: premi Ok...",	"MENU: carregue Ok."},
	{"MENU",				"MENU",					"MENU",					"MENU"},
	{"EQUIPOS",				"DEVICES",				"DISPOSITIVOS",			"DISPOSITIVOS"},
	{"CONFIGURACION",		"CONFIG",				"CONFIGURAZIONE",		"CONFIGURAÇAO"},
	{"LOCALIZACION",		"LOCATIONS",			"LOCALIZZAZIONE",		"LOCALIZAÇAO"},
	{"CAPTURA GPS",			"GPS CAPTURE",			"CAPTARE GPS",			"Captura GPS"},
	{"LOCALIZ. GUARDADA",	"SAVED POSITION",		"LOCALIZZ. SALVATA",	"LOCALIZ. GUARDADA"},
	{"IDIOMA",				"LANGUAGE",				"LINGUA",				"IDIOMA"},
	{"FECHA/HORA",			"DATE/TIME",			"DATA/ORA",				"DATA/HORA"},
	{"HORA",				"HOUR",					"ORA",					"HORA"},
	{"MINUTO",				"MINUTE",				"MINUTO",				"MINUTO"},
	{"DIA",					"DAY",					"GIORNO",				"DIA"},
	{"MES",					"MONTH",				"MESE",					"MES"},
	{"AÑO",					"YEAR",					"ANNO",					"ANO"},
	{"BORRAR MEMORIA",		"DELETE MEMORY",		"CANCEL. MEMORIA",		"APAGAR MEMORIA"},
	{"VERSION",				"VERSION",				"VERSIONE",				"VERSION"},
	{"CONTRASTE",			"CONTRAST",				"CONTRASTO",			"CONTRASTE"},
	{"LEYENDO",				"READING",				"LETTURA",				"A LER"},
	{"Leído...Ok",			"Read...Ok",			"Leggere...Ok",			"Lido...Ok"},
	{"Enviado...Ok",		"Sent...Ok",			"Inviato...Ok",			"Enviado...Ok"},
	{"¿Otro más?",			"Other one?",			"Altro?",				"Outro?"},
	{"Opciones: ",			"Options: ",			"Opzioni: ",			"Opçoes?"},
	{"VOLCANDO",			"SENDING",				"INVIO",				"A ENVIAR"},
	{"GUARDAR como...",		"SAVE as...",			"SALVARE...",			"GUARDAR como..."},
	{"EQUIPOS GUARDADOS",	"SAVED DEVICES",		"DISPOSIT. SALVATO",	"DISPOSIT. GUARDADO"},
	{"CIUDADES POR PAIS",	"CITIES BY COUNTRY",	"REGIONE E PROVINCIA",	"CIDADES POR PAIS"},
	{"Zona",	  			"Zone",					"Zona",					"Zona"},
	{"Ajustes",	  			"Settings",				"Impostazioni",			"Definiçoes"},
	{"Reloj",	  			"Clock",				"Orologio",				"Relogio"},
	{"Estaciones",			"Seasons",				"Stagione",				"Estaçoes"},
	{"Vacaciones",			"Holidays",				"Vacanze",				"Ferias"},
	{"Programa ", 			"Program ",				"Programma ",			"Programa "},
	{"Programas...",		"Programs...",			"Programmi...",			"Programas..."},
	{"Ya existe",			"It exists",			"Gia esiste",			"Já existe"},
	{"¿Sobreescribir?",		"Overwrite?",			"Sostituire?",			"Sobrescrever?"},
	{"¿Reintentar?",		"Try again?",			"Ritentare?",			"Tentar novamente?"},
	{"¡Archivo guardado!",	"File saved!",			"Archivio salvato!",	"Ficheiro guardado!"},
	{"¡Memoria vacia!",		"Memory empty!",		"Memoria vuota!",		"Memória vazia!"},
	{"Error guardando",		"Error saving"			"Salvat. errore",		"Erro ao guardar"},
	{"Salir sin guardar",	"Exit without saving", 	"Uscire senza salvare",	"Sair sem guardar"},
	{"Sincronización",		"Synchronization",		"Sincronizzazione",		"Sincronizaçao"},
	{"en progreso...",		"in progress...",		"in corso...",			"em progresso"},
	{"¿Sincr.Reloj?",		"Clock sync?",			"Sincr. Orologio?",		"Sincr.Relógio?"},
	{"¿Sincronizar?",		"Synchronize?",			"Sincroniz.?",			"Sincronizar?"},
	{"Lat: ",				"Lat: ",				"Lat: ",				"Lat: "},
	{"Lon: ",				"Lon: ",				"Lon: ",				"Lon: "},
	{"Formateando...",		"Formatting...",		"Formattare...",		"Formataçao"},
	{"El borrado puede",	"Erasing can take",		"Cancellare memoria,",	"Apagamento pode"},
	{"durar unos 60 seg.",	"arround 60 seconds.",	"attendere 60 secondi.","demorar 60 seg."},
	{"Cambiar",				"Replace",				"Sostituire",			"Substituir"},
	{"Pilas",				"Batteries",			"Batterie",				"Baterias"},
	{"...Pulse Ok...",		"...Press Ok...",		"...Premi Ok...",		"...Carregue Ok..."}
};

/*************************************************************************//**
 * @var		LSTBOX
 * @scope	public
 * @brief   Exported Array with texts for listbox options in different languages
 ****************************************************************************/
const char * const LSTBOX[][MAX_LANGUAGES] = {
	{" Equipos\n Configuración\n Localización\n",								" Devices\n Configuration\n Location\n",					        " Dispositivo\n Configurazione\n Localizzazione\n",						" Dispositivo\n Configuraçao\n Localizaçao\n"},
	{" Leer Equipo\n Equipo Guardado\n",										" Read Device\n Saved Device\n",							        " Leggere Strumento\n Dato Salvato\n",							        " Leitura do dispositivo\n Dispositivo guardado\n"},
	{" Fecha y Hora\n Idioma\n Contraste\n Borrar Memoria\n Versión\n",		    " Date and Time\n Language\n Contrast\n Clear Memory\n Version\n",	" Data e Ora\n Lingua\n Contrasto\n Cancellare Memoria\n Versione\n",	" Data e Hora\n Idioma\n Contraste\n Apagar Memoria\n Version\n"},
  #if DEVICE_MODEL == GPS_SAT
	{" Capturar coordenada GPS\n Enviar coordenada GPS\n Ciudades por país\n",	" GPS capture\n Send GPS coordinate\n Cities by country\n",		    " Captare GPS\n Inviare coord. GPS\n Regione e Provincia\n",		    " Captura GPS\n Enviar coord. GPS\n Cidades por país\n"},
  #else
	{" Ciudades por país\n",													" Cities by country\n",										        " Regione e Provincia\n",												" Cidades por país\n"},
  #endif
	{" Español\n English\n Italiano\n Portugues\n",								" Español\n English\n Italiano\n Portugues\n",						" Español\n English\n Italiano\n Portugues\n",							" Español\n English\n Italiano\n Portugues\n"},
	{" Equipos guardados\n Formatear\n",										" Saved devices\n Format\n",								        " Dispositivi salvati\n Formattare\n",									" Dispositivo guardado\n Formataçao\n"}
};


static const uint8_t  languageList[] = {L_ESPANOL, L_INGLES, L_ITALIANO, L_END};

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 									COUNTRIES, REGIONS AND CITIES	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

static const char * const countries = "\n ESPAÑA\n ITALIA\n PORTUGAL\n";
static const uint8_t  countryList[] = {EU_ESP, EU_ITA, EU_POR, COUNTRIES_END};
static const int8_t   countryGMT[] =  {	  1,	  1,	  0, 		-1};

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
static const char * const regions_1 = " Abruzzo\n Basilicata\n Calabria\n Campania\n Emilia-Romagna\n Friuli-Venezia Giulia\n Lazio\n Liguria\n Lombardia\n Marche\n Molise\n Piemonte\n Puglia\n Sardegna\n Sicilia\n Toscana\n Trentino-Alto Adige\n Umbria\n Valle d.Aosta\n Veneto\n";
											
static const char * const cities_1[] = {	" Chieti\n L.Aquila\n Pescara\n",
											" Potenza\n",
											" Catanzaro\n Reggio Calabria\n",
											" Caserta\n Napoli\n Salerno\n",
											" Bologna\n Modena\n Parma\n Piacenza\n Reggio Emilia\n Rimini\n",
											" Pordenone\n Trieste\n Udine\n",
											" Frosinone\n Latina\n Rieti\n Roma\n Viterbo\n",
											" Genova\n Imperia\n La Spezia\n Savona\n",
											" Bergamo\n Brescia\n Como\n Cremona\n Lecco\n Milano\n Pavia\n Sondrio\n Varese\n",
											" Ancona\n Pesaro e Urbino\n",
											" Campobasso\n",
											" Alessandria\n Asti\n Cuneo\n Novara\n Torino\n Vercelli\n",
											" Bari\n Brindisi\n Foggia\n Lecce\n",
											" Cagliari\n Nuoro\n Sassari\n",
											" Agrigento\n Catania\n Messina\n Palermo\n Ragusa\n Siracusa\n Trapani\n",
											" Arezzo\n Firenze\n Livorno\n Lucca\n Massa-Carrara\n Pisa\n Pistoia\n Prato\n Siena\n",
											" Bolzano\n Trento\n",
											" Perugia\n Terni\n",
											" Aosta\n",
											" Belluno\n Padova\n Rovigo\n Treviso\n Venezia\n Verona\n Vicenza\n"
										   };
static const uint8_t codes_1[][9] = {
											{18,28,47},
											{52},
											{17,55},
											{15,38,61},
											{9,37,43,48,56,58},
											{51,72,73},
											{24,29,57,59,79},
											{25,26,27,63},
											{8,11,19,20,31,36,44,66,74},
											{2,46},
											{14},
											{1,5,21,39,68,76},
											{6,12,23,30},
											{13,40,62},
											{0,16,35,42,54,65,69},
											{4,22,32,33,34,49,50,53,64},
											{10,70},
											{45,67},
											{3},
											{7,41,60,71,75,77,78},
											{255}
										   };

/****************************************************************************************************//**
 *                        	COUNTRY 2
 ********************************************************************************************************/
static const char * const regions_2	 = " Regiao do Norte\n Regiao de Lisboa\n Algarve\n Islas\n";

static const char * const cities_2[] = 	{	" Porto\n",
											" Lisboa\n",
											" Faro\n",
											" Angra do Heroísmo\n Funchal\n Ponta Delgada\n Sta Cruz da Graciosa\n"
										};
static const uint8_t codes_2[][4] = {
											{2},
											{1},
											{0},
											{3,4,5,6}
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
		case 2:
			return((char *)regions_2);
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
		case 2:
			return((char *)cities_2[region_ID]);
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
		case 2:
			return(codes_2[region_ID][city_ID]);
	}
	return((uint8_t)-1);	
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

