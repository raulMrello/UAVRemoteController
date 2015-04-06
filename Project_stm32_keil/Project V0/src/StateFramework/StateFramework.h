/** ###################################################################
**     Filename  : StateFramework.h
**     Date/Time : 18.02.2009
**     Abstract  :
**         Módulo de gestión del Framework de estados, basado en la
**          metodología de Miro Samek (Quantum Leaps), pero mucho más 
**          ligero y optimizado.
**          No es 100% UML-compliant, pero implementa correctamente los
**          eventos ENTRY, EXIT, así como la herencia en las máquinas de
**          estados jerárquicas.
**         
**         NOTAS: En los eventos EXIT, no se pueden realizar transiciones
**                a otras máquinas, ya que estos eventos sólo se utilizan
**                como destructores de datos. Sin embargo, en los eventos
**                ENTRY si es posible saltar a submáquinas.
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
    uint32_t sig;               /**< código del evento */
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
            Estructura de una máquina de estados jerárquica            
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
#define RET_TRAN          ((State)2u)     /**< indica que hay una transición de estados */
#define RET_SUPER         ((State)3u)     /**< indica que el evento se ha delegado al estado padre */



/*
** ==================================================================================================
**     Method      :  StateMachine_Ctor
**
**     Description :
**         Constructor de una máquina de estados. Asigna al miembro <init> la función pasada como
**          argumento, que se corresponde con el estado Initial.
**     Parameters  :
**          me:     Componente que gestiona la máquina de estados
**          Init:   Función que maneja el estado Initial
**     Return :
** ==================================================================================================
*/
void StateMachine_Ctor(Object *pThis, StateHandler Init);



/*
** ==================================================================================================
**     Method      :  StateMachine_Init
**
**     Description :
**         Invoca la ejecución del dispatcher sin ningún evento. Esto implica que se ejecute el 
**          primer manejador, que se corresponde con el del estado Initial.
**     Parameters  :
**          me:     Componente que gestiona la máquina de estados
**     Return :
** ==================================================================================================
*/
void StateMachine_Init(Object *pThis);



/*
** ==================================================================================================
**     Method      :  StateMachine_Top
**
**     Description :
**         Es el manejador del superestado de mayor rango en la jerarquía. Lo único que hace es manejar
**          los eventos delegados por los estados hijo, ignorándolos.
**     Parameters  :
**          me:     Componente que gestiona la máquina de estados
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
**         Es el dispatcher del Framework. Se encarga de evaluar a qué estado hay que ceder el control
**          y cuál es el evento que se procesa en cada momento.
**     Parameters  :
**          me:     Componente que gestiona la máquina de estados
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
**         Este método permite que la llamada a las macros TRAN, HANDLED, IGNORED, SUPER, THROW tenga
**          una terminación coherente con la forma de trabajo del dispatcher. Se encarga de fijar el
**          estado a cambiar (target) en una operación TRAN y de devolver en cada caso el identificador
**          del tipo de macro ejecutada.
**     Parameters  :
**          vme:    Componente que gestiona la máquina de estados
**          retVal: Código correspondiente al tipo de macro ejecutada
**          state:  Estado objetivo.
**     Return :
** ==================================================================================================
*/
State ReturningFromState(Object *pThis, State retVal, StateHandler state);



/**************************************************************
            Macros de finalización de estado
            
            HANDLED: permite terminar un estado indicando que
                     el evento se ha procesado.
            IGNORED: permite terminar un estado indicando que
                     el evento se ha procesado ignorándolo.
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

