/*************************************************************************//**
 * @file    Lib_GPS.c
 * @brief   GPS Library
 * @date    11.06.2010
 * @author  Raúl M.
 ****************************************************************************/

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include "Lib_GPS.h"

/****************************************************************************************************//**
 *                        PRIVATE MEMBERS
 ********************************************************************************************************/



#define HEADER_GPRMC	"$GPRMC"

const STRGps gps_init = {
	0,					// estado
	0,					// error
	{"0000.0000","N","00000.0000","E"},	// Posicion
	"000000",			// Hora
	0,					// Satelites
	"0.0",				// Altura
	"0.0",				// Velocidad
	"000000",			// fecha
	"0.0",				// hdop
	"0.0"				// rumbo
};


#define MAX_BUFFER_GPS 	128

// Captura de tramas
static uint8_t 	gpsBuffer[MAX_BUFFER_GPS];
static char 	buffer_aux[15];
static uint8_t	sentenceReady = 0;
STRPosicion 	GPS_pos;

// Funciones locales
static uint8_t 	TratarGPS_GPRMC(STRGps *gps, unsigned char *trama);
//static uint8_t 	TratarGPS_GPGGA(STRGps *gps, unsigned char *trama);
//static uint8_t 	TratarGPS_GPZDA(STRGps *gps, unsigned char *trama);


/********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************
 										IMPLEMENTATION	
 ********************************************************************************************************
 ********************************************************************************************************
 ********************************************************************************************************/

/****************************************************************************************************//**
 * @fun		GPS_GetData
 * @brief	Interrupt due to a character reception
 * @param	data	character received	
 * @return	sentenceReady
 **********************************************************************************/
uint8_t GPS_GetData(uint8_t data){
	static uint16_t bufferPos = 0;
	// inicio de trama... siempre en [0]
	sentenceReady = 0;
	if(data == '$'){
		bufferPos = 0;
		gpsBuffer[bufferPos++] = data;
	}
	// fin de trama, el próximo irá a [0]
	else if(data == '\n'){
		bufferPos = 0;
		sentenceReady = 1;
	}
	// datos de trama al buffer
	else if(bufferPos > 0 && bufferPos < MAX_BUFFER_GPS){
		gpsBuffer[bufferPos++] = data;
	}
	// buffer overflow... resetear y descartar
	else{
		bufferPos = 0;
	}
	return sentenceReady;
}

/****************************************************************************************************//**
 * @fun		GPS_CheckFrame
 * @brief	Checks for a correct GPS frame and notify to GUI manager through IDataReady interface.
 * 
 * @param	gpsPos	destination data structure
 * @param	INotify	callback to invoke once data is ready
 * @return  true: if a sentence is received, false: if no sentences received
 *
 **********************************************************************************/
void GPS_CheckFrame(STRGps *gpsPos, void (*INotify)(uint8_t *,uint16_t)){
	uint8_t errno;
	if(!sentenceReady)	return;
	if(strncmp((const char *)gpsBuffer, (const char *)HEADER_GPRMC, 6) == 0){
		errno = TratarGPS_GPRMC(gpsPos, gpsBuffer);	// Tratamiento $GPRMC
		if(!errno){
			INotify((uint8_t*)gpsPos, (uint16_t)sizeof(STRGps));
		}
	}
}

/****************************************************************************************************//**
 * @fun		GPS_LatitudeToString
 * @brief	Convert latitud to string format: GGº MM' N or GGº MM' S
 * @param	gpsPos	GPS data structure
 * @param	lat		destination string
 * @return	
 **********************************************************************************/
void GPS_LatitudeToString(STRGps *gpsPos, char *lat){
	strncpy(lat, &gpsPos->Posicion.latitud[0],2);
	lat[2] = 0;
	strcat(lat, "º ");
	strncat(lat, &gpsPos->Posicion.latitud[2],2);
	strcat(lat, "' ");
	strcat(lat, gpsPos->Posicion.latitudC);
}

/****************************************************************************************************//**
 * @fun		GPS_LongitudeToString
 * @brief	Convert longitude to string format: GGGº MM' E or GGGº MM' W
 * @param	gpsPos	GPS data structure
 * @param	lon		destination string
 * @return	
 **********************************************************************************/
void GPS_LongitudeToString(STRGps *gpsPos, char *lon){
			strncpy(lon, &gpsPos->Posicion.longitud[0],3);
			lon[3] = 0;
			strcat(lon, "º ");
			strncat(lon, &gpsPos->Posicion.longitud[3],2);
			strcat(lon, "' ");
			strcat(lon, gpsPos->Posicion.longitudC);
}

/****************************************************************************************************//**
 * @fun		TratarGPS_GPGGA
 * @brief	Process GPGGA stream
 * @param	trama		stream to be precessed
 * @return	
 **********************************************************************************/
//
//static uint8_t TratarGPS_GPGGA(STRGps *gps, unsigned char *trama){
//	char *ptr1,*ptr2;
//
//	// parte del estado de no validez
//	gps->estado=GPS_NOPOS;
//
//	// descarta $GPGGA
//	trama += 7;
//
//    /*=====================LEE UTC ==================================*/
//    ptr2 = memchr (trama,',',12);           /* cabezera              */
//    if (ptr2==0){
//	    return 1;                            /* error sintaxis termina*/
//	  }				 
//	if(ptr2-(char*)trama != 10)
//		return 1;
//    *ptr2 = 0;                              /* termina cadena        */
//	 strcpy(gps->hora,(const char*)trama);				/* guarda hasta dato valido*/
//
//    /*=====================LEE LATITUD ==============================*/
//    ptr1=ptr2;
//    ptr1++;                                 /* inicio                */
//    ptr2 = memchr (ptr1,',',15);            
//    if (ptr2==0){
//	 return 1;                   /* error sintaxis        */
//	}
// 	if(ptr2-ptr1 > (sizeof(GPS_pos.latitud)-1))
//		return 1;
//  	*ptr2=0;								/* termina cadena        */
//	strcpy(gps->Posicion.latitud,ptr1);
//	GPS_pos.latitud[9] = 0;
//    /*=====================LEE LATITUDC==============================*/
//    ptr1=ptr2;
//    ptr1++;                                 /* inicio                */
//    ptr2 = memchr (ptr1,',',2); 
//    if (ptr2==0 || ptr2-ptr1 > (sizeof(GPS_pos.latitudC)-1) ){
//	 return 1;                   /* error sintaxis        */
//	 }
//	  *ptr2=0;								/* termina cadena        */
//	  strcpy(gps->Posicion.latitudC,ptr1);
//    /*=====================LEE LONGITUD==============================*/
//    ptr1=ptr2;
//    ptr1++;                                 /* inicio                */
//    ptr2 = memchr (ptr1,',',15);            /* fin                   */
//    if (ptr2==0) 
//		return 1;                   /* error sintaxis        */
// 	if(ptr2-ptr1 > (sizeof(GPS_pos.longitud)-1))
//		return 1;
//
//  	*ptr2=0;								/* termina cadena        */
//	strcpy(gps->Posicion.longitud,ptr1);
//	GPS_pos.longitud[10] = 0;
//    /*=====================LEE LONGITUDC=============================*/
//    ptr1=ptr2;
//    ptr1++;                                 /* inicio                */
//    ptr2 = memchr (ptr1,',',2);            
//    if (ptr2==0 ) 
//		return 1;                   /* error sintaxis        */
// 	if(ptr2-ptr1 > (sizeof(GPS_pos.longitudC)-1))
//		return 1;
//	  *ptr2=0;								/* termina cadena        */
//	  strcpy(gps->Posicion.longitudC,ptr1);
//    /*=====================LEE VALIDEZ ==============================*/
//    ptr1=ptr2;
//    ptr1++;                                 /* inicio                */
//    ptr2 = memchr (ptr1,',',3);             
//    *ptr2 = 0;                              /* termina cadena        */
//    if (*ptr1<'1'){
//		  gps->estado=GPS_NOPOS;
//		  return 1;
//	}
//	gps->estado=GPS_OK;
//	// paso del resto de la trama
//	return 0;
////    /*=====================LEE NUMERO DE SATELITES===================*/
////    ptr1=ptr2;
////    ptr1++;                                 /* inicio                */
////    ptr2 = memchr (ptr1,',',4);            
////    if (ptr2==0) return 1;                   /* error sintaxis        */
////	  *ptr2=0;								/* termina cadena        */
////	//gps->satelites=atoi(ptr1);
////    /*=====================LEE HDOP==================================*/
////    ptr1=ptr2;
////    ptr1++;                                 /* inicio                */
////    ptr2 = memchr (ptr1,',',10);            
////    if (ptr2==0) return 1;                   /* error sintaxis        */
////	  *ptr2=0;								/* termina cadena        */
//// 	  strcpy(gps->hdop,ptr1);					/* copia cadena          */	
////    /*=====================ALTURA====================================*/
////    ptr1=ptr2;
////    ptr1++;                                 /* inicio                */
////    ptr2 = memchr (ptr1,',',10);            
////    if (ptr2==0) return 1;                   /* error sintaxis        */
////	  *ptr2=0;								/* termina cadena        */
//// 	strcpy(gps->altura,ptr1);				/* copia cadena          */
////	return 0;
//}


/****************************************************************************************************//**
 * @fun		TratarGPS_GPRMC
 * @brief	Process GPRMC stream
 * @param	trama		stream to be precessed
 * @return	
 **********************************************************************************/
static uint8_t TratarGPS_GPRMC(STRGps *gps, unsigned char *trama){
	char *ptr1,*ptr2;

	// parte del estado no válido
	gps->estado=GPS_NOPOS;

	// descarta $GPRMC
	trama += 7;

    /*=====================LEE UTC ==================================*/
    ptr2 = memchr (trama,',',12);           /* cabezera              */
    if (ptr2==0)
	    return 1;                            /* error sintaxis termina*/
	//if(ptr2-(char*)trama != 10)
	//	return 1;
	*ptr2 = 0;                              /* termina cadena        */
	strncpy(buffer_aux,(const char*)trama,6);		// guarda formato HHMMSS
	*(buffer_aux+6) = 0;							//termina con 0
    /*=====================LEE VALIDEZ ==============================*/
    ptr1=ptr2;
    ptr1++;                                 /* inicio                */
    ptr2 = memchr (ptr1,',',3);             
    if (ptr2==0) 
		return 1;                   /* error sintaxis        */
	if (*ptr1 !='A'){
		gps->estado=GPS_NOPOS;
        return 1;							    /* datos no validad      */
	}
	strncpy(gps->hora,buffer_aux,6);			/* guarda hora          */

    /*=====================LEE LATITUD ==============================*/
    ptr1=ptr2;
    ptr1++;                                 /* inicio                */
    ptr2 = memchr (ptr1,',',15);            
    if (ptr2==0)
		return 1;                   /* error sintaxis        */
 	//if(ptr2-ptr1 > (sizeof(GPS_pos.latitud)-1))
	//	return 1;
    *ptr2 = 0;                              /* termina cadena        */
 	strncpy(gps->Posicion.latitud,ptr1, 8);		// guarda formato DDMM.ddd
 	gps->Posicion.latitud[8] = 0;
    /*=====================LEE LATITUDC==============================*/
    ptr1=ptr2;
    ptr1++;                                 /* inicio                */
    ptr2 = memchr (ptr1,',',2); 
    if (ptr2==0) return 1;                   /* error sintaxis        */
    *ptr2 = 0;                              /* termina cadena        */
 	strcpy(gps->Posicion.latitudC,ptr1);				/* copia cadena          */	
    /*=====================LEE LONGITUD==============================*/
    ptr1=ptr2;
    ptr1++;                                 /* inicio                */
    ptr2 = memchr (ptr1,',',15);            /* fin                   */
    if (ptr2==0)
		return 1;                   /* error sintaxis        */
 	//if(ptr2-ptr1 > (sizeof(GPS_pos.longitud)-1))
	//	return 1;
    *ptr2 = 0;                              /* termina cadena        */
 	strncpy(gps->Posicion.longitud,ptr1,9);	// guarda formato DDDMM.ddd
 	gps->Posicion.longitud[9]=0;
    /*=====================LEE LONGITUDC=============================*/
    ptr1=ptr2;
    ptr1++;                                 /* inicio                */
    ptr2 = memchr (ptr1,',',2);            
    if (ptr2==0) return 1;                   /* error sintaxis        */
	*ptr2=0;								/* termina cadena        */
 	strcpy(gps->Posicion.longitudC,ptr1);				/* copia cadena          */	
    /*=====================LEE VELOCIDAD=============================*/
    ptr1=ptr2;
    ptr1++;                                 /* inicio                */
    ptr2 = memchr (ptr1,',',15);           
    if (ptr2==0) return 1;                   /* error sintaxis        */
	*ptr2=0;								/* termina cadena        */
 	strcpy(gps->vel,ptr1);				    /* copia cadena          */	
    /*=====================LEE RUMBO=================================*/
    ptr1=ptr2;
    ptr1++;                                 /* inicio                */
    ptr2 = memchr (ptr1,',',15);           
    if (ptr2==0) return 1;                   /* error sintaxis        */
	*ptr2=0;								/* termina cadena        */
 	strcpy(gps->rumbo,ptr1);  				/* copia cadena          */	
    /*=====================LEE DATE==================================*/
    ptr1=ptr2;
    ptr1++;                                 /* inicio                */
    ptr2 = memchr (ptr1,',',15);           
    if (ptr2==0)
		return 1;                   /* error sintaxis        */
 	else if(ptr2-ptr1 != 6)
		return 1;
	*ptr2=0;								/* termina cadena        */
 	strcpy(gps->fecha,ptr1);  				/* copia cadena          */	
	// marca como trama correcta
	gps->estado=GPS_OK;
	return 0;
}


/****************************************************************************************************//**
 * @fun		TratarGPS_GPZDA
 * @brief	Update time data even without satelites
 * @param	trama		stream to be precessed
 * @return	
 **********************************************************************************/
//static uint8_t TratarGPS_GPZDA(STRGps *gps, unsigned char *trama){
//	char *ptr2;
//
//	// descarta $GPZDA
//	trama += 7;
//
//    ptr2 = memchr (trama,',',12);           /* cabezera              */
//    if (ptr2==0){
//      
//      //LED_STS = 1; 
//	    return 1;                            /* error sintaxis termina*/
//    }
//	//LED_STS = 0;    
//	*ptr2 = 0;   
//	if(strlen((const char*)trama) > 1)                   /* termina cadena        */
//		strcpy(gps->hora,(const char*)trama);				/* guarda hasta dato valido*/
//
//	trama = (unsigned char*)(ptr2+1);
//	ptr2 = memchr (trama,',',3);
//	if(ptr2 ==0)
//		return 1;
//	*ptr2 = 0;
//	if(strlen((const char*)trama) > 1)
//		memcpy(gps->fecha,(const char*)trama,2);          // copia fecha 
//	
//	trama = (unsigned char*)(ptr2+1);
//	ptr2 = memchr (trama,',',3);
//	if(ptr2 ==0)
//		return 1;
//	*ptr2 = 0;
//	if(strlen((const char*)trama) > 1)
//		memcpy(&gps->fecha[2],(const char*)trama,2);          // copia fecha 
//		
//	trama = (unsigned char*)(ptr2+1);
//	ptr2 = memchr (trama,',',5);
//	if(ptr2 ==0)
//		return 1;
//	*ptr2 = 0;
//	if(strlen((const char*)trama) > 2)
//		memcpy(&gps->fecha[4],(const char*)&trama[2],2);          // copia fecha 
//
//    gps->fecha[6]=0;
//	return 0;
//}


