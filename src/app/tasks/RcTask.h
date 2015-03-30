/*
 * RcTask.h
 *
 *  Created on: 13/3/2015
 *      Author: raulMrello
 *
 *  RcTask will manage GPIO input actions like pushbuttons press and release. In order to
 *  process both events, this task will be attached to topics /push and /release. Also, it
 *  will keep an internal timer to know if a pushbutton is pressed for a long time.
 *  Depending on the input combination, it will update /rc topics for other modules.
 */

#ifndef SRC_MODULES_RCTASK_H_
#define SRC_MODULES_RCTASK_H_

#include "../../mmf_c/os/mmf.h"

/** \def RcTaskPtr
 *  \brief Definition to abstract the callback handler into void* type
 */
typedef void* RcTaskPtr;

/** \fn RcTask_init
 *  \brief RcTask initialization callback
 *  \param t RcTask task object
 */
void RcTask_init(RcTaskPtr t);

/** \fn RcTask_OnYieldTurn
 *  \brief RcTask on yield turn events callback
 *  \param t RcTask task object
 */
void RcTask_OnYieldTurn(RcTaskPtr t);

/** \fn RcTask_OnResume
 *  \brief RcTask on resume events callback
 *  \param t RcTask task object
 */
void RcTask_OnResume(RcTaskPtr t);

/** \fn RcTask_OnEventFlag
 *  \brief RcTask on event flags callback
 *  \param t RcTask task object
 *  \param event Event raised
 */
void RcTask_OnEventFlag(RcTaskPtr t, int event);

/** \fn RcTask_OnTopicUpdate
 *  \brief RcTask on topic update events callback
 *  \param t RcTask task object
 *  \param td Topic data
 */
void RcTask_OnTopicUpdate(RcTaskPtr t, TopicData * td);



#endif /* SRC_MODULES_RCTASK_H_ */
