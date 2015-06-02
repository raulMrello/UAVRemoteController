/** ###################################################################
**     Filename  : StateFramework.c
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

#include "StateFramework.h"

const Event timeoutEvt = {TIMEOUT_SIG};

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
State ReturningFromState(Object *pThis, State retVal, StateHandler state){
    StateMachine *me = (StateMachine *)pThis;
     switch(retVal){
        case RET_HANDLED:
        case RET_IGNORED:
            return retVal;
        case RET_TRAN:
        case RET_SUPER:
            EnterCritical();
            me->target = state;         // fija el estado objetivo
            ExitCritical();
            return retVal;
    }
    return retVal;
}



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
void StateMachine_Ctor(Object *pThis, StateHandler Init){
    StateMachine *me = (StateMachine *)pThis;
    // Establece como estado inicial el manejador del estado Initial <Init>.
    EnterCritical();
    me->init = Init;
    me->state = me->init;
    ExitCritical();

}



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
void StateMachine_Init(Object *pThis){
    // Lanza por primera vez el dispatcher
    StateMachine_Dispatch(pThis, (Event *)0);
}



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
State StateMachine_Top(Object *pThis, Event const *e) {
    (void)pThis;
    (void)e;              /* supress the "unused argument" compiler warning */
    IGNORED();                   /* the top state ignores all events */
}





/**************************************************************
            Flags utilizados por el dispatcher                            
**************************************************************/

enum DispatchingFlags{
    FIRST_TIME  = 1u,
    EXIT_NOW    = 2u,
};
#define IS_FIRST_TIME   (flags & FIRST_TIME)
#define MUST_CONTINUE   (!(flags & EXIT_NOW))

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
void StateMachine_Dispatch(Object *pThis, Event const *e){
    StateMachine *me = (StateMachine *)pThis;
    Event localEvt;
    State result;
	StateHandler exitState;
	StateHandler superState;
    uint8_t flags = FIRST_TIME;
    
    // bucle hasta que la propagaci�n de eventos y/o transiciones termine
    while(MUST_CONTINUE){
        // la primera vez, procesa el evento que invoca al dispatcher
        if(IS_FIRST_TIME){
            result = me->state(pThis, e);
            flags &= ~FIRST_TIME;            
        }
        // para la primera y sucesivas transicion y/o delegaci�n de eventos...
        // verifica el resultado de la �ltima ejecuci�n del dispatcher        
        switch(result){
            // si el evento se ha controlado, termina abruptamente.
            case RET_HANDLED:
            case RET_IGNORED:
                return;       //flags |= EXIT_NOW;break;
            // si hay que realizar una transici�n de estado...                
            case RET_TRAN:               
                // primero ejecuta el EXIT del estado actual y posteriormente el ENTRY del nuevo estado.
                localEvt.sig = EXIT_SIG;
                exitState = me->state;
                EnterCritical();
                me->state = me->target;
                ExitCritical();
                if(exitState != me->init)
                    exitState(pThis, &localEvt);
                localEvt.sig = ENTRY_SIG;
                result = me->state(pThis, &localEvt);
                break;
            // si el evento se delega al estado padre...    
            case RET_SUPER:
                // se invoca al manejador de estados del padre, pas�ndole el evento delegado.
                EnterCritical();
                superState = me->target;
                ExitCritical();
                result = superState(pThis, e);
                break;
        }
        
    }
    
}
