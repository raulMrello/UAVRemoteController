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

#if DISTRIBUTION_MODEL == MODEL_EU0

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
	{"NO",					"NO",					"NAO", 					"NO",					"NEI",					"NEJ"},
	{"SI",					"YES",					"SIM",					"SI",					"JA",					"JA"},
	{"Volcar",				"Send",					"Enviar",				"Inviare",				"Send",					"Skicka"},
	{"Salvar",				"Save",					"Guardar",				"Salvare",				"Lagre",				"Spara"},
	{"Borrar",				"Erase",				"Apagar",				"Cancel.",				"Slett ",				"Radera"},
	{"¿Volcar?",			"Send?",				"Enviar?",				"Inviare?",				"Send?",				"Skicka"},
	{"GUARDAR...",			"SAVE...",				"GUARDAR...",			"SALVARE...",			"LAGRE...",				"SPARA..."},
	{"¿Borrar?",			"Delete?",				"Apagar?",				"Cancel.?",				"Slett?",				"Radera?"},
	{"¿Guardar cambios?",	"Save changes?",		"Guardar alteraçoes?",	"Salvare?",				"Lagre endringer?",		"Spara andringar?"},
	{"MENU: Pulse Ok...",	"MENU: press Ok..",		"MENU: carregue Ok.",	"MENU: premi Ok...",	"MENY: trykk Ok..",		"MENY: tryck pa Ok.."},
	{"MENU",				"MENU",					"MENU",					"MENU",					"MENY",					"MENY"},
	{"EQUIPOS",				"DEVICES",				"DISPOSITIVOS",			"DISPOSITIVOS",			"ENHETER",				"ENHERTER"},
	{"CONFIGURACION",		"CONFIG",				"CONFIGURAÇAO",			"CONFIGURAZIONE",		"KONFIG",				"KONFIG"},
	{"LOCALIZACION",		"LOCATIONS",			"LOCALIZAÇAO",			"LOCALIZZAZIONE",		"STEDER",				"PLATSER"},
	{"CAPTURA GPS",			"GPS CAPTURE",			"Captura GPS",			"CAPTARE GPS",			"GPS FANGST",			"GPS FANGA"},
	{"LOCALIZ. GUARDADA",	"SAVED POSITION",		"LOCALIZ. GUARDADA",	"LOCALIZZ. SALVATA",	"LAGREDE POSISJONEN",	"SPARADE POSITION"},
	{"IDIOMA",				"LANGUAGE",				"IDIOMA",				"LINGUA",				"SPRÅK",				"SPRAK"},
	{"FECHA/HORA",			"DATE/TIME",			"DATA/HORA",			"DATA/ORA",				"DATO/TID",				"DATUM/TID"},
	{"HORA",				"HOUR",					"HORA",					"ORA",					"TID",					"TIMME"},
	{"MINUTO",				"MINUTE",				"MINUTO",				"MINUTO",				"MINUTT",				"MINUTT"},
	{"DIA",					"DAY",					"DIA",					"GIORNO",				"DAG",					"DAG"},
	{"MES",					"MONTH",				"MES",					"MESE",					"MÅNED",				"MANAD"},
	{"AÑO",					"YEAR",					"ANO",					"ANNO",					"ÅR",					"AR"},
	{"BORRAR MEMORIA",		"DELETE MEMORY",		"APAGAR MEMORIA",		"CANCEL. MEMORIA",		"SLETTE MINNET",		"TA BORT MINNE"},
	{"VERSION",				"VERSION",				"VERSION",				"VERSIONE",				"VERSJON",				"VERSION"},
	{"CONTRASTE",			"CONTRAST",				"CONTRASTE",			"CONTRASTO",			"KONTRAST",				"KONTRAST"},
	{"LEYENDO",				"READING",				"A LER",				"LETTURA",				"LESING",				"LASNING"},
	{"Leído...Ok",			"Read...Ok",			"Lido...Ok",			"Leggere...Ok",			"Les...Ok",				"Las...Ok"},
	{"Enviado...Ok",		"Sent...Ok",			"Enviado...Ok",			"Inviato...Ok",			"Sendt...Ok",			"Skickas...Ok"},
	{"¿Otro más?",			"Other one?",			"Outro?",				"Altro?",				"Andre?",				"Andra en?"},
	{"Opciones: ",			"Options: ",			"Opçoes?",				"Opzioni: ",			"Opsjoner: ",			"Alternativ:"},
	{"VOLCANDO",			"SENDING",				"A ENVIAR",				"INVIO",				"SENDING",				"SANDANDE"},
	{"GUARDAR como...",		"SAVE as...",			"GUARDAR como...",		"SALVARE...",			"LAGRE som...",			"SPARA som..."},
	{"EQUIPOS GUARDADOS",	"SAVED DEVICES",		"DISPOSIT. GUARDADO",	"DISPOSIT. SALVATO",	"LAGREDE ENHETER",		"SPARADE ENHETER"},
	{"CIUDADES POR PAIS",	"CITIES BY COUNTRY",	"CIDADES POR PAIS",		"REGIONE E PROVINCIA",	"BYER ETTER LAND",		"STADER PER LAND"},
	{"Zona",	  			"Zone",					"Zona",					"Zona",					"Sone",					"Zon"},
	{"Ajustes",	  			"Settings",				"Definiçoes",			"Impostazioni",			"Innstilling",			"Konfig"},
	{"Reloj",	  			"Clock",				"Relogio",				"Orologio",				"Klokke",				"Tid"},
	{"Estaciones",			"Seasons",				"Estaçoes",				"Stagione",				"Arstid",				"Sasong"},
	{"Vacaciones",			"Holidays",				"Ferias",				"Vacanze",				"Ferie",				"Status"},
	{"Programa ", 			"Program ",				"Programa ",			"Programma ",			"Program ",				"Program "},
	{"Programas...",		"Programs...",			"Programas...",			"Programmi...",			"Programs...",			"Programs..."},
	{"Ya existe",			"It exists",			"Já existe",			"Gia esiste",			"Det finnes ",			"Den finns"},
	{"¿Sobreescribir?",		"Overwrite?",			"Sobrescrever?",		"Sostituire?",			"overskriv?",			"Skriva over?"},
	{"¿Reintentar?",		"Try again?",			"Tentar novamente?",	"Ritentare?",			"Prov pa nytt?",		"Forsok igen?"},
	{"¡Archivo guardado!",	"File saved!",			"Ficheiro guardado!",	"Archivio salvato!",	"Fil lagret!",			"Fil som sparats!"},
	{"¡Memoria vacia!",		"Memory empty!",		"Memória vazia!",		"Memoria vuota!",		"Minne tomt!",			"Minne tom!"},
	{"Error guardando",		"Error saving",			"Erro ao guardar",		"Salvat. errore",		"Feil ved lagring",		"Fel att spara"},
	{"Salir sin guardar",	"Exit without saving",	"Sair sem guardar", 	"Uscire senza salvare",	"Avslutt uten a lagre",	"Avsluta utan att spara"},
	{"Sincronización",		"Synchronization",		"Sincronizaçao",		"Sincronizzazione",		"Synkronisering ",		"Synkronisering"},
	{"en progreso...",		"in progress...",		"em progresso",			"in corso...",			"pagar...",				"pagar..."},
	{"¿Sincr.Reloj?",		"Clock sync?",			"Sincr.Relógio?",		"Sincr. Orologio?",		"Klokke synch?",		"Klocka synk?"},
	{"¿Sincronizar?",		"Synchronize?",			"Sincronizar?",			"Sincroniz.?",			"Synkronizer?",			"Synkronisera?"},
	{"Lat: ",				"Lat: ",				"Lat: ",				"Lat: ",				"Brd: ",				"Lat: "},
	{"Lon: ",				"Lon: ",				"Lon: ",				"Lon: ",				"Lng: ",				"Lon: "},
	{"Formateando...",		"Formatting...",		"Formataçao",			"Formattare...",		"Formatering...",		"Formatering..."},
	{"El borrado puede",	"Erasing can take",		"Apagamento pode",		"Cancellare memoria",	"Slettingen kan",		"Raderingen kan"},
	{"durar unos 60 seg.",	"arround 60 seconds.",	"demorar 60 seg.",		"attendere 60 secondi.","ta 60 sekunder.",		"ta 60 sekunder."},
	{"Cambiar",				"Replace",				"Substituir",			"Sostituire",			"Erstatte",				"Ersatta"},
	{"Pilas",				"Batteries",			"Baterias",				"Batterie",				"Batteri",				"Batteri"},
	{"...Pulse Ok...",		"...Press Ok...",		"...Carregue Ok...",	"...Premi Ok...",		"...Trykk Ok..",		"...Press Ok.."}
};

/*************************************************************************//**
 * @var		LSTBOX
 * @scope	public
 * @brief   Exported Array with texts for listbox options in different languages
 ****************************************************************************/
const char * const LSTBOX[][MAX_LANGUAGES] = {
	{" Equipos\n Configuración\n Localización\n",								" Devices\n Configuration\n Location\n",							" Dispositivo\n Configuraçao\n Localizaçao\n",					    " Dispositivo\n Configurazione\n Localizzazione\n",						" Enheter\n Konfigurasjon\n Plassering\n",								" Enheter\n Konfiguration\n Plats\n"},
	{" Leer Equipo\n Equipo Guardado\n",										" Read Device\n Saved Device\n",							        " Leitura do dispositivo\n Dispositivo guardado\n",					" Leggere Strumento\n Dato Salvato\n",									" Les Enhet\n Lagret enhet\n",											" Las Enhet\n Sparade Enhet\n"},
	{" Fecha y Hora\n Idioma\n Contraste\n Borrar Memoria\n Versión\n",		    " Date and Time\n Language\n Contrast\n Clear Memory\n Version\n",	" Data e Hora\n Idioma\n Contraste\n Apagar Memoria\n Version\n",	" Data e Ora\n Lingua\n Contrasto\n Cancellare Memoria\n Versione\n",	" Dato og Klokkeslett\n Sprak\n Kontrast\n Tomme minnet\n Versjon\n",	" Datum och Tid\n Sprak\n Kontrast\n Ren Minne\n Version\n"},
  #if DEVICE_MODEL == GPS_SAT
	{" Capturar coordenada GPS\n Enviar coordenada GPS\n Ciudades por país\n",	" GPS capture\n Send GPS coordinate\n Cities by country\n",		    " Captura GPS\n Enviar coord. GPS\n Cidades por país\n",		    " Captare GPS\n Inviare coord. GPS\n Regione e Provincia\n",			" GPS fangst\n Send GPS koordinere\n Byer etter land\n",				" GPS fanga\n Skicka GPS samordna\n Stader efter land\n"},
  #else
	{" Ciudades por país\n",													" Cities by country\n",												" Cidades por país\n",										        " Regione e Provincia\n",												" Byer etter land\n",													" Stader efter land\n"},
  #endif
	{" Español\n English\n Portugues\n Italiano\n Norske\n Svenska\n",			" Español\n English\n Portugues\n Italiano\n Norske\n Svenska\n",	" Español\n English\n Portugues\n Italiano\n Norske\n Svenska\n",	" Español\n English\n Portugues\n Italiano\n Norske\n Svenska\n",		" Español\n English\n Portugues\n Italiano\n Norske\n Svenska\n",		" Español\n English\n Portugues\n Italiano\n Norske\n Svenska\n"},
	{" Equipos guardados\n Formatear\n",										" Saved devices\n Format\n",										" Dispositivo guardado\n Formataçao\n",								" Dispositivi salvati\n Formattare\n",									" Lagrede enheter\n Format\n",											" Sparade produkter\n Format\n"}
};


static const uint8_t  languageList[] = {L_ESPANOL, L_INGLES, L_PORTUGUES, L_ITALIANO, L_NORUEGO, L_SUECO, L_END};

/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 									COUNTRIES, REGIONS AND CITIES	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

static const char * const countries = "\n ESPAÑA\n ITALIA\n PORTUGAL\n NORGE\n SVERIGE\n";
static const uint8_t  countryList[] = {EU_ESP, EU_ITA, EU_POR, EU_NOR, EU_SVE, COUNTRIES_END};
static const int8_t   countryGMT[] =  {	  1,	  1,	  0, 	  1,      1,     	-1};


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

/****************************************************************************************************//**
 *                        	COUNTRY 3 (NORGE)
 ********************************************************************************************************/
static const char * const regions_3 = " Byer fra A til L\n Byer fra M til Z\n";


static const char * const cities_3[] = {	" Alesund\n Bergen\n Drammen\n Haugesund\n Kristiansand\n Kristiansund\n Larvik\n Lillehammer\n",
											" Molde\n Oslo\n Roros\n Sarpsborg\n Stavanger\n Steinkjaer\n Trondheim\n"
										};

static const uint8_t codes_3[][8] 	= 	{	{0,1,2,3,4,5,6,7},
											{8,9,10,11,12,13,14}
										};

/****************************************************************************************************//**
 *                        	COUNTRY 4
 ********************************************************************************************************/
static const char * const regions_4 = " Stader fran A till L\n Stader fran M till Z\n";
											
static const char * const cities_4[] = {	" Gavle\n Goteborg\n Halmstad\n Helsinborg\n Jonkoping\n Kalmar\n Karlstad\n Kristianstad\n Linkoping\n",
											" Malmo\n Mora\n Norrkoping\n Ostersund\n Skelleftea\n Stockholm\n Sundsvall\n Umea\n Uppsala\n Vaxjo\n"
										   };
static const uint8_t codes_4[][10] = {
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
		case 2:
			return((char *)regions_2);
		case 3:
			return((char *)regions_3);
		case 4:
			return((char *)regions_4);
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
		case 3:
			return((char *)cities_3[region_ID]);
		case 4:
			return((char *)cities_4[region_ID]);
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
		case 3:
			return(codes_3[region_ID][city_ID]);
		case 4:
			return(codes_4[region_ID][city_ID]);
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

