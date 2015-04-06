
#include "../mmf_c/os/mmf.h"

#include "tasks/RcTask.h"
#include "tasks/GpsTask.h"
#include "tasks/CommTask.h"
#include "topics/InputTopics.h"
#include "topics/UartTopics.h"
#include "topics/DataTopics.h"

int main(void) {
	// Creates an exception to handle errors and initializes to default data
	Exception e = Exception_INIT();


	// 1 - Kernel allocation and initialization.
	//     This creates a static task list in the form: 	static Task* mmf_os_tasklist[2];
	//     And invokes to init member in the form:			OS_init(mmf_os_tasklist, 2, &e);
	//     Exception handling should be checked afterwards.

	OS_ALLOC(os_tasklist, 4, 10000, &e);
	catch(&e){
		Exception_clear(&e);
		return -1;
	}

	// 2- [optional] Topics initialization, it can be done in tasks implementation. Nevertheless is done
	//	  here for clarification to ensure that observerlist is not accidentally cleared.

	InputTopic_initialize("/key", &e);
	catch(&e){
		return -1;
	}
	UartTopic_initialize("/nmea", &e);
	catch(&e){
		return -1;
	}
	UartTopic_initialize("/ubx", &e);
	catch(&e){
		return -1;
	}
	UartTopic_initialize("/com", &e);
	catch(&e){
		return -1;
	}
	DataTopic_initialize("/rc", &e);
	catch(&e){
		return -1;
	}
	DataTopic_initialize("/gps", &e);
	catch(&e){
		return -1;
	}


	// 3 - Tasks creation, init callback is invoked internally.
	//     Exception checking should be done afterwards with "catch" macro
	//	   It allocates memory to get up to 5 topic updates pending. The aray is named: mmf_topicpool_TASKNAME

	Task rctask;
	Task_ALLOC_TOPIC_POOL(rctask, 6);
	Task_initialize(&rctask,
					"rctask",
					PRIO_MAX,
					mmf_topicpool_rctask,
					6,
					RcTask_init,
					RcTask_OnYieldTurn,
					RcTask_OnResume,
					RcTask_OnEventFlag,
					RcTask_OnTopicUpdate,
					&rctask,
					&e);
	catch(&e){
		Exception_clear(&e);
		return -1;
	}

	Task gpstask;
	Task_ALLOC_TOPIC_POOL(gpstask, 2);
	Task_initialize(&gpstask,
					"gpstask",
					PRIO_MAX+1,
					mmf_topicpool_gpstask,
					2,
					GpsTask_init,
					GpsTask_OnYieldTurn,
					GpsTask_OnResume,
					GpsTask_OnEventFlag,
					GpsTask_OnTopicUpdate,
					&gpstask,
					&e);
	catch(&e){
		Exception_clear(&e);
		return -1;
	}

	Task commtask;
	Task_ALLOC_TOPIC_POOL(commtask, 6);
	Task_initialize(&commtask,
					"commtask",
					PRIO_MAX+2,
					mmf_topicpool_commtask,
					6,
					CommTask_init,
					CommTask_OnYieldTurn,
					CommTask_OnResume,
					CommTask_OnEventFlag,
					CommTask_OnTopicUpdate,
					&commtask,
					&e);
	catch(&e){
		Exception_clear(&e);
		return -1;
	}

	Task systask;
	Task_initialize(&systask,
					"systask",
					PRIO_MAX+3,
					0,
					0,
					SysTask_init,
					SysTask_OnYieldTurn,
					SysTask_OnResume,
					SysTask_OnEventFlag,
					SysTask_OnTopicUpdate,
					&systask,
					&e);
	catch(&e){
		Exception_clear(&e);
		return -1;
	}

	// 4 - Kernel scheduling. Runs forever, else call OS_schedule_once
	//	   Exception handling should be done if scheduling finish

	OS_schedule(&e);
	catch(&e){
		Exception_clear(&e);
	}

	return 0;
}


