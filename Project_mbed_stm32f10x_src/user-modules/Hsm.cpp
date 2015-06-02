/** ###################################################################
**     Filename  : StateFramework.c
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

#include "StateFramework.h"

const Event timeoutEvt = {TIMEOUT_SIG};

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
**         Constructor de una máquina de estados. Asigna al miembro <init> la función pasada como
**          argumento, que se corresponde con el estado Initial.
**     Parameters  :
**          me:     Componente que gestiona la máquina de estados
**          Init:   Función que maneja el estado Initial
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
**         Invoca la ejecución del dispatcher sin ningún evento. Esto implica que se ejecute el 
**          primer manejador, que se corresponde con el del estado Initial.
**     Parameters  :
**          me:     Componente que gestiona la máquina de estados
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
**         Es el manejador del superestado de mayor rango en la jerarquía. Lo único que hace es manejar
**          los eventos delegados por los estados hijo, ignorándolos.
**     Parameters  :
**          me:     Componente que gestiona la máquina de estados
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
**         Es el dispatcher del Framework. Se encarga de evaluar a qué estado hay que ceder el control
**          y cuál es el evento que se procesa en cada momento.
**     Parameters  :
**          me:     Componente que gestiona la máquina de estados
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
    
    // bucle hasta que la propagación de eventos y/o transiciones termine
    while(MUST_CONTINUE){
        // la primera vez, procesa el evento que invoca al dispatcher
        if(IS_FIRST_TIME){
            result = me->state(pThis, e);
            flags &= ~FIRST_TIME;            
        }
        // para la primera y sucesivas transicion y/o delegación de eventos...
        // verifica el resultado de la última ejecución del dispatcher        
        switch(result){
            // si el evento se ha controlado, termina abruptamente.
            case RET_HANDLED:
            case RET_IGNORED:
                return;       //flags |= EXIT_NOW;break;
            // si hay que realizar una transición de estado...                
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
                // se invoca al manejador de estados del padre, pasándole el evento delegado.
                EnterCritical();
                superState = me->target;
                ExitCritical();
                result = superState(pThis, e);
                break;
        }
        
    }
    
}
