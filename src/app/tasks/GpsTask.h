/*
 * GpsTask.h
 *
 *  Created on: 13/3/2015
 *      Author: raulMrello
 *
 *  GpsTask handles /nmea and or /ubxs topic updates.It analyzes the type of stream and
 *  extract location data from them. Then checks if location has changed and calculates
 *  its new heading. Once calculated, publish a /gps topic update
 */

#ifndef SRC_MODULES_GPSTASK_H_
#define SRC_MODULES_GPSTASK_H_

#include "../../mmf_c/os/mmf.h"

/** \def GpsTaskPtr
 *  \brief Definition to abstract the callback handler into void* type
 */
typedef void* GpsTaskPtr;

/** \fn GpsTask_init
 *  \brief GpsTask initialization callback
 *  \param t GpsTask task object
 */
void GpsTask_init(GpsTaskPtr t);

/** \fn GpsTask_OnYieldTurn
 *  \brief GpsTask on yield turn events callback
 *  \param t GpsTask task object
 */
void GpsTask_OnYieldTurn(GpsTaskPtr t);

/** \fn GpsTask_OnResume
 *  \brief GpsTask on resume events callback
 *  \param t GpsTask task object
 */
void GpsTask_OnResume(GpsTaskPtr t);

/** \fn GpsTask_OnEventFlag
 *  \brief GpsTask on event flags callback
 *  \param t GpsTask task object
 *  \param event Event raised
 */
void GpsTask_OnEventFlag(GpsTaskPtr t, int event);

/** \fn GpsTask_OnTopicUpdate
 *  \brief GpsTask on topic update events callback
 *  \param t GpsTask task object
 *  \param td Topic data
 */
void GpsTask_OnTopicUpdate(GpsTaskPtr t, TopicData * td);



#endif /* SRC_MODULES_GPSTASK_H_ */
