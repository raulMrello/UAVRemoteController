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

 #include "CCalendar.h"




static uint8_t RotateRight(uint8_t d, uint8_t num_bits){
    // param cheq.
    uint8_t ret = d;
    
    // antes de rotar comprueba el valor que retornará    
    if(ret & 1){
        ret = ret >> 1;
        ret |= 0x40;
        return ret;
    }
    else{
        ret = ret >> 1;
        return ret;
    }
}

static uint8_t RotateLeft(uint8_t d, uint8_t num_bits){
    // param cheq.
    uint8_t ret = d;
    
    // antes de rotar comprueba el valor que retornará. si b[num_bits-1] = 1:    
    if(ret & 0x40){
        ret = ret << 1;
        ret |= 1;
        ret &= 0x7f;        // borra los bits no usados.        
        return ret;
    }
    // si b[num_bits-1] = 0:
    else{
        ret = ret << 1;
        ret &= 0x7f;        // borra los bits no usados.        
        return ret;
    }
}

/*
** ==================================================================================================
**                                    CTime
** ==================================================================================================
*/

/*
** ===================================================================
**     Method      :  CTime_set
**
**     Description :
**         Establece la hora [*c] teniendo en cuenta todas sus propiedades.
**     Parameters  :
**         CTime *c : componente sobre el que aplicar la función.
**         uint8_t d  : día
**         uint8_t h  : hora
**         uint8_t m  : minuto
**         uint8_t s  : segundo
**     Return :
** ===================================================================
*/
void CTime_Set(CTime *c, uint8_t d, uint8_t h, uint8_t m, uint8_t s){
    c->sem = d;
    c->hor = h;
    c->min = m;
    c->seg = s;    
}



/*
** ===================================================================
**     Method      :  CTime_incr
**
**     Description :
**         Incrementa la hora [*c] teniendo en cuenta todas sus propiedades.
**     Parameters  :
**         CTime *c : componente sobre el que aplicar la función.
**     Return :
**      EVENT_CMBSEG    evento de cambio de segundo
**      EVENT_CMBMIN    de cambio de minuto
**      EVENT_CMBHOR    de hora
**      EVENT_CMBDIA    de día
**      EVENT_CMBSEM    de semana
**      EVENT_ISSEG15   si es el segundo 15 (xx:xx:15)
**      EVENT_ISSEG45   si es el 45
**      EVENT_ISMIDDAY  si es medio día (12:00:00)
** ===================================================================
*/
uint32_t CTime_Incr(CTime *c){
    uint32_t err;
    err = EVENT_CMBSEG;
    
    err &= ~EVENT_ISMIDDAY;
    c->seg = (uint8_t)((c->seg < 59)? c->seg+1 : 0);
    if(c->seg == 0){
        err |= EVENT_CMBMIN;
        c->min = (uint8_t)((c->min < 59)? c->min+1 : 0);
        if(c->min == 0){
            err |= EVENT_CMBHOR;
            c->hor = (uint8_t)((c->hor < 23)? c->hor+1 : 0);    
            if(c->hor == 0){
                err |= EVENT_CMBDIA;
                c->sem = RotateRight(c->sem, 7);
                if(c->sem == CTime_LUNES){
                    err |= EVENT_CMBSEM;                    
                }
            }
            else if(c->hor == 12){
                err |= EVENT_ISMIDDAY;
            }
        }                
    }
    
    return err;            
}


/*
** ===================================================================================================
**     Method      :  CTime_IncSem
**
**     Description :
**         Incrementa el día de la semana
**     Parameters  :
**         CTime *c : componente sobre el que aplicar la función.
**     Return :
**      EVENT_CMBSEM    de semana
** ===================================================================================================
*/
uint32_t CTime_IncSem(CTime *c){
    c->sem = RotateRight(c->sem, 7);
    if(c->sem != CTime_LUNES)
        return 0;
    return EVENT_CMBSEM;    
}



/*
** ===================================================================
**     Method      :  CTime_decr
**
**     Description :
**         Decrementa la hora [*c] teniendo en cuenta todas sus propiedades.
**     Parameters  :
**         CTime *c : componente sobre el que aplicar la función.
**     Return :
**      EVENT_CMBSEG    evento de cambio de segundo
**      EVENT_CMBMIN    de cambio de minuto
**      EVENT_CMBHOR    de hora
**      EVENT_CMBDIA    de día
**      EVENT_CMBSEM    de semana
**      EVENT_ISSEG15   si es el segundo 15 (xx:xx:15)
**      EVENT_ISSEG45   si es el 45
**      EVENT_ISMIDDAY  si es medio día (12:00:00)
** ===================================================================
*/
uint32_t CTime_Decr(CTime *c){
    uint32_t err;
    err = EVENT_CMBSEG;
    c->seg = (uint8_t)((c->seg > 0)? c->seg-1 : 59);
    if(c->seg == 15){
        err |= EVENT_ISSEG15;        
        return err;        
    }
    else if(c->seg == 45){
        err |= EVENT_ISSEG45;        
        return err;        
    }
    else if(c->seg != 59)
        return err;
       
    
    err |= EVENT_CMBMIN;
    c->min = (uint8_t)((c->min > 0)? c->min-1 : 59);
    if(c->min != 59)
        return err;
    
    err |= EVENT_CMBHOR;
    c->hor = (uint8_t)((c->hor > 0)? c->hor-1 : 23);
    // comprueba que son las 12:00:00
    if(c->hor == 12)
        err |= EVENT_ISMIDDAY;
    else
        err &= ~EVENT_ISMIDDAY;
    
    if(c->hor != 23)
        return err;
    
    err |= EVENT_CMBDIA;
    c->sem = RotateLeft(c->sem, 7);
    if(c->sem != CTime_DOMINGO)
        return err;
    
    err |= EVENT_CMBSEM;
    return err;            
}


/*
** ===================================================================================================
**     Method      :  CTime_DecSem
**
**     Description :
**         Incrementa el día de la semana
**     Parameters  :
**         CTime *c : componente sobre el que aplicar la función.
**     Return :
**      EVENT_CMBSEM    de semana
** ===================================================================================================
*/
uint32_t CTime_DecSem(CTime *c){
    c->sem = RotateLeft(c->sem, 7);
    if(c->sem != CTime_DOMINGO)
        return 0;
    return EVENT_CMBSEM;    
}



/*
** ===================================================================================================
**     Method      :  CTime_FormatSem
**
**     Description :
**         El día de la semana suele expresarse en un valor entre 1..7 (chip rtc). Esta función formatea
**          el día de la semana a un grupo de bits.
**     Parameters  :
**          *c      -   clase de referencia
**          decVal  -   valor de 1..7
**     Return :
** ===================================================================================================
*/
void CTime_FormatSem(CTime *c, uint8_t decVal){
    switch(decVal){
        case 1:
            c->sem = CTime_LUNES;
            break;
        case 2:
            c->sem = CTime_MARTES;
            break;
        case 3:
            c->sem = CTime_MIERCOLES;
            break;
        case 4:
            c->sem = CTime_JUEVES;
            break;
        case 5:
            c->sem = CTime_VIERNES;
            break;
        case 6:
            c->sem = CTime_SABADO;
            break;
        case 7:
            c->sem = CTime_DOMINGO;
            break;
    }
}



/*
** ===================================================================================================
**     Method      :  CTime_UnformatSem
**
**     Description :
**         El día de la semana suele expresarse en un valor entre 1..7 (chip rtc). Esta función formatea
**          el día de la semana dado en un grupo de bits a un valor entre 1..7
**     Parameters  :
**          *c      -   clase de referencia
**     Return :
**          sem     -   valor entre 1..7
** ===================================================================================================
*/
uint8_t CTime_UnformatSem(CTime *c){
    switch(c->sem){
        case CTime_LUNES:
            return 1u;
        case CTime_MARTES:
            return 2u;
        case CTime_MIERCOLES:
            return 3u;
        case CTime_JUEVES:
            return 4u;
        case CTime_VIERNES:
            return 5u;
        case CTime_SABADO:
            return 6u;
        case CTime_DOMINGO:
            return 7u;
    }
    return 0u;
}

/*
** ===================================================================
**     Method      :  CTime_comparar
**
**     Description :
**         Compara dos clases de tipo CTime.
**     Parameters  :
**         CTime *co,*cd : clases a comparar
**     Return :
**         uint8_t : CTime_IGUALES - co.hora = cd.hora
**                 CTime_MENOR - co.hora < cd.hora
**                 CTime_MAYOR - co.hora > cd.hora
** ===================================================================
*/
uint8_t CTime_Compare(CTime *co, CTime *cd){
     
    if(co->seg == cd->seg && co->min == cd->min && co->hor == cd->hor)
        return(CTime_IGUALES);
    else if(co->seg < cd->seg && co->min == cd->min && co->hor == cd->hor)
        return(CTime_MENOR);
    else if(co->min < cd->min && co->hor == cd->hor)
        return(CTime_MENOR);    
    else if(co->hor < cd->hor)
        return(CTime_MENOR);
    else
        return(CTime_MAYOR);
}

/* ===================================================================
    Name:
        CTime_Copy
    Description :
        Copia una hora
    Params:
        org                 componente origen
        dest                componente destino
 ===================================================================*/
void CTime_Copy(CTime *org, CTime *dest){
    dest->seg = org->seg;
    dest->min = org->min;
    dest->hor = org->hor;
    dest->sem = org->sem;
}



/* ===================================================================
    Name:
        CTime_ConvertU32
    Description :
        Convierte la hora a uint32_t
    Params:
        t                 componente origen
 ===================================================================*/
uint32_t CTime_ConvertU32(CTime t){
    uint32_t retVal = 0;
    retVal += t.seg;
    retVal += (uint32_t)(t.min * 60);
    retVal += (uint32_t)(t.hor * 3600);
    return(retVal);
}
/*************************************************************************//**
 * @fun     CTime_ToString
 * @brief   Parses a time into a string HH:MM:SS
 *
 * @param	time		T_Time origin struct
 * @param	str			pointer to the result string
 *
 * @return  n/a
 ****************************************************************************/
 void CTime_ToString(CTime time, char * str){
	*(str) 	 = (char)(0x30 + time.hor/10);
  	*(str+1) = (char)(0x30 + time.hor%10);
  	*(str+2) = (char)':';
  	*(str+3) = (char)(0x30 + time.min/10);
  	*(str+4) = (char)(0x30 + time.min%10);
  	*(str+5) = (char)':';
  	*(str+6) = (char)(0x30 + time.seg/10);
  	*(str+7) = (char)(0x30 + time.seg%10);
  	*(str+8) = (char)0;

 }

/*************************************************************************//**
 * @fun     CTime_FromString
 * @brief   Parses a time from a formatted string: HH:MM:SS
 *
 * @param	time		T_Time result struct
 * @param	str			pointer to the origin string
 *
 * @return  n/a
 ****************************************************************************/
 void CTime_FromString(CTime * time, char * str){
	time->hor = (uint8_t)(((*(str) - 0x30)*10)+(*(str+1) - 0x30));
	time->min = (uint8_t)(((*(str+3) - 0x30)*10)+(*(str+4) - 0x30));
	time->seg = (uint8_t)(((*(str+6) - 0x30)*10)+(*(str+7) - 0x30));
 }

/*
** ==================================================================================================
**                                    CDate
** ==================================================================================================
*/
uint32_t CDate_IncDia(CDate *c);
uint32_t CDate_DecDia(CDate *c);

/*****************************************************************************************************************/
                                       /* PUBLIC METHODS */
/*****************************************************************************************************************/

/* ===================================================================
    Name:
        CDate_inc_dia
    Description :
        Incrementa el día. No se puede llamar directamente a la función
        API ya que hay que comprobar el mes, así como si es o no bisiesto.
    Params:
        c               componente sobre el que aplicar la función.
     Return :
        API_VAL_OVF     en el caso de que los días hayan rebosado
 ===================================================================*/
uint32_t CDate_IncDia(CDate *c){
    //SA02#06 . err debe ser de 32bits
    uint32_t err = EVENT_CMBDIA;
    switch(c->mes){
        case CDate_ENERO:
        case CDate_MARZO:
        case CDate_MAYO:
        case CDate_JULIO:
        case CDate_AGOSTO:
        case CDate_OCTUBRE:
        case CDate_DICIEMBRE:
            c->dia = (uint8_t)((c->dia < 31)? c->dia+1 : 1);
            break;

        case CDate_ABRIL:
        case CDate_JUNIO:
        case CDate_SEPTIEMBRE:
        case CDate_NOVIEMBRE:
            c->dia = (uint8_t)((c->dia < 30)? c->dia+1 : 1);
            break;
        case CDate_FEBRERO:
            if(c->ani % 4 == 0)
                c->dia = (uint8_t)((c->dia < 29)? c->dia+1 : 1);
            else
                c->dia = (uint8_t)((c->dia < 28)? c->dia+1 : 1);
            break;
    }
    if(c->dia == 1)
        err |= EVENT_CMBMES;
    
    return err;
}



/* ===================================================================
    Name:
        CDate_DecDia
    Description :
        Decrementa el día. No se puede llamar directamente a la función
        API ya que hay que comprobar el mes, así como si es o no bisiesto.
    Params:
        c               componente sobre el que aplicar la función.
     Return :
        API_VAL_OVF     en el caso de que los días hayan rebosado
 ===================================================================*/
uint32_t CDate_DecDia(CDate *c){
    //SA02#06 . err debe ser de 32bits
    uint32_t err = EVENT_CMBDIA;
    uint8_t limit = 0;
    switch(c->mes){
        case CDate_ENERO:
        case CDate_MARZO:
        case CDate_MAYO:
        case CDate_JULIO:
        case CDate_AGOSTO:
        case CDate_OCTUBRE:
        case CDate_DICIEMBRE:
            limit = 31;
            break;
        case CDate_ABRIL:
        case CDate_JUNIO:
        case CDate_SEPTIEMBRE:
        case CDate_NOVIEMBRE:
            limit = 30;
            break;
        case CDate_FEBRERO:
            if(c->ani % 4 == 0)
                limit = 29;
            else
                limit = 28;
            break;
    }
    c->dia = (uint8_t)((c->dia > 1)? c->dia-1 : limit);
    if(c->dia == limit)
        err |= EVENT_CMBMES;
    
    return err;
}



/* ===================================================================
    Name:
        CDate_Set
    Description :
        Establece una fecha a partir de sus variables
    Params:
        c               componente sobre el que aplicar la función.
        d               día
        m               mes
        a               año
 ===================================================================*/
void CDate_Set(CDate *c, uint8_t d, uint8_t m, uint8_t a){
    c->dia = (uint8_t)((d > 0)? d : c->dia);
    c->mes = (uint8_t)((m > 0)? m : c->mes);
    c->ani = (uint8_t)((a > 0)? a : c->ani);
    CDate_CheckIntegrity(c);    
}



/* ===================================================================
    Name:
        CDate_Incr
    Description :
        Incrementa la fecha y devuelve eventos
    Params:
        c               componente sobre el que aplicar la función.
     Return :
        EVENT_CMBDIA    cambio de día
        EVENT_CMBMES    cambio de mes
        EVENT_CMBANI    cambio de año
 ===================================================================*/
uint32_t CDate_Incr(CDate *c){
    uint32_t err;
    
    err = EVENT_CMBDIA;
    if(!(CDate_IncDia(c) & EVENT_CMBMES))
        return err;
    
    err |= EVENT_CMBMES;
    c->mes = (uint8_t)((c->mes < 12)? c->mes+1 : 1);
    if(c->mes != 1)
        return err;
    
    err |= EVENT_CMBANI;
    c->ani = (uint8_t)((c->ani > 0)? c->ani+1 : 99);
    return err;                
}



/* ===================================================================
    Name:
        CDate_Decr
    Description :
        Decrementa la fecha y devuelve eventos
    Params:
        c               componente sobre el que aplicar la función.
     Return :
        EVENT_CMBDIA    cambio de día
        EVENT_CMBMES    cambio de mes
        EVENT_CMBANI    cambio de año
 ===================================================================*/
uint32_t CDate_Decr(CDate *c){
    uint32_t err;
    
    err = EVENT_CMBDIA;
    if(c->dia > 1){
        if(!(CDate_DecDia(c) & EVENT_CMBMES))
            return err;
        
        err |= EVENT_CMBMES;
        c->mes = (uint8_t)((c->mes > 1)? c->mes-1 : 12);
        if(c->mes != 12)
            return err;
        
        err |= EVENT_CMBANI;
        c->ani = (uint8_t)((c->ani > 0)? c->ani-1 : 99);
        
        return err;                    
    }
    else{
        err |= EVENT_CMBMES;
        c->mes = (uint8_t)((c->mes > 1)? c->mes-1 : 12);
        if(c->mes == 12){
            err |= EVENT_CMBANI;
            c->ani = (uint8_t)((c->ani > 0)? c->ani-1 : 99);    
        }
        CDate_DecDia(c);
        return err;
    }
}

/* ===================================================================
    Name:
        CDate_Compare
    Description :
        Compara dos fechas
    Params:
        co              componente origen
        cd              componente destino
        flags           flags a tener en cuenta en la comparación
     Return :
        CDate_IGUALES   iguales
        CDate_MENOR     co < cd
        CDate_MAYOR     co > cd
 ===================================================================*/
uint8_t CDate_Compare(CDate *co, CDate *cd, uint8_t flags){
    if (co->dia == cd->dia && co->mes == cd->mes && co->ani == cd->ani)
        return(uint8_t)(flags + CDate_IGUALES);
    else if (co->dia < cd->dia && co->mes == cd->mes && co->ani == cd->ani)
        return(uint8_t)(flags + CDate_MENOR);
    else if (co->mes < cd->mes && co->ani == cd->ani)
        return(uint8_t)(flags + CDate_MENOR);
    else if (co->ani < cd->ani)
        return(uint8_t)(flags + CDate_MENOR);
    else
        return(uint8_t)(flags + CDate_MAYOR);
}




/* ===================================================================
    Name:
        CDate_CheckIntegrity
    Description :
        Comprueba si la fecha está en los márgenes permitidos. Si no lo
        está, la arregla.
    Params:
        c               componente origen
     Return :
        >0              nº de variables fuera de rango
 ===================================================================*/
uint8_t CDate_CheckIntegrity(CDate *c){
    uint8_t err = 0;
    // SA15#01 - incluye estas comparaciones para evitar fechas a 0.
    if(c->dia == 0){
        err ++;
        c->dia = 1;        
    }
    if(c->mes == 0){
        err ++;
        c->mes = 1;        
    }
    switch(c->mes){
        case CDate_ENERO:
        case CDate_MARZO:
        case CDate_MAYO:
        case CDate_JULIO:
        case CDate_AGOSTO:
        case CDate_OCTUBRE:
        case CDate_DICIEMBRE:
            if(c->dia > 31){
                c->dia = 31;   
                err ++;
            }                
            break;

        case CDate_ABRIL:
        case CDate_JUNIO:
        case CDate_SEPTIEMBRE:
        case CDate_NOVIEMBRE:
            if(c->dia > 30){
                c->dia = 30;
                err++;                
            }
            break;
        case CDate_FEBRERO:
            if(c->ani % 4 == 0){
                if(c->dia > 29){
                    c->dia = 29;
                    err++;                    
                }
            }
            else{
                if(c->dia > 28){
                    c->dia = 28;
                    err++;                    
                }
            }
            break;
        default:
            err++;
            break;
    }
    return(err);
}


/* ===================================================================
    Name:
        CDate_Copy
    Description :
        Copia una fecha
    Params:
        org                 componente origen
        dest                componente destino
 ===================================================================*/
void CDate_Copy(CDate *org, CDate *dest){
    dest->dia = org->dia;
    dest->mes = org->mes;
    dest->ani = org->ani;
    
}
/*************************************************************************//**
 * @fun     CDate_ToString
 * @brief   Parses a date into a string DD/MM/AAAA
 *
 * @param	date		T_Date original struct
 * @param	str			pointer to the result string
 *
 * @return  n/a
 ****************************************************************************/
 void CDate_ToString(CDate date, char * str){
	*(str) 	 = (char)(0x30 + date.dia/10);
  	*(str+1) = (char)(0x30 + date.dia%10);
  	*(str+2) = (char)'/';
  	*(str+3) = (char)(0x30 + date.mes/10);
  	*(str+4) = (char)(0x30 + date.mes%10);
  	*(str+5) = (char)'/';
  	*(str+6) = (char)'2';
  	*(str+7) = (char)'0';
  	*(str+8) = (char)(0x30 + (date.ani)/10);
  	*(str+9) = (char)(0x30 + (date.ani)%10);
  	*(str+10)= (char)0;
 }


/*************************************************************************//**
 * @fun     CDate_FromString
 * @brief   Parses a date from a formatted string: DD/MM/AAAA
 *
 * @param	date		T_Date result struct
 * @param	str			pointer to the origin string
 *
 * @return  n/a
 ****************************************************************************/
 void CDate_FromString(CDate * date, char * str){
	date->dia = (uint8_t)(((*(str) - 0x30)*10)+(*(str+1) - 0x30));
	date->mes = (uint8_t)(((*(str+3) - 0x30)*10)+(*(str+4) - 0x30));
	date->ani = (uint8_t)(((*(str+8) - 0x30)*10)+(*(str+9) - 0x30)); 	
 }


/*************************************************************************//**
 * @fun     CDate_IsLeapYear
 * @brief   Check if this year is a leap year
 *
 * @param	date		T_Date result struct pointer
 *
 * @return  1 if leap year
 ****************************************************************************/
 uint8_t CDate_IsLeapYear(CDate * date){
 	return((uint8_t)((date->ani % 4 == 0)? 1 : 0));
 }


/*************************************************************************//**
 * @fun     CDate_DaysTillYear
 * @brief   get num seconds till last year since 2000
 *
 * @param	date		T_Date result struct pointer
 *
 * @return  1 if leap year
 ****************************************************************************/
static uint32_t CDate_DaysTillYear(uint8_t year){
	uint16_t anios = year - 1;
	uint8_t bisiestos = anios/4;
	uint8_t nobisiestos = anios - bisiestos;
	uint32_t dias = (365*nobisiestos)+(366*bisiestos);
	return(dias);
}


/*************************************************************************//**
 * @fun     CDate_DaysThisYear
 * @brief   get num seconds for date in this year. If summer substract UTC
 *			daylight correction
 *
 * @param	date		T_Date result struct pointer
 *
 * @return  1 if leap year
 ****************************************************************************/
static uint32_t CDate_DaysThisYear(CDate *date){
    const uint16_t dias[12] = { 0, 31, 31+28, 59+31, 90+30, 120+31,
          151+30, 181+31, 212+31, 243+30, 273+31, 304+30};
   	uint32_t resultado;
    resultado = dias[date->mes - 1];
	resultado += (date->dia - 1);
	// if mon > February and...
    if ( date->mes > 2 && CDate_IsLeapYear(date))
        resultado++;
    return resultado;
}


/*
** ==================================================================================================
**                                    CRtc
** ==================================================================================================
*/
/*
** ===================================================================
**     Method      :  CRtc_incr
**
**     Description :
**         Incrementa el Rtc [*c] teniendo en cuenta todas sus propiedades.
**     Parameters  :
**         CRtc *c : componente sobre el que aplicar la función.
**     Return :
**         EVENT_xx     cualquier evento relativo a CTime y CDate
** ===================================================================
*/
uint32_t CRtc_Incr(CRtc *c){
    uint32_t err;
    
    err = CRtc_IncTime(c);   
    if(err & EVENT_CMBDIA)
        err |= CRtc_IncDate(c);
    
    return(err);
}



/*
** ===================================================================
**     Method      :  CRtc_decr
**
**     Description :
**         Decrementa el Rtc [*c] teniendo en cuenta todas sus propiedades.
**     Parameters  :
**         CRtc *c : componente sobre el que aplicar la función.
**     Return :
**         EVENT_xx     cualquier evento relativo a CTime y CDate
** ===================================================================
*/
uint32_t CRtc_Decr(CRtc *c){
    uint32_t err;
    
    err = CRtc_DecTime(c);   
    if(err & EVENT_CMBDIA)
        err |= CRtc_DecDate(c);
    
    return(err);    
}




/*
** ===================================================================
**     Method      :  CRtc_compare
**
**     Description :
**         Compara dos clases de tipo CRtc.
**     Parameters  :
**         CRtc *co,*cd : clases a comparar
**     Return :
**         uint8_t : CDate_IGUALES -      co.date = cd.date
**                 CDate_MENOR -        co.date < cd.date
**                 CDate_MAYOR -        co.date > cd.date
**                 CTime_IGUALES -      co.time = cd.time
**                 CTime_MENOR -        co.time < cd.time
**                 CTime_MAYOR -        co.time > cd.time
** ===================================================================
*/
uint8_t CRtc_Compare(CRtc *co, CRtc *cd){
    uint8_t err;
    
    //compara las times
    err = CTime_Compare(&co->time, &cd->time);
    
    // compara la date teniendo en cuenta la comparación de times anterior.
    err = CDate_Compare(&co->date, &cd->date, err);
        
    return(err);
}



/* ==================================================================================================
**	   Description :
**			CRtc_Copy copia la clase origen en el destino
** ===================================================================================================
*/
void CRtc_Copy(CRtc *org, CRtc *dest){
    CTime_Copy(&org->time, &dest->time);
    CDate_Copy(&org->date, &dest->date);
}



/*
** ===================================================================
**     Method      :  CRtc_CheckIntegrity
**
**     Description :
**         Comprueba si las diferentes propiedades están dentro de los rangos permitidos.
**     Parameters  :
**         CRtc *c  - estrucutura de referencia
**     Return :
**         uint8_t : ERR_OK       - rangos correctos
**                 ERR_VALUE    - fuera de rango
** ===================================================================
*/
uint8_t CRtc_CheckIntegrity(CRtc *c){
    uint8_t errCount = 0;
    CTime *time = (CTime *)&c->time;
    CDate *date = (CDate *)&c->date;
    
    // chequea la clase CTime
    if(time->seg > 59)  
        errCount++;
    if(time->min > 59)
        errCount++;
    if(time->hor > 23)
        errCount++;
    if(time->sem != CTime_LUNES && time->sem != CTime_MARTES && time->sem != CTime_MIERCOLES && time->sem != CTime_JUEVES && time->sem != CTime_VIERNES && time->sem != CTime_SABADO && time->sem != CTime_DOMINGO)
        errCount++;    
    
    // Chequea la clase CDate
    errCount += CDate_CheckIntegrity(date);
    if(errCount)
        return CAL_ERR_VALUE;
    else
        return CAL_ERR_OK;
}


/*
** ===================================================================
**     Method      :  CRtc_CheckStringIntegrity
**
**     Description :
**         Comprueba si la hora/fecha pasada como "HHMMSS" "DDMMAA" es
**			válida.
**     Parameters  :
**		   c	  -  Rtc updated after checking
**         stime  - "HHMMSS"
**		   sdate	"DDMMAA"
**     Return :
**         uint8_t : ERR_OK       - rangos correctos
**                 ERR_VALUE    - fuera de rango
** ===================================================================
*/
uint8_t CRtc_CheckStringIntegrity(CRtc * c, char *stime, char *sdate){
	c->time.hor = ((stime[0] - 48) * 10) + (stime[1] - 48);
	c->time.min = ((stime[2] - 48) * 10) + (stime[3] - 48);
	c->time.seg = ((stime[4] - 48) * 10) + (stime[5] - 48);
	c->date.dia = ((sdate[0] - 48) * 10) + (sdate[1] - 48);
	c->date.mes = ((sdate[2] - 48) * 10) + (sdate[3] - 48);
	c->date.ani = ((sdate[4] - 48) * 10) + (sdate[5] - 48);
	CRtc_CalcWeekday(c, 1);
	return(CRtc_CheckIntegrity(c));
}

/*
** ===================================================================
**     Method      :  CRtc_CalcWeekday
**     http://www.terra.es/personal2/grimmer/
**     Description :
**         Calcula el día de la semana en función de la fecha
**     Parameters  :
**         CRtc *c  - estrucutura de referencia
**         updateItself - indica si actualiza su propio día de la semana
**     Return :
**         uint8_t : devuelve el día de la semana (1:lun, 7:dom)
** ===================================================================
*/
uint8_t CRtc_CalcWeekday(CRtc *c, uint8_t updateItself){
    int16_t suma = 0;
    // see: http://www.terra.es/personal2/grimmer/#Monthcodes
    const uint8_t dayCodes[]={0,6,2,2,5,0,3,5,1,4,6,2,4};
    suma = (int16_t)(c->date.ani + (c->date.ani/4) + c->date.dia + dayCodes[c->date.mes]);
    while(suma > 7){
        suma -= 7;
    }
    if(c->date.ani % 4 == 0 && c->date.mes < 3){
        suma -= 1;
    }
    if(suma <= 0){
        suma += 7;        
    }
    
    if(updateItself){
        CTime_FormatSem(&c->time, (uint8_t)suma);
    }
    return((uint8_t)suma);    
}

/*
** ==================================================================================================
**                                    CCalendar
** ==================================================================================================
*/



static void 	CCalendar_CalcHolidays		(CCalendar *This);
static uint32_t CCalendar_CheckHolidays		(CCalendar *This, uint8_t *statChange);
static uint8_t 	CCalendar_CheckAndRepair	(CCalendar *c);

/*
** ==================================================================================================
**                                    SERVICIOS PRIVADOS
** ==================================================================================================
*/
/*
** ==================================================================================================
**     Method      :  CCalendar_CalcSeason
**
**     Description :
**         Comprueba si los cambios están activos, si no lo están termina.
**         Comprueba la date actual para ver si es bisiesto, así como el
**         tipo de cambio para actualizar las clases CRtc de verano e invierno.
**     Parameters  :
**         CCalendar *This : referencia al componente. 
**     Return :
** ==================================================================================================
*/
void CCalendar_CalcSeason(CCalendar *This, uint8_t *statChange){
    uint8_t ultimo_domingo;
    uint8_t ani = This->objRtc_sistema.date.ani;

    // sólo procesa si los cambios están activos.
    if(This->status & CCalendar_STAT_ENEST){
    
        *statChange |= CCalendar_STAT_ENEST;    // Indica que se actualizan los cambios de estación.
    
        // si el modo es AUTO, recalcula los últimos domingos de marzo y octubre.
        if(This->status & CCalendar_STAT_ESTAUTO){
        
            // dependiendo de la latitud, asigna Marzo a verano o a invierno.
            if(This->status & CCalendar_STAT_NORTE){
                // aplica la fórmula de cambio de estación automático :¡¡¡¡ LA OSTIA !!!!
                ultimo_domingo = (uint8_t)((26 - ani - (ani/4)) + (((24- (26 - ani - (ani/4)))/7)+1)*7);
                CDate_Set(&((This->objRtc_verano).date), ultimo_domingo, CDate_MARZO, ani);
                CTime_Set(&This->objRtc_verano.time, CTime_DOMINGO, 2, 0, 0);
                
                ultimo_domingo = (uint8_t)((29 - ani - (ani/4)) + (((24- (29 - ani - (ani/4)))/7)+1)*7);
                CDate_Set(&((This->objRtc_invierno).date), ultimo_domingo, CDate_OCTUBRE, ani);        
                CTime_Set(&((This->objRtc_invierno).time), CTime_DOMINGO, 3, 0, 0);
                
            }
            else{
                CDate_Set(&((This->objRtc_verano).date), 0, CDate_OCTUBRE, ani);
                CDate_Set(&((This->objRtc_invierno).date), 0, CDate_MARZO, ani);                            
            }
        }
        else{
        
            // actualiza al año en curso respetando la integridad. Sólo actualiza el año.
            CDate_Set(&((This->objRtc_verano).date), 0, 0, ani);
            CDate_Set(&((This->objRtc_invierno).date), 0, 0, ani);
        }
            
    }     
    CCalendar_CheckAndRepair(This);
}



 
/*
** ==================================================================================================
**     Method      :  CCalendar_CheckSeason
**
**     Description :
**         Si los cambios de estación están habilitados:
**         Compara la date y time actuales con la de los cambios de estación:
**         Si cambio_v < cambio_i -> latitud norte
**         Si cambio_v > cambio_i -> latitud sur
**         Si latitidud_norte:
**          Si time_actual > cambio_i -> invierno
**          Si time_actual < cambio_v -> inierno
**          sino -> verano
**         Si latitidud_sur:
**          Si time_actual > cambio_i -> verano
**          Si time_actual < cambio_v -> verano
**          sino -> invierno
**     Parameters  :
**         CCalendar *This : referencia al componente. 
**     Return :
**          CCalendar_ESVER : (=1/0) si es verano o no.
**     Notes: SA02#03: Todas las comparaciones tipo err & ... estaban equivocadas ya que no era
**                     necesario hacer un & sino un == ya que el valor que devuelve es absoluto.
**                     Además era necesario añadir un par de "else" para no generar un error.
**                     estos son los marcados con comentario "else_added"
** ==================================================================================================
*/
uint32_t CCalendar_CheckSeason(CCalendar *This, uint8_t *statChange){
    uint8_t err;
    CTime *ch;
    
    // SA14#03 - controla que inicialmente no se marque por error un cambio de estación
    *statChange &= ~CCalendar_EST_CHANGED;

    // si los cambios de estación no están activados, termina.
    if(!(This->status & CCalendar_STAT_ENEST))
        return(0);
       
    // si es LATITUD NORTE
    if(This->status & CCalendar_STAT_NORTE){
        
        // compara la time actual con la de verano.
        err = CRtc_Compare(&This->objRtc_sistema, &This->objRtc_verano);
        
        // si es menor, devuelve INVIERNO
        if((err & CDate_MENOR) || (err == (CDate_IGUALES + CTime_MENOR))){
            This->status &= ~CCalendar_STAT_ESVER;
        }
        
        // si es igual, realiza el cambio de estación a VERANO
        else if(err == (CDate_IGUALES + CTime_IGUALES)){
            ch = &((This->objRtc_sistema).time);
            ch->hor = (uint8_t)((ch->hor < 23)? ch->hor+1 : ch->hor); // SA02#05
            //CTime_Incr(ch);
            This->status |= CCalendar_STAT_ESVER;
            *statChange |= CCalendar_STAT_ESVER;        // notifica que hay cambio de estacion.
            // SA14#03 - marca el flag de cambio de estación
            *statChange |= CCalendar_EST_CHANGED;
        }
        //else_added
        else{
            // compara la time actual con la de invierno.
            err = CRtc_Compare(&This->objRtc_sistema, &This->objRtc_invierno);
            
            // si es menor, devuelve VERANO              
            if(err & CDate_MENOR){
                This->status |= CCalendar_STAT_ESVER;
            }
            // si es menor pero 1 hora antes el mismo día, devuelve VERANO                 SA08#02: se añade esta última comparación...
            else if((err == (CDate_IGUALES + CTime_MENOR)) && (This->objRtc_sistema.time.hor < (This->objRtc_invierno.time.hor-1))){
                This->status |= CCalendar_STAT_ESVER;
            }
            
            // si es igual, y es verano, realiza el cambio de estación a INVIERNO
            else if((err == (CDate_IGUALES + CTime_IGUALES)) && (This->status & CCalendar_STAT_ESVER)){
                ch = &This->objRtc_sistema.time;
                ch->hor = (uint8_t)((ch->hor > 0)? ch->hor-1 : ch->hor); // SA02#05
                This->status &= ~CCalendar_STAT_ESVER;
                *statChange |= CCalendar_STAT_ESVER;        // notifica que hay cambio de estacion.
                // SA14#03 - marca el flag de cambio de estación
                *statChange |= CCalendar_EST_CHANGED;
            }
            
            // en el resto de casos, es invierno
            else if(!((err == (CDate_IGUALES + CTime_MENOR)) && (This->objRtc_sistema.time.hor == (This->objRtc_invierno.time.hor-1)))){
                This->status &= ~CCalendar_STAT_ESVER;
            }
        }
    }
    
    // si es LATITUD_SUR
    else{
        // compara la time actual con la de invierno.
        err = CRtc_Compare(&This->objRtc_sistema, &This->objRtc_invierno);
        
        // si es menor, devuelve VERANO
        if((err & CDate_MENOR) || (err == (CDate_IGUALES + CTime_MENOR))){
            This->status |= CCalendar_STAT_ESVER;
        }
        
        // si es igual, y está en verano, realiza el cambio de estación a INVIERNO
        else if((err == (CDate_IGUALES + CTime_IGUALES)) && (This->status & CCalendar_STAT_ESVER)){
            ch = &((This->objRtc_sistema).time);
            ch->hor = (uint8_t)((ch->hor > 0)? ch->hor-1 : ch->hor); // SA02#05
            This->status &= ~CCalendar_STAT_ESVER;
            *statChange |= CCalendar_STAT_ESVER;        // notifica que hay cambio de estación.
            // SA14#03 - marca el flag de cambio de estación
            *statChange |= CCalendar_EST_CHANGED;
        }
        //else_added
        else{
            // compara la time actual con la de verano.
            err = CRtc_Compare(&This->objRtc_sistema, &This->objRtc_verano);
            
            // si es menor, devuelve INVIERNO                    SA08#01: se añade esta última comparación...
            if(err & CDate_MENOR){
                This->status &= ~CCalendar_STAT_ESVER;
            }
            else if(err == (CDate_IGUALES + CTime_MENOR) && This->objRtc_sistema.time.hor < (This->objRtc_verano.time.hor-1)){
                This->status &= ~CCalendar_STAT_ESVER;
            }
            // si es igual, realiza el cambio de estación a VERANO
            else if(err == (CDate_IGUALES + CTime_IGUALES)){
                ch = &((This->objRtc_sistema).time);
                ch->hor = (uint8_t)((ch->hor < 23)? ch->hor+1 : ch->hor); // SA02#05
                This->status |= CCalendar_STAT_ESVER;
                *statChange |= CCalendar_STAT_ESVER;        // notifica que hay cambio de estacion.
                // SA14#03 - marca el flag de cambio de estación
                *statChange |= CCalendar_EST_CHANGED;
            }
            
            // en el resto de casos, es verano
            else if(!(err == (CDate_IGUALES + CTime_MENOR) && This->objRtc_sistema.time.hor == (This->objRtc_verano.time.hor-1))){
                This->status |= CCalendar_STAT_ESVER;        
            }
        }
        
    }
    
    return(This->status & CCalendar_STAT_ESVER);
}


/*
** ==================================================================================================
**     Method      :  CCalendar_IsSummerTime
**
**     Description :
**         Chequea si está en verano
**     Parameters  :
**         CCalendar *This : referencia al componente. 
**     Return :
**          CCalendar_ESVAC : (=1/0) si está en verano o no.
** ==================================================================================================
*/
static uint8_t CCalendar_IsSummerTime(CCalendar *This){
	return ((uint8_t)((This->status & CCalendar_STAT_ESVER)?1:0));
}

/*
** ==================================================================================================
**     Method      :  CCalendar_CalcHolidays
**
**     Description :
**         Actualiza el año de las vacaciones al del reloj actual. Es 
**         necesario verificar la integridad de las dates ya que por
**         ejemplo puede haber una establecida el 29-02 y el año 
**         siguiente, esa date no es válida.
**     Parameters  :
**         CCalendar *This : referencia al componente. 
**     Return :
**          CCalendar_ESVAC : (=1/0) si está en vacaciones o no.
** ==================================================================================================
*/
static void CCalendar_CalcHolidays(CCalendar *This){
    uint8_t ani = This->objRtc_sistema.date.ani;
    // actualiza el año de los objetos vacaciones con el año del Calendario
    if(This->status & CCalendar_STAT_ENVAC){
        CDate_Set(&((This->objRtc_vacs_ini).date), 0, 0, ani);
        CDate_Set(&((This->objRtc_vacs_ini).date), 0, 0, ani);
        CCalendar_CheckAndRepair(This);        
    }
}



/*
** ==================================================================================================
**     Method      :  CCalendar_CheckHolidays
**
**     Description :
**         Comprueba si las vacaciones están activadas. Si es así, com-
**         prueba si está en un periodo de vacaciones. Hay que distinguir
**         que el inicio del periodo sea anterior o posterior a la finaliza-
**         ción.
**     Parameters  :
**         CCalendar *This : referencia al componente. 
**     Return :
**          CCalendar_ESVAC : (=1/0) si está en vacaciones o no.
** ==================================================================================================
*/
static uint32_t CCalendar_CheckHolidays(CCalendar *This, uint8_t *statChange){
    uint8_t err;
    
    // si las vacaciones no están activados, termina.
    if(!(This->status & CCalendar_STAT_ENVAC))
        return(0);
    
    // comprueba si se cumple la condición de inicio < fin o no
    err = CRtc_Compare(&This->objRtc_vacs_ini, &This->objRtc_vacs_fin);
    
    // si es INICIO < FINAL
    if(err & CDate_MENOR){
        
        // compara la time actual con la de inicio.
        err = CRtc_Compare(&This->objRtc_sistema, &This->objRtc_vacs_ini);
        
        // si es menor, devuelve NOVAC
        if((err & CDate_MENOR) || err & (CDate_IGUALES + CTime_MENOR)){
            This->status &= ~CCalendar_STAT_ESVAC;
        }
        
        // si es igual, pasa a modo vacaciones
        else if(err & (CDate_IGUALES + CTime_IGUALES)){
            This->status |= CCalendar_STAT_ESVAC;
            *statChange |= CCalendar_STAT_ESVAC;        // notifica que se actualiza el estado de las vacaciones
        }
        
        // compara la time actual con la de fin.
        err = CRtc_Compare(&This->objRtc_sistema, &This->objRtc_vacs_fin);
        
        // si es menor, devuelve ESVAC
        if((err & CDate_MENOR) || err & (CDate_IGUALES + CTime_MENOR)){
            This->status |= CCalendar_STAT_ESVAC;
        }
        
        // si es igual o mayor, sale de ESVAC
        else{
            This->status &= ~CCalendar_STAT_ESVAC;
            *statChange |= CCalendar_STAT_ESVAC;        // notifica que se actualiza el estado de las vacaciones
        }        
    }
    
    // si es INICIO > FINAL
    else{
        
        // compara la time actual con la de fin.
        err = CRtc_Compare(&This->objRtc_sistema, &This->objRtc_vacs_fin);
        
        // si es menor, devuelve ESVAC
        if((err & CDate_MENOR) || err & (CDate_IGUALES + CTime_MENOR)){
            This->status |= CCalendar_STAT_ESVAC;
        }
        
        // si es igual, pasa a NOVAC
        else if(err & (CDate_IGUALES + CTime_IGUALES)){
            This->status &= ~CCalendar_STAT_ESVAC;
            *statChange |= CCalendar_STAT_ESVAC;        // notifica que se actualiza el estado de las vacaciones
        }
        
        // compara la time actual con la de inicio.
        err = CRtc_Compare(&This->objRtc_sistema, &This->objRtc_vacs_ini);
        
        // si es menor, devuelve NOVAC
        if((err & CDate_MENOR) || err & (CDate_IGUALES + CTime_MENOR)){
            This->status &= ~CCalendar_STAT_ESVAC;
        }
        
        // si es igual o mayor, devuelve ESVAC
        else{
            This->status |= CCalendar_STAT_ESVAC;
            *statChange |= CCalendar_STAT_ESVAC;        // notifica que se actualiza el estado de las vacaciones
        }        
    }
    
    return(This->status & CCalendar_STAT_ESVAC);
}





/*
** ===================================================================
**     Method      :  CCalendar_CheckAndRepair
**
**     Description :
**         Comprueba si las diferentes propiedades están dentro de los rangos permitidos y
**          en caso contrario las ajusta a sus valores límite.
**     Parameters  :
**         CCalendar *c  - estrucutura de referencia
**     Return :
**         uint8_t : ERR_OK       - rangos correctos
**                 ERR_VALUE    - fuera de rango
** ===================================================================
*/
static uint8_t CCalendar_CheckAndRepair(CCalendar *c){
    uint8_t err = 0;
    if(CRtc_CheckIntegrity(&c->objRtc_sistema) != CAL_ERR_OK)
        err ++;
    if(CRtc_CheckIntegrity(&c->objRtc_verano) != CAL_ERR_OK)
        err ++;
    if(CRtc_CheckIntegrity(&c->objRtc_invierno) != CAL_ERR_OK)
        err ++;
    if(CRtc_CheckIntegrity(&c->objRtc_vacs_ini) != CAL_ERR_OK)
        err ++;
    if(CRtc_CheckIntegrity(&c->objRtc_vacs_fin) != CAL_ERR_OK)
        err ++;
    if(err)
        return CAL_ERR_VALUE;
    else
        return CAL_ERR_OK;    
}


/*
** ==================================================================================================
**                                    SERVICIOS PUBLICOS
** ==================================================================================================
*/
/*
** ===================================================================================================
**     Method      :  CCalendar_Init
**
**     Description :
**         Inicia el calendario a 1 de enero de 2008 a las 00:00. Con los
**         cambios de estación AUTO y sin vacaciones. El modo timerio será
**         24H. Asigna los punteros de función.
**     Parameters  :
**         CCalendar *c : componente sobre el que aplicar la función.
**     Return :
** ===================================================================================================
*/
void CCalendar_Init(CCalendar *c){
    CRtc *sys;                      //arg. evita **CRtc en las llamadas _set_time y _set_date.
    
    /* 
        Establece la time del sistema:
        00:00:00 a 1 - Enero - 2010
    */
    
    sys =  &(c->objRtc_sistema); 
    CRtc_SetTime(sys,CTime_VIERNES,0,0,0);
    CRtc_SetDate(sys,1u, CDate_ENERO, 10);
    
    /* 
        Establece la time del cambio a verano
        02:00:00 a 28 - Marzo - 2010
    */
    
    sys =  &(c->objRtc_verano); 
    CRtc_SetTime(sys,CTime_DOMINGO,2,0,0);
    CRtc_SetDate(sys,28u, CDate_MARZO, 10);

    /* 
        Establece la time del cambio a invierno
        03:00:00 a 31 - Octubre - 2010
    */
    
    sys =  &(c->objRtc_invierno); 
    CRtc_SetTime(sys,CTime_DOMINGO,3,0,0);
    CRtc_SetDate(sys,31u, CDate_OCTUBRE, 10);
   
    /* 
        Establece la time del inicio de vacaciones
        00:00:00 a 1 - Enero - 2010
    */
    
    sys =  &(c->objRtc_vacs_ini); 
    CRtc_SetTime(sys,CTime_VIERNES,0,0,0);
    CRtc_SetDate(sys,1u, CDate_ENERO, 10);

    /* 
        Establece la time del fin de vacaciones
        00:00:00 a 1 - Enero - 2010
    */
    
    sys =  &(c->objRtc_vacs_fin); 
    CRtc_SetTime(sys,CTime_VIERNES,0,0,0);
    CRtc_SetDate(sys,1u, CDate_ENERO, 10);

    /* 
        Establece los flags:
        Modo 24h, cambios de estación habilitados, con cambios automáticos en invierno 
        y sin vacaciones.
    */      
    c->status = (CCalendar_STAT_24H + CCalendar_STAT_ENEST + CCalendar_STAT_ESTAUTO + CCalendar_STAT_NORTE);        
}




/*
** ===================================================================================================
**     Method      :  CCalendar_Incr
**
**     Description :
**         actualiza el calendario
**     Parameters  :
**         CCalendar *This : referencia al componente. 
**     Return :
** ===================================================================================================
*/
uint32_t CCalendar_Incr(CCalendar *This, uint8_t *statChange){
    uint32_t toPost = 0;
      
    // Se actualiza el calendario.
    toPost = CRtc_Incr(&This->objRtc_sistema);

    // SA10#01 - en caso de que el año rebose y se ponga a 0 se reinicia todo
    if((toPost & EVENT_CMBANI) && This->objRtc_sistema.date.ani >= 100){
        CCalendar_Init(This);        
    }
    
    // Se procesan las acciones a realizar cada: seg-min-time-mediodia-dia-semana-mes-año
    if(toPost & EVENT_CMBSEG){
        
    }
    /*
        cada minuto si no hay cambio de año:
                - Cambio de estación.
                - Comprobar Entrada-salida de vacaciones.
    */
    if(toPost & EVENT_CMBMIN){
        if(!(toPost & EVENT_CMBANI)){
            CCalendar_CheckSeason(This, statChange);
            #if CCalendar_VACNDAY < 1
            CCalendar_CheckHolidays(This, statChange);            
            #endif
        }                
    }
    if(toPost & EVENT_CMBHOR){
        
    }
    // en el cambio de día hay que decrementar las vacaciones si están configuradas por nº de días
    if(toPost & EVENT_CMBDIA){
        #if CCalendar_VACNDAY > 0
        CCalendar_DecrementHolidays(This, statChange);        
        #endif
    }
    if(toPost & EVENT_CMBSEM){
        
    }
    if(toPost & EVENT_CMBMES){
        
    }
    /*
        cada año:
                - Calcular los nuevos cambios de estación.
                - Calcular los nuevos periodos de vacaciones.
                - Comprobar Entrada-salida de vacaciones.
    */
    if(toPost & EVENT_CMBANI){
        CCalendar_CalcSeason(This, statChange);
        CCalendar_CheckSeason(This, statChange);
        #if CCalendar_VACNDAY < 1
        CCalendar_CalcHolidays(This);
        CCalendar_CheckHolidays(This, statChange);                    
        #endif           
    }
           
    return toPost;

}          





/* ==================================================================================================
**	   Description :
**			CCalendar_Copy copia la clase origen en el destino
** ===================================================================================================
*/
void CCalendar_Copy(CCalendar *org, CCalendar *dest){
    dest->status = org->status;
    CRtc_Copy(&org->objRtc_sistema, &dest->objRtc_sistema);
    CRtc_Copy(&org->objRtc_verano, &dest->objRtc_verano);
    CRtc_Copy(&org->objRtc_invierno, &dest->objRtc_invierno);
    #if CCalendar_VACNDAY > 0
    dest->objRtc_vacs_ini.time.seg = org->objRtc_vacs_ini.time.seg;
    #else
    CRtc_Copy(&org->objRtc_vacs_ini, &dest->objRtc_vacs_ini);
    CRtc_Copy(&org->objRtc_vacs_fin, &dest->objRtc_vacs_fin);                                
    #endif
}




/* ==================================================================================================
**	   Description :
**			CCalendar_CopyConfig copia la clase origen en el destino excepto la parte del reloj
** ===================================================================================================
*/
void CCalendar_CopyConfig(CCalendar *org, CCalendar *dest){
    dest->status = org->status;
    CRtc_Copy(&org->objRtc_verano, &dest->objRtc_verano);
    CRtc_Copy(&org->objRtc_invierno, &dest->objRtc_invierno);
    #if CCalendar_VACNDAY > 0
    dest->objRtc_vacs_ini.time.seg = org->objRtc_vacs_ini.time.seg;
    #else
    CRtc_Copy(&org->objRtc_vacs_ini, &dest->objRtc_vacs_ini);
    CRtc_Copy(&org->objRtc_vacs_fin, &dest->objRtc_vacs_fin);                                
    #endif
}


                                                  
/*
** ===================================================================
**     Method      :  CRtc_CheckIntegrity
**
**     Description :
**         Comprueba si las diferentes propiedades están dentro de los rangos permitidos.
**     Parameters  :
**         CCalendar *c  - estrucutura de referencia
**     Return :
**         uint8_t : ERR_OK       - rangos correctos
**                 ERR_VALUE    - fuera de rango
** ===================================================================
*/
uint8_t CCalendar_CheckIntegrity(CCalendar *c){
    if(CCalendar_CheckConfigIntegrity(c) != CAL_ERR_OK)
        return CAL_ERR_VALUE;
    if(CRtc_CheckIntegrity(&c->objRtc_sistema) != CAL_ERR_OK)
        return CAL_ERR_VALUE;
    return CAL_ERR_OK;
}


/*
** ===================================================================
**     Method      :  CCalendar_CheckConfigIntegrity
**
**     Description :
**         Comprueba únicamente la parte de configuración, es decir 
**          todo menos el RTC
**     Parameters  :
**         CCalendar *c  - estrucutura de referencia
**     Return :
**         uint8_t : ERR_OK       - rangos correctos
**                 ERR_VALUE    - fuera de rango
** ===================================================================
*/
uint8_t CCalendar_CheckConfigIntegrity(CCalendar *c){
    if(CRtc_CheckIntegrity(&c->objRtc_verano) != CAL_ERR_OK)
        return CAL_ERR_VALUE;
    if(CRtc_CheckIntegrity(&c->objRtc_invierno) != CAL_ERR_OK)
        return CAL_ERR_VALUE;
    // en el caso de que las vacaciones sean por nº de días, controla que sólo valgan de 1 a 31.
    #if CCalendar_VACNDAY > 0
    c->objRtc_vacs_ini.time.seg = (uint8_t)((c->objRtc_vacs_ini.time.seg > CCalendar_MAX_N_DAYS)? CCalendar_MAX_N_DAYS : c->objRtc_vacs_ini.time.seg);
    c->objRtc_vacs_ini.time.seg = (uint8_t)((c->objRtc_vacs_ini.time.seg == 0)? 1 : c->objRtc_vacs_ini.time.seg);
    return CAL_ERR_OK;
    
    #else
    if(CRtc_CheckIntegrity(&c->objRtc_vacs_ini) != CAL_ERR_OK)
        return CAL_ERR_VALUE;
    if(CRtc_CheckIntegrity(&c->objRtc_vacs_fin) != CAL_ERR_OK)
        return CAL_ERR_VALUE;
    
    return CAL_ERR_OK;    
    #endif
}


/****************************************************************************************************//**
 * @fun		CCalendar_GetFromSeconds
 * @brief	Get time and date from time counter seconds
 * @param	cal		DateTime structure
 * @param	counter	Time count
 * @return	
 **********************************************************************************/
uint8_t CCalendar_GetFromSeconds(CCalendar *cal, uint32_t counter){
	uint32_t dias;
	uint32_t segundosHoy;
	uint8_t  horasHoy;
	uint8_t  minutosHoy;
	uint8_t stats;
	uint8_t cuatrenios;
	uint8_t anios;
	uint32_t diasRestantes;
	uint16_t *pDia;
	uint8_t i;
	const uint32_t SIZE_CUATRENIO = 365+365+365+366;
	const uint32_t COUNTER_AT_2010 = 283996800;
	const uint16_t dias_bisiesto[12] = { 0, 31, 31+29, 60+31, 91+30, 121+31, 152+30, 182+31, 213+31, 244+30, 274+31, 305+30};
	const uint16_t dias_no_bisiesto[12] = { 0, 31, 31+28, 59+31, 90+30, 120+31, 151+30, 181+31, 212+31, 243+30, 273+31, 304+30};

	if(counter < COUNTER_AT_2010){
		CCalendar_Init(cal);
		return 1;
	}
	dias = counter / 86400;
	segundosHoy = counter - (86400 * dias);
	horasHoy = (uint8_t)(segundosHoy / 3600);
	segundosHoy = segundosHoy - (3600 * horasHoy);
	minutosHoy = (uint8_t)(segundosHoy / 60);
	segundosHoy = segundosHoy - (60 * minutosHoy);
	cal->objRtc_sistema.time.seg = (uint8_t)segundosHoy;
	cal->objRtc_sistema.time.min = minutosHoy;
	cal->objRtc_sistema.time.hor = horasHoy;

	cuatrenios = (uint8_t)(dias / SIZE_CUATRENIO);
	diasRestantes = dias - (cuatrenios * SIZE_CUATRENIO);

	anios = 4 * cuatrenios;
	while(diasRestantes >= 365){
		anios ++;
		diasRestantes -= 365;		
	}
	cal->objRtc_sistema.date.ani = anios + 1;

	pDia = (uint16_t *)dias_no_bisiesto;
	if(CDate_IsLeapYear(&cal->objRtc_sistema.date)){
		pDia = (uint16_t *)dias_bisiesto;
	}

	for(i=1;i<12;i++){
		if(diasRestantes >= pDia[i-1] && diasRestantes < pDia[i]){
			break;
		}
	}
	cal->objRtc_sistema.date.mes = i;
	cal->objRtc_sistema.date.dia = diasRestantes + 1 - pDia[i-1];
	CCalendar_CalcSeason(cal, &stats);
	CCalendar_CheckSeason(cal, &stats);
	CRtc_CalcWeekday(&cal->objRtc_sistema, 1);
	// if summer but no changed right now, adds 1 hour and fix it
	if(CCalendar_IsSummerTime(cal) && (stats & CCalendar_EST_CHANGED) == 0){
		if(++cal->objRtc_sistema.time.hor > 23){
			cal->objRtc_sistema.time.hor = 0;
			cal->objRtc_sistema.date.dia++;
			if(CDate_CheckIntegrity(&cal->objRtc_sistema.date) > 0){
				cal->objRtc_sistema.date.dia = 1;
				cal->objRtc_sistema.date.mes++;
			}
		}
	}
	CCalendar_CalcSeason(cal, &stats);
	CCalendar_CheckSeason(cal, &stats);
	CRtc_CalcWeekday(&cal->objRtc_sistema, 1);
	return 0;
}



/****************************************************************************************************//**
 * @fun		CCalendar_GetInSeconds
 * @brief	Get counter (seconds) from calendar
 * @param	cal		calendar
 * @param	counter	Time count
 * @return	number of seconds since 01/01/2000
 **********************************************************************************/
uint32_t CCalendar_GetInSeconds(CCalendar *cal){
	uint32_t counter = 0;
	counter += CDate_DaysTillYear(cal->objRtc_sistema.date.ani);
	counter += CDate_DaysThisYear(&cal->objRtc_sistema.date);
	// counter in seconds
	counter *= 86400;
	// if summer, substract 1 hour = 3600 seconds
	if(CCalendar_IsSummerTime(cal)){
		counter -= 3600;
	}
	// add seconds for today
	counter += (3600 * cal->objRtc_sistema.time.hor);
	counter += (60 * cal->objRtc_sistema.time.min);
	counter += cal->objRtc_sistema.time.seg;
	return counter;
}																																	  



/****************************************************************************************************//**
 * @fun		CCalendar_AddOneHour
 * @brief	Increments one hour and keep track of the calendar update
 * @param	cal		calendar 
 * @return	
 **********************************************************************************/
void CCalendar_AddOneHour(CCalendar *cal){
	uint8_t notused;
	if(++cal->objRtc_sistema.time.hor > 23){
		cal->objRtc_sistema.time.hor = 0;
		cal->objRtc_sistema.date.dia++;
		if(CDate_CheckIntegrity(&cal->objRtc_sistema.date) > 0){
			cal->objRtc_sistema.date.mes++;
			if(CDate_CheckIntegrity(&cal->objRtc_sistema.date) > 0){
				cal->objRtc_sistema.date.ani++;
			}
		}
		CRtc_CalcWeekday(&cal->objRtc_sistema, 1);
		CCalendar_CalcSeason(cal, &notused);
		CCalendar_CheckSeason(cal, &notused);
		CRtc_CalcWeekday(&cal->objRtc_sistema, 1);
	}
}



/****************************************************************************************************//**
 * @fun		CCalendar_SubOneHour
 * @brief	Decrements one hour and keep track of the calendar update
 * @param	cal		calendar 
 * @return	
 **********************************************************************************/
void CCalendar_SubOneHour(CCalendar *cal){
	uint8_t notused;
	if(cal->objRtc_sistema.time.hor == 0){
		cal->objRtc_sistema.time.hor = 23;
		CDate_Decr(&cal->objRtc_sistema.date);
	}
	else{
		cal->objRtc_sistema.time.hor --;
	}
	CRtc_CalcWeekday(&cal->objRtc_sistema, 1);
	CCalendar_CalcSeason(cal, &notused);
	CCalendar_CheckSeason(cal, &notused);
	CRtc_CalcWeekday(&cal->objRtc_sistema, 1);
}


/****************************************************************************************************//**
 * @fun		CCalendar_SetFromUTC
 * @brief	Sets time/date calendar localtime with UTC clock
 * @param	cal		calendar created
 * @param	utc		utc time/date
 * @return	
 **********************************************************************************/
void CCalendar_SetFromUTC(CCalendar *cal, CRtc *utc, int8_t utcRef){
	uint8_t notused;
	cal->objRtc_sistema = *utc;
	CRtc_CalcWeekday(&cal->objRtc_sistema, 1);
	CCalendar_CalcSeason(cal, &notused);
	CCalendar_CheckSeason(cal, &notused);
	CRtc_CalcWeekday(&cal->objRtc_sistema, 1);
	if(utcRef > 0){
		int8_t i;
		for(i=0;i<utcRef;i++)
			CCalendar_AddOneHour(cal);
	}
	else{
		int8_t i;
		for(i=0;i<utcRef;i++)
			CCalendar_SubOneHour(cal);
	}
	if(CCalendar_IsSummerTime(cal)){
		CCalendar_AddOneHour(cal);
	}
}



