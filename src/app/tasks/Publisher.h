/*
 * Publisher.h
 *
 *  Created on: 13/3/2015
 *      Author: RaulM
 *
 *  Publisher task is demostrative task to post different event conditions to a Subscriber. It could raise events or
 *  topic updates
 *
 *  Example of use:
 *
 *  1 - It will initialize
 *  2 - It will publish topic updates
 *  3 - Sometimes it will send an event to the Subscriber or a combination of events
 */

#ifndef SRC_MODULES_PUBLISHER_H_
#define SRC_MODULES_PUBLISHER_H_

#include "../../../src/mmf/c/os/mmf.h"

/** \def PublisherTaskPtr
 *  \brief Definition to abstract the callback handler into void* type
 */
typedef void* PublisherTaskPtr;

/** \fn Publisher_init
 *  \brief Publisher initialization callback
 *  \param t Publisher task object
 */
void Publisher_init(PublisherTaskPtr t);

/** \fn Publisher_OnYieldTurn
 *  \brief Publisher on yield turn events callback
 *  \param t Publisher task object
 */
void Publisher_OnYieldTurn(PublisherTaskPtr t);

/** \fn Publisher_OnResume
 *  \brief Publisher on resume events callback
 *  \param t Publisher task object
 */
void Publisher_OnResume(PublisherTaskPtr t);

/** \fn Publisher_OnEventFlag
 *  \brief Publisher on event flags callback
 *  \param t Publisher task object
 *  \param event Event raised
 */
void Publisher_OnEventFlag(PublisherTaskPtr t, int event);

///** \fn SPublisher_OnTopicUpdate
// *  \brief Publisher on topic update events callback
// *  \param t Publisher task object
// *  \param td Topic data
// */
//void Publisher_OnTopicUpdate(PublisherTaskPtr t, TopicData * td);



#endif /* SRC_MODULES_PUBLISHER_H_ */
