/** ###################################################################
**     Filename  : StateFramework.h
**     Date/Time : 18.02.2009
**     Abstract  :
**         M�dulo de gesti�n del Framework de estados, basado en la
**          metodolog�a de Miro Samek (Quantum Leaps), pero mucho m�s 
**          ligero y optimizado.
**          No es 100% UML-compliant, pero implementa correctamente los
**          eventos ENTRY, EXIT, as� como la herencia en las m�quinas de
**          estados jer�rquicas.
**         
**         NOTAS: En los eventos EXIT, no se pueden realizar transiciones
**                a otras m�quinas, ya que estos eventos s�lo se utilizan
**                como destructores de datos. Sin embargo, en los eventos
**                ENTRY si es posible saltar a subm�quinas.
**     Settings  :
**     Contents  :
**     Realizado :
**         Raul Martin
** ###################################################################*/
#ifndef _STATEFRAMEWORK_H
#define _STATEFRAMEWORK_H

/****************************************************************************************************//**
 *                        REQUIRED LIBRARIES
 ********************************************************************************************************/
#include <stdint.h>
#include <RTL.h>		// porting to RTX RTOS critical section 
#define EnterCritical()			tsk_lock()
#define ExitCritical()			tsk_unlock()

/****************************************************************************************************//**
 *                        PUBLIC DEFINITIONS
 ********************************************************************************************************/

typedef uint8_t State;
typedef void    Object;

                               
/**************************************************************
            Enumera los eventos reservados al Framework
**************************************************************/

enum ReservedSignals {
    ENTRY_SIG = 1,          /**< evento de entrada */
    EXIT_SIG,               /**< evento de salida */
	TIMEOUT_SIG,			/**< evento de timeout */
    USER_SIG                /**< primer evento libre para el usuario */
};

/**************************************************************
            Estructura de los eventos manejados por el Framework
**************************************************************/

typedef struct{
    uint32_t sig;               /**< c�digo del evento */
}Event;

extern const Event timeoutEvt;

/**************************************************************
            Tipo definido para cada manejador de estado
            Nombre: StateHandler
            Param0: me (componente activo)
            Param1: e  (evento)
**************************************************************/

typedef State (*StateHandler)(Object *pThis, Event const *e);

/**************************************************************
            Estructura de una m�quina de estados jer�rquica            
**************************************************************/

typedef struct{
    StateHandler init;     /**< manejador del estado Initial */
    StateHandler state;    /**< manejador del estado actual */
    StateHandler target;   /**< manjeador del estado al que conmutar */
} StateMachine;

/**************************************************************
            Tipo de datos devueltos por los manejadores de estado            
**************************************************************/

#define RET_HANDLED       ((State)0u)     /**< indica que el evento se ha procesado */
#define RET_IGNORED       ((State)1u)     /**< indica que el evento se ha controlado pero se ignora */
#define RET_TRAN          ((State)2u)     /**< indica que hay una transici�n de estados */
#define RET_SUPER         ((State)3u)     /**< indica que el evento se ha delegado al estado padre */



/*
** ==================================================================================================
**     Method      :  StateMachine_Ctor
**
**     Description :
**         Constructor de una m�quina de estados. Asigna al miembro <init> la funci�n pasada como
**          argumento, que se corresponde con el estado Initial.
**     Parameters  :
**          me:     Componente que gestiona la m�quina de estados
**          Init:   Funci�n que maneja el estado Initial
**     Return :
** ==================================================================================================
*/
void StateMachine_Ctor(Object *pThis, StateHandler Init);



/*
** ==================================================================================================
**     Method      :  StateMachine_Init
**
**     Description :
**         Invoca la ejecuci�n del dispatcher sin ning�n evento. Esto implica que se ejecute el 
**          primer manejador, que se corresponde con el del estado Initial.
**     Parameters  :
**          me:     Componente que gestiona la m�quina de estados
**     Return :
** ==================================================================================================
*/
void StateMachine_Init(Object *pThis);



/*
** ==================================================================================================
**     Method      :  StateMachine_Top
**
**     Description :
**         Es el manejador del superestado de mayor rango en la jerarqu�a. Lo �nico que hace es manejar
**          los eventos delegados por los estados hijo, ignor�ndolos.
**     Parameters  :
**          me:     Componente que gestiona la m�quina de estados
**          e:      evento delegado desde los estados hijo.
**     Return :
** ==================================================================================================
*/
State StateMachine_Top(Object *pThis, Event const *e);



/*
** ==================================================================================================
**     Method      :  StateMachine_Dispatch
**
**     Description :
**         Es el dispatcher del Framework. Se encarga de evaluar a qu� estado hay que ceder el control
**          y cu�l es el evento que se procesa en cada momento.
**     Parameters  :
**          me:     Componente que gestiona la m�quina de estados
**          e:      evento que invoca al dispatcher.
**     Return :
** ==================================================================================================
*/
void StateMachine_Dispatch(Object *pThis, Event const *e);



/*
** ==================================================================================================
**     Method      :  ReturningFromState
**
**     Description :
**         Este m�todo permite que la llamada a las macros TRAN, HANDLED, IGNORED, SUPER, THROW tenga
**          una terminaci�n coherente con la forma de trabajo del dispatcher. Se encarga de fijar el
**          estado a cambiar (target) en una operaci�n TRAN y de devolver en cada caso el identificador
**          del tipo de macro ejecutada.
**     Parameters  :
**          vme:    Componente que gestiona la m�quina de estados
**          retVal: C�digo correspondiente al tipo de macro ejecutada
**          state:  Estado objetivo.
**     Return :
** ==================================================================================================
*/
State ReturningFromState(Object *pThis, State retVal, StateHandler state);



/**************************************************************
            Macros de finalizaci�n de estado
            
            HANDLED: permite terminar un estado indicando que
                     el evento se ha procesado.
            IGNORED: permite terminar un estado indicando que
                     el evento se ha procesado ignor�ndolo.
            TRAN:    permite terminar un estado indicando al
                     dispatcher el nuevo estado al que conmutar
                     (target_).
            SUPER:   permite terminar un estado indicando al
                     dispatcher que el evento no se ha controlado
                     y que debe delegarse al estado padre (super_).                            
**************************************************************/

#define HANDLED()		return(RET_HANDLED)    
#define IGNORED()       return(RET_IGNORED)
    
#define TRAN(st_)       {((StateMachine *)me)->target = (StateHandler)st_;return(RET_TRAN);}

#define SUPER(st_)      {((StateMachine *)me)->target = (StateHandler)(st_);return(RET_SUPER);}

#define DELEGATE(st_)	SUPER(st_)

//#define TRAN(st_)       ((StateMachine*)me)->target=(StateHandler)st_;return(RET_TRAN)
//#define TRAN(target_)  (ReturningFromState(me, RET_TRAN, target_))
//#define SUPER(super_)  (ReturningFromState(me, RET_SUPER, super_))


#endif

