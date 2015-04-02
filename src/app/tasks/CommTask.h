/*
 * CommTask.h
 *
 *  Created on: 13/3/2015
 *      Author: raulMrello
 *
 *  CommTask receives updates from /rc and /gps topics and also /com topics from the
 *  remote wifi link. And generates /data topics.
 */

#ifndef SRC_MODULES_COMMTASK_H_
#define SRC_MODULES_COMMTASK_H_

#include "../../mmf_c/os/mmf.h"

/** \def CommTaskPtr
 *  \brief Definition to abstract the callback handler into void* type
 */
typedef void* CommTaskPtr;

/** \fn CommTask_init
 *  \brief CommTask initialization callback
 *  \param t CommTask task object
 */
void CommTask_init(CommTaskPtr t);

/** \fn CommTask_OnYieldTurn
 *  \brief CommTask on yield turn events callback
 *  \param t CommTask task object
 */
void CommTask_OnYieldTurn(CommTaskPtr t);

/** \fn CommTask_OnResume
 *  \brief CommTask on resume events callback
 *  \param t CommTask task object
 */
void CommTask_OnResume(CommTaskPtr t);

/** \fn CommTask_OnEventFlag
 *  \brief CommTask on event flags callback
 *  \param t CommTask task object
 *  \param event Event raised
 */
void CommTask_OnEventFlag(CommTaskPtr t, int event);

/** \fn CommTask_OnTopicUpdate
 *  \brief CommTask on topic update events callback
 *  \param t CommTask task object
 *  \param td Topic data
 */
void CommTask_OnTopicUpdate(CommTaskPtr t, TopicData * td);



#endif /* SRC_MODULES_COMMTASK_H_ */
