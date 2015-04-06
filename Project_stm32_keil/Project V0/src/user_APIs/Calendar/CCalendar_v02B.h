/** ###################################################################
**     Filename  : CCalendar.C
**     Date/Time : 06/11/2007, 10:41
**     Abstract  :
**         Código fuente del componente CCalendar que implementa un 
**         Calendario  con cambios de estación y peridos 
**         vacacionales.
**     Settings  :
**     Contents  :
**     Realizado :
**         Raul Martin
** ###################################################################*/
/* MODULE CCalendar */

#ifndef __CCalendar_H
#define __CCalendar_H

#include <stdint.h>


/*****************************************************************************************************************/
                                       /* CTime */
/*****************************************************************************************************************/

#define CTime_LUNES             0x40
#define CTime_MARTES            0x20
#define CTime_MIERCOLES         0x10
#define CTime_JUEVES            0x08
#define CTime_VIERNES           0x04
#define CTime_SABADO            0x02
#define CTime_DOMINGO           0x01
#define CTime_LUVI              0x7c
#define CTime_SADO              0x03
#define CTime_LUDO              0x7f

#define CTime_IGUALES           8
#define CTime_MENOR             16
#define CTime_MAYOR             32

__packed typedef struct {
    uint8_t seg;
    uint8_t min;
    uint8_t hor;
    uint8_t sem;
}CTime;

void 		CTime_Set			(CTime *c, uint8_t d, uint8_t h, uint8_t m, uint8_t s);
uint32_t 	CTime_Incr			(CTime *c);
uint32_t 	CTime_IncSem		(CTime *c);
uint32_t 	CTime_Decr			(CTime *c);
uint32_t 	CTime_DecSem		(CTime *c);
uint8_t 	CTime_Compare		(CTime *co, CTime *cd);
void 		CTime_FormatSem		(CTime *c, uint8_t decVal);
uint8_t 	CTime_UnformatSem	(CTime *c);
void 		CTime_Copy			(CTime *org, CTime *dest);
uint32_t 	CTime_ConvertU32	(CTime t);
void 		CTime_ToString		(CTime time, char * str);
void 		CTime_FromString	(CTime * time, char * str);


/*****************************************************************************************************************/
                                       /* CDate */
/*****************************************************************************************************************/

#define CDate_ENERO        (uint8_t)1
#define CDate_FEBRERO      (uint8_t)2
#define CDate_MARZO        (uint8_t)3
#define CDate_ABRIL        (uint8_t)4
#define CDate_MAYO         (uint8_t)5
#define CDate_JUNIO        (uint8_t)6
#define CDate_JULIO        (uint8_t)7
#define CDate_AGOSTO       (uint8_t)8
#define CDate_SEPTIEMBRE   (uint8_t)9
#define CDate_OCTUBRE      (uint8_t)10
#define CDate_NOVIEMBRE    (uint8_t)11
#define CDate_DICIEMBRE    (uint8_t)12

#define CDate_IGUALES      1
#define CDate_MENOR        2
#define CDate_MAYOR        4

__packed typedef struct {
    uint8_t dia;
    uint8_t mes;
    uint8_t ani;
    uint8_t reserved1;
}CDate;

uint32_t 	CDate_IncDia		(CDate *c);
uint32_t 	CDate_DecDia		(CDate *c);
void 		CDate_Set			(CDate *c, uint8_t d, uint8_t m, uint8_t a);
uint32_t 	CDate_Incr			(CDate *c);
uint32_t 	CDate_Decr			(CDate *c);    
uint8_t 	CDate_Compare		(CDate *co, CDate *cd, uint8_t flags);
uint8_t 	CDate_CheckIntegrity(CDate *c);
void 		CDate_Copy			(CDate *org, CDate *dest);
void 		CDate_ToString		(CDate date, char * str);
void 		CDate_FromString	(CDate * date, char * str);
uint8_t		CDate_IsLeapYear	(CDate *date);

/*****************************************************************************************************************/
                                       /* CRtc */
/*****************************************************************************************************************/

#define RTC_RELATIVE_ADDR_FOR_TIME  0                // situación de la hora dentro de la clase
#define RTC_RELATIVE_ADDR_FOR_DATE  sizeof(CTime)    // situación de la fecha dentro de la clase

#define CRtc_SetTime(c,d,h,m,s)         CTime_Set(&c->time,d,h,m,s)
#define CRtc_IncTime(c)                 CTime_Incr(&c->time)
#define CRtc_DecTime(c)                 CTime_Decr(&c->time)
#define CRtc_SetDate(c,d,m,a)           CDate_Set(&c->date,d,m,a)
#define CRtc_IncDate(c)                 CDate_Incr(&c->date)
#define CRtc_DecDate(c)                 CDate_Decr(&c->date)

__packed typedef struct {
    CTime time;
    CDate date;
}CRtc;

uint32_t 	CRtc_Incr					(CRtc *c);
uint32_t	CRtc_Decr					(CRtc *c);    
uint8_t 	CRtc_Compare				(CRtc *co, CRtc *cd);
void 		CRtc_Copy					(CRtc *org, CRtc *dest);
uint8_t 	CRtc_CheckIntegrity			(CRtc *c);
uint8_t 	CRtc_CheckStringIntegrity	(CRtc *c, char *stime, char *sdate);
uint8_t 	CRtc_CalcWeekday			(CRtc *c, uint8_t updateItself);


/*****************************************************************************************************************/
                                       /* CCalendar */
/*****************************************************************************************************************/

#define  CCalendar_STAT_24H       1u        // Modo 24h activado
#define  CCalendar_STAT_ENEST     2u        // Cambio de estación activado
#define  CCalendar_STAT_ESTAUTO   4u        // Cambio de estación AUTOMATICO seleccionado
#define  CCalendar_STAT_NORTE     8u        // Cambio de estación en modo HEMISFERIO NORTE
#define  CCalendar_STAT_ESVER     0x10u     // Calendario en fecha de VERANO
#define  CCalendar_STAT_ENVAC     0x20u     // Periodo de vacaciones activado
#define  CCalendar_STAT_ESVAC     0x40u     // Calendario en periodo de VACACIONES
#define  CCalendar_EST_CHANGED    0x80u     // SA14#02 - Indica que acaba de cambiar de estación 
#define CAL_DEF_STATUS	(CCalendar_STAT_24H | CCalendar_STAT_ENEST | CCalendar_STAT_ESTAUTO | CCalendar_STAT_NORTE)
#define EVENT_CMBSEG	1
#define EVENT_ISSEG15	2
#define EVENT_ISSEG45	4
#define EVENT_CMBMIN	8
#define EVENT_CMBHOR	16
#define EVENT_ISMIDDAY	32                                           
#define EVENT_CMBDIA	64
#define EVENT_CMBSEM	128
#define EVENT_CMBMES	256
#define EVENT_CMBANI	512

#define CAL_ERR_OK			0
#define CAL_ERR_VALUE		1
                                            
#define CCalendar_MAX_N_DAYS      31             

__packed typedef struct {
    uint32_t status;               // estado de la configuración
    CRtc objRtc_verano;         // Rtc para gestionar cambios de estación inv->ver
    CRtc objRtc_invierno;       // Rtc para gestionar cambios de estación ver->inv
    CRtc objRtc_vacs_ini;       // Rtc para gestionar inicio de vacaciones
    CRtc objRtc_vacs_fin;       // Rtc para gestionar final de vacaciones
    CRtc objRtc_sistema;        // Rtc para gestionar la fecha/hora del sistema
}CCalendar;

void 		CCalendar_Init					(CCalendar *c);
uint32_t 	CCalendar_Incr					(CCalendar *this, uint8_t *statChange);
void 		CCalendar_Copy					(CCalendar *org, CCalendar *dest);
void 		CCalendar_CopyConfig			(CCalendar *org, CCalendar *dest);
uint8_t 	CCalendar_CheckIntegrity		(CCalendar *c);
uint8_t 	CCalendar_CheckConfigIntegrity	(CCalendar *c);
void 		CCalendar_CalcSeason			(CCalendar *This, uint8_t *statChange);
uint32_t 	CCalendar_CheckSeason			(CCalendar *This, uint8_t *statChange);
uint8_t		CCalendar_GetFromSeconds		(CCalendar *cal, uint32_t counter);
uint32_t 	CCalendar_GetInSeconds			(CCalendar *cal);
void 		CCalendar_SetFromUTC			(CCalendar *cal, CRtc *utc, int8_t utcRef);
void 		CCalendar_AddOneHour			(CCalendar *cal);
void 		CCalendar_SubOneHour			(CCalendar *cal);
#endif

