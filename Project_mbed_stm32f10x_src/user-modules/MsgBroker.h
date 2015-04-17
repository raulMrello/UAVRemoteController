/*
 * MsgBroker.h
 *
 *  Created on: 16/4/2015
 *      Author: dptoid
 */

#ifndef MSGBROKER_H_
#define MSGBROKER_H_

#include "List.h"
#include <string.h>
#include <stdlib.h>


class MsgBroker {
public:
	enum Errors{NULL_POINTER, DEINIT, OUT_OF_MEMORY, TOPIC_EXISTS, TOPIC_NOT_FOUND, SUBSCRIBER_NOT_FOUND, TOPIC_PENDING, TOPIC_SIZE};
	typedef int MsgBrokerException;
	typedef void (*NotifyCallback)(void *subscriber, const char * topicname);

	/** Observer data */
	struct Observer{
		void *observer;
		NotifyCallback notify;
	};

	/** Topic data */
	struct Topic{
		List<Observer >	*observer_list;
		const char 	*name;
		void 		*data;
		uint32_t 	datasize;
		uint32_t    count;
	};

	static void init() {
		if(!MsgBroker::topic_list){
			MsgBroker::topic_list = new List<MsgBroker::Topic>();
		}
	}

	static void deinit() {
		if(MsgBroker::topic_list){
			Topic * topic = topic_list->getFirstItem();
			while(topic){
				Observer * obs = topic->observer_list->getFirstItem();
				while(obs){
					free(obs);
					obs = topic->observer_list->getNextItem();
				}
				delete(topic->observer_list);
				free(topic);
				topic = topic_list->getNextItem();
			}
			delete(MsgBroker::topic_list);
			MsgBroker::topic_list = 0;
		}

	}

	static MsgBroker::Topic * installTopic(const char* name, uint32_t datasize) throw (MsgBrokerException){
		if(!MsgBroker::topic_list){
			throw DEINIT;
		}
		MsgBroker::Topic * topic = findTopic(name);
		if(topic){
			throw TOPIC_EXISTS;
		}
		topic = (MsgBroker::Topic*)malloc(sizeof(MsgBroker::Topic));
		if(!topic){
			throw OUT_OF_MEMORY;
		}
		topic->observer_list = new List<Observer>();
		topic->name = name; topic->datasize = datasize; topic->data = 0; topic->count = 0;
		MsgBroker::topic_list->addItem(topic);
		return topic;
	}

	static void removeTopic(const char * topic_name) throw (MsgBrokerException){
		if(!MsgBroker::topic_list){
			throw DEINIT;
		}
		MsgBroker::Topic * topic = findTopic(topic_name);
		if(!topic){
			throw TOPIC_NOT_FOUND;
		}
		topic_list->removeItem(topic);
		free(topic);
	}

	static void removeTopic(Topic * t) throw (MsgBrokerException){
		if(!MsgBroker::topic_list){
			throw DEINIT;
		}
		MsgBroker::removeTopic(t->name);
	}

	static MsgBroker::Observer * attach (const char * topic_name, void *subscriber, NotifyCallback notify) throw (MsgBrokerException){
		if(!MsgBroker::topic_list){
			throw DEINIT;
		}
		MsgBroker::Topic * topic = findTopic(topic_name);
		if(!topic){
			throw TOPIC_NOT_FOUND;
		}
		MsgBroker::Observer *obs = (MsgBroker::Observer *)malloc(sizeof(MsgBroker::Observer));
		obs->observer = subscriber;
		obs->notify = notify;
		topic->observer_list->addItem(obs);
		return obs;
	}

	static void detach (const char * topic_name, void * subscriber){
		if(!MsgBroker::topic_list){
			throw DEINIT;
		}
		MsgBroker::Topic * topic = findTopic(topic_name);
		if(!topic){
			throw TOPIC_NOT_FOUND;
		}
		MsgBroker::Observer *obs = findObserver(topic->observer_list, subscriber);
		if(!obs){
			throw SUBSCRIBER_NOT_FOUND;
		}

		topic->observer_list->removeItem(obs);
		free(obs);
	}

	static void publish (const char * topic_name, void *data, uint32_t datasize) throw (MsgBrokerException){
		if(!MsgBroker::topic_list){
			throw DEINIT;
		}
		MsgBroker::Topic * topic = findTopic(topic_name);
		if(!topic){
			throw TOPIC_NOT_FOUND;
		}
		if(topic->count > 0){
			throw TOPIC_PENDING;
		}
		if(datasize != topic->datasize){
			throw TOPIC_SIZE;
		}
		topic->data = data;
		topic->datasize = datasize;
		Observer * obs = topic->observer_list->getFirstItem();
		while(obs){
			topic->count++;
			obs->notify(obs->observer, topic->name);
			obs = topic->observer_list->getNextItem();
		}
	}

	static void consumed(const char * topic_name) throw (MsgBrokerException){
		if(!MsgBroker::topic_list){
			throw DEINIT;
		}
		MsgBroker::Topic * topic = findTopic(topic_name);
		if(!topic){
			throw TOPIC_NOT_FOUND;
		}
		if(topic->count){
			topic->count--;
		}
	}


	static void * getTopicData(const char * topic_name){
		if(!MsgBroker::topic_list){
			throw DEINIT;
		}
		MsgBroker::Topic * topic = findTopic(topic_name);
		if(!topic){
			throw TOPIC_NOT_FOUND;
		}
		return topic->data;
	}

	static uint32_t getTopicDatasize(const char * topic_name){
		if(!MsgBroker::topic_list){
			throw DEINIT;
		}
		MsgBroker::Topic * topic = findTopic(topic_name);
		if(!topic){
			throw TOPIC_NOT_FOUND;
		}
		return topic->datasize;
	}

private:
	static List<MsgBroker::Topic> * topic_list;

	static MsgBroker::Topic * findTopic(const char * topic_name){
		if(!MsgBroker::topic_list){
			throw DEINIT;
		}
		MsgBroker::Topic *topic = topic_list->getFirstItem();
		while(topic){
			if(strcmp(topic_name, topic->name) == 0){
				return topic;
			}
			topic = topic_list->getNextItem();
		}
		return 0;
	}

	static MsgBroker::Observer * findObserver(List<MsgBroker::Observer> * list, void *subscriber){
		if(!MsgBroker::topic_list){
			throw DEINIT;
		}
		MsgBroker::Observer *obs = list->getFirstItem();
		while(obs){
			if(subscriber == obs->observer){
				return obs;
			}
			obs = list->getNextItem();
		}
		return 0;
	}


};

#endif /* MSGBROKER_H_ */
