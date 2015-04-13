/*
 * List.cpp
 *
 *  Created on:	09/04/2015
 *  Author: 	raulMrello
 *
 *  List module, manages double-linked lists.
 */

#include "List.h"
#include "Memory.h"
#include "../port/platforms.h"	///< required for critical sections management
using namespace MMF;

#if !defined(UINT32_MAX)
#define UINT32_MAX 	0xffffffff  /* 4294967295U */
#endif

//------------------------------------------------------------------------------------
//-- PRIVATE FUNCTIONS ---------------------------------------------------------------
//------------------------------------------------------------------------------------

void List::removeItem(ListItem *listitem) throw (Exception){
	// if is first will point to the next
	if(!listitem->prev){
		// if next is null then list is empty
		if(!listitem->next){
			_first = 0;
			_last = 0;
			_count = 0;
			_search = 0;
			Memory::free(listitem);
		}
		_first = listitem->next;
		listitem->next->prev = 0;
		_count--;
		 Memory::free(listitem);
	}
	// else, adjust pointers
	listitem->prev->next = listitem->next;
	if(listitem->next)
		listitem->next->prev = listitem->prev;
	else
		_last = listitem->prev;
	_count--;
	Memory::free(listitem);
}

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

List::List(){
	PLATFORM_ENTER_CRITICAL();
	_count = 0;
	_first = 0;
	_last = 0;
	_search = 0;
	_numItems = UINT32_MAX;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
List::~List(){
	// remove all items
	while(_first){
		removeItem(_last);
	}
}

//------------------------------------------------------------------------------------
void List::setLimit(uint32_t numItems) throw (Exception){
	if(!numItems){
		throw Exception(Exception::BAD_ARGUMENT, "List::setLimit list or numItems are null");
	}
	PLATFORM_ENTER_CRITICAL();
	_numItems = numItems;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void List::addItem(Item item) throw (Exception){
	if(!item){
		throw Exception(Exception::BAD_ARGUMENT, "List::addItem list or item are null");
	}
	if(_count >= _numItems){
		throw Exception(Exception::MEMORY_ALLOC, "List_addItem no more items allowed");
	}
	ListItem* listitem;
	PLATFORM_ENTER_CRITICAL();
	try{
		listitem = (ListItem*)Memory::alloc(sizeof(ListItem));
	}
	catch(Exception &e){
		PLATFORM_EXIT_CRITICAL();
		throw e;
	}
	listitem->item = item;
	// if first element, initializes pointers
	if(!_count){
		_first = listitem;
		_last = listitem;
		listitem->prev = 0;
		listitem->next = 0;
	}
	// else at to the end
	else{
		ListItem* last = _last;
		last->next = listitem;
		listitem->prev = last;
		listitem->next = 0;
		_last = listitem;
	}
	_count++;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void List::removeItem(Item item) throw (Exception){
	if(!item){
		throw Exception(Exception::BAD_ARGUMENT, "List::removeItem list or item are null");
	}
	PLATFORM_ENTER_CRITICAL();
	ListItem* listitem;
	// search the item in all elements
	listitem = _first;
	do{
		if(listitem->item == item){
			try{
				removeItem(listitem);
				PLATFORM_EXIT_CRITICAL();
				return;
			}
			catch(Exception &e){
				PLATFORM_EXIT_CRITICAL();
				throw e;
			}
		}
		listitem = listitem->next;
	}while(listitem);
	PLATFORM_EXIT_CRITICAL();
	throw Exception(Exception::UNKNOWN_EXCEPTION, "List::removeItem item not found");
}

//------------------------------------------------------------------------------------
uint32_t List::getItemCount(){
	return _count;
}

//------------------------------------------------------------------------------------
Item List::getFirstItem(){
	PLATFORM_ENTER_CRITICAL();
	_search = _first;
	if(!_search){
		PLATFORM_EXIT_CRITICAL();
		return 0;
	}
	PLATFORM_EXIT_CRITICAL();
	return _search->item;

}

//------------------------------------------------------------------------------------
Item List::getNextItem(){
	PLATFORM_ENTER_CRITICAL();
	if(!_search){
		PLATFORM_EXIT_CRITICAL();
		return 0;
	}
	_search = _search->next;
	if(!_search){
		PLATFORM_EXIT_CRITICAL();
		return 0;
	}
	PLATFORM_EXIT_CRITICAL();
	return _search->item;
}

//------------------------------------------------------------------------------------
Item List::searchItem(Item item) throw (Exception){
	if(!item){
		throw Exception(Exception::BAD_ARGUMENT, "List::searchItem item are null");
	}
	PLATFORM_ENTER_CRITICAL();
	Item i = getFirstItem();
	if(!i){
		PLATFORM_EXIT_CRITICAL();
		return 0;
	}
	for(;;){
		if(i == item){
			PLATFORM_EXIT_CRITICAL();
			return i;
		}
		i = getNextItem();
		if(!i){
			break;
		}
	}
	PLATFORM_EXIT_CRITICAL();
	return 0;
}

