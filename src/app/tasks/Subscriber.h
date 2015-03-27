/*
 * MySubscriber.h
 *
 *  Created on: 12/3/2015
 *      Author: dptoid
 *
 *  Sample class to show how to attend different types of events/topics. Events are uint16_t typed codes, while
 *  topics are complex structures with name, data and datasize. Tasks can wait on different cases at the same time,
 *  so, this class is an example of use.
 *
 *  Example:
 *
 *  1 - It will intialize and will be attached to MyTopic updates and also start waiting for eventcode = 1.
 *  2 - If event_code=1 is attended, then it will wait for event_code=2 or 4.
 *  3 - If event_code=2/4 is attended, then it will wait for the combination of event_code=8&16
 *  4 - Along the event waiting process, if a topic update is done, then task will handle it.
 */

#ifndef SRC_TASKS_SUBSCRIBER_H_
#define SRC_TASKS_SUBSCRIBER_H_

#include "../../../src/mmf/c/os/mmf.h"


/** \def SubscriberTask
 *  \brief Definition to abstract the callback handler into void* type
 */
typedef void* SubscriberTaskPtr;

/** \fn Subscriber_init
 *  \brief Subscriber initialization callback
 *  \param t Subscriber task object
 */
void Subscriber_init(SubscriberTaskPtr t);

/** \fn Subscriber_OnYieldTurn
 *  \brief Subscriber on yield turn events callback
 *  \param t Subscriber task object
 */
void Subscriber_OnYieldTurn(SubscriberTaskPtr t);

/** \fn Subscriber_OnResume
 *  \brief Subscriber on resume events callback
 *  \param t Subscriber task object
 */
void Subscriber_OnResume(SubscriberTaskPtr t);

/** \fn Subscriber_OnEventFlag
 *  \brief Subscriber on event flags callback
 *  \param t Subscriber task object
 *  \param event Event raised
 */
void Subscriber_OnEventFlag(SubscriberTaskPtr t, int event);

/** \fn Subscriber_OnTopicUpdate
 *  \brief Subscriber on topic update events callback
 *  \param t Subscriber task object
 *  \param td Topic data
 */
void Subscriber_OnTopicUpdate(SubscriberTaskPtr t, TopicData * td);

#endif /* SRC_TASKS_SUBSCRIBER_H_ */
