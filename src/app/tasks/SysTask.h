/*
 * SysTask.h
 *
 *  Created on: 05/04/2015
 *      Author: raulMrello
 *
 *  SysTask is the system task that runs at lowest priority level. Its main function is to carry out
 *  power policies or in other cases, execute different test situations like systick simulations.
 *
 */

#ifndef SRC_TASKS_SYSTASK_H_
#define SRC_TASKS_SYSTASK_H_

#include "../../mmf_c/os/mmf.h"


/** \def SubscriberTask
 *  \brief Definition to abstract the callback handler into void* type
 */
typedef void* SysTaskPtr;

/** \fn SysTask_init
 *  \brief SysTask initialization callback
 *  \param t SysTask task object
 */
void SysTask_init(SysTaskPtr t);

/** \fn SysTask_OnYieldTurn
 *  \brief SysTask on yield turn events callback
 *  \param t SysTask task object
 */
void SysTask_OnYieldTurn(SysTaskPtr t);

/** \fn SysTask_OnResume
 *  \brief SysTask on resume events callback
 *  \param t SysTask task object
 */
void SysTask_OnResume(SysTaskPtr t);

/** \fn SysTask_OnEventFlag
 *  \brief SysTask on event flags callback
 *  \param t SysTask task object
 *  \param event Event raised
 */
void SysTask_OnEventFlag(SysTaskPtr t, int event);

/** \fn SysTask_OnTopicUpdate
 *  \brief SysTask on topic update events callback
 *  \param t SysTask task object
 *  \param td Topic data
 */
void SysTask_OnTopicUpdate(SysTaskPtr t, TopicData * td);

#endif /* SRC_TASKS_SYSTASK_H_ */
