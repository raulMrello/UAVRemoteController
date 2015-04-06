#ifndef __CALENDARIO_H
#define __CALENDARIO_H

#include "RTL.h"

/** Estructura par el manejo de fechas*/
typedef struct
{
	/** Día*/
	unsigned char	Dia;
	/** Mes*/
	unsigned char	Mes;
	/** Año*/
	unsigned short	Anyo;
//	/** Flag que indica si se trata de un día festivo*/
//	unsigned char	Festivo;
//	/** Flag que indica si se trata de vacaciones*/
//	unsigned char	Vacaciones;
	/** Indica el tipo de día:
		- 'LABORABLE' (valor 0)
		- 'FESTIVO' (valor 1)
		- 'VACACIONES' (valor 2)*/
	unsigned char	TipoDia;
	/** Uso horario*/
	signed char		GMT;
	/** Flag que indica si nos encontramos en horario de verano*/
	unsigned char	HorarioVerano;
	/** Exclusión mutua para este objeto*/
	OS_MUT Mutex;
} tCalendarioFecha;

#define MAX_DIAS_FESTIVOS	20
#define MAX_DIAS_VACACIONES	40

/** Estructura para manejar dias festivos*/
typedef struct
{
	tCalendarioFecha Dia[MAX_DIAS_FESTIVOS];
	unsigned char NumDias;
} tDiasFestivos;

/** Estructura para manejar dias de vacaciones*/
typedef struct
{
	tCalendarioFecha Dia[MAX_DIAS_VACACIONES];
	unsigned char NumDias;
} tDiasVacaciones;

/** Segundos en un día*/
#define SEGUNDOS_POR_DIA	86400
/** Segundos en una hora*/
#define SEGUNDOS_POR_HORA	3600
/** Segundos en un minuto*/
#define SEGUNDOS_POR_MIN	60
//#define SEGUNDOS_POR_MIN	10
//#define DEFAULT_DAY 28
//#define DEFAULT_MONTH 12
//#define DEFAULT_YEAR 2008
/** Constante para reconocer un año bisiesto*/
#define BISIESTO 	1
/** Constante para reconocer un año no bisiesto*/
#define NO_BISIESTO 0
/** Constante para reconocer un día laborable*/
#define LABORABLE		(0)
/** Constante para reconocer un día festivo*/
#define FESTIVO			(1)
/** Constante para reconocer un día de vacaciones*/
#define VACACIONES		(2)

//Prototipos de funciones
//-----------------------------------------------------------------
void			Calendario_Init(tCalendarioFecha *Fecha);
unsigned char	Calendario_FijarFecha(tCalendarioFecha *Fecha, unsigned char Day, unsigned char Month, unsigned short Year);
unsigned char	Calendario_ComprobarBisiesto(unsigned short Year);
void			Calendario_DisminuirDia(tCalendarioFecha *Fecha);
void			Calendario_AumentarDia(tCalendarioFecha *Fecha);
void			Calendario_AumentarMes(tCalendarioFecha *Fecha);
void			Calendario_AumentarAnyo(tCalendarioFecha *Fecha);
unsigned char	Calendario_DiaSemana(tCalendarioFecha *Fecha);
void 			Calendario_ObtenerFecha(tCalendarioFecha *Fecha);
unsigned char	Calendario_ObtenerTipoDia(tDiasFestivos *Festivos, 
										  tDiasVacaciones *Vacaciones, 
										  tCalendarioFecha *Fecha);
void 			Calendario_RegistrarFestivo(tDiasFestivos *Festivos, tCalendarioFecha *Fecha);
void 			Calendario_RegistrarVacional(tDiasVacaciones *Vacaciones, tCalendarioFecha *Fecha);
//-----------------------------------------------------------------

#endif //__CALENDARIO_H
