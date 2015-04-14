/*************************************************************************//**
 * @file    Lib_GPS.h
 * @brief   GPS Library for data processing
 * @date    15.06.2010
 * @author  Raúl M.
 ****************************************************************************/
#ifndef __Lib_GPS_H__
#define __Lib_GPS_H__

#ifdef __cplusplus
 extern "C" {
#endif 


/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>

/****************************************************************************************************//**
 *                        PUBLIC DATA TYPES
 ********************************************************************************************************/


/*************************************************************************//**
 * @def		NMEA_ABP_ENABLE		
 * @scope	protected
 * @brief   NMEA sentence for Almanac Based Positioning enable. NMEA $PSRF103
 *			is used an applied to RMC sentence mode=2 (ABP on). Checksum is
 *			calculated by: http://www.hhhh.org/wiml/proj/nmeaxor.html
 ****************************************************************************/
#define NMEA_ABP_ENABLE	"$PSRF103,04,02,01,01*23\r\n"


/*************************************************************************//**
 * @enum	E_GPS_STAT		
 * @scope	protected
 * @brief   GPS status control flag
 ****************************************************************************/
 enum E_GPS_STAT{GPS_NOPOS, GPS_OK, GPS_PDOWN, GPS_ERROR};

/*************************************************************************//**
 * @struct	STRPosicion	
 * @scope	protected
 * @brief   Position coordinates 
 ****************************************************************************/
typedef struct{
    char latitud[11];     /* latitud               */
    char latitudC[2];     /* latitud ESTE/OESTE    */
    char longitud[12];    /* longitud              */
    char longitudC[2];    /* longitud N/S          */
} STRPosicion; 

/*************************************************************************//**
 * @struct	STRGps	
 * @scope	protected
 * @brief   GPS data information structure 
 ****************************************************************************/
typedef struct {          /* DATOS DE GPS          */
    char estado;          /* estado                */
    char error;
	STRPosicion Posicion; /* Posicion Latitud, Longitud */
    char hora[10];         /* hora                  */
    int satelites;       /* numero de satelites   */
    char altura[10];      /* altura                */
    char vel[10];         /* velocidad             */
    char fecha[7];        /* fecha                 */
    char hdop[7];         /* calidad señal         */
    char rumbo[7];        /* rumbo                 */
}STRGps;

/*************************************************************************//**
 * @const	gps_init	
 * @scope	public
 * @brief   STRGps constant structure to initialize object. 
 ****************************************************************************/
extern const STRGps gps_init;

/****************************************************************************************************//**
 *                        	PUBLIC PRIMITIVES
 ********************************************************************************************************/
uint8_t GPS_GetData				(uint8_t data);
void 	GPS_CheckFrame			(STRGps *gpsPos, void (*INotify)(uint8_t *,uint16_t));
void 	GPS_LatitudeToString	(STRGps *gpsPos, char *lat);
void 	GPS_LongitudeToString	(STRGps *gpsPos, char *lon);



#ifdef __cplusplus
}
#endif



#endif

