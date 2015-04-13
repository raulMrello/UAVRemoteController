/*
 * List.c
 *
 *  Created on:	09/04/2015
 *  Author: 	raulMrello
 *
 *  List module, manages double-linked lists.
 */

#include "List.h"

//------------------------------------------------------------------------------------
//-- PRIVATE TYPEDEFS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

/** List item structure */
typedef struct _ListItem_t{
	struct _ListItem_t *prev;		///< pointer to the previous ListItem
	struct _ListItem_t *next;		///< pointer to the next ListItem
	Item	 		    item;		//!< item
}ListItem;

/** List structure */
typedef struct {
	ListItem *first;	///< pointer to the first ListItem
	ListItem *last;		///< pointer to the last ListItem
	ListItem *search; 	///< auxiliary pointer in search operations
	uint32_t  count;	///< items count
	uint32_t  numItems;	///< max number of allowed items on list
}List;

//------------------------------------------------------------------------------------
//-- PRIVATE PROTOTYPES --------------------------------------------------------------
//------------------------------------------------------------------------------------

/**
 * Removes an item from the list
 * @param list List
 * @param listitem ListItem to remove
 * @param e Exception handler
 */
static void removeItem(List *list, ListItem *listitem, ExceptionPtr e);

//------------------------------------------------------------------------------------
//-- PRIVATE FUNCTIONS ---------------------------------------------------------------
//------------------------------------------------------------------------------------

static void removeItem(List *list, ListItem *listitem, ExceptionPtr e){
	// if is first will point to the next
	if(!listitem->prev){
		// if next is null then list is empty
		if(!listitem->next){
			list->first = 0;
			list->last = 0;
			list->count = 0;
			list->search = 0;
			Memory_free(listitem, e);
			catch(e){
				return;
			}
		}
		list->first = listitem->next;
		listitem->next->prev = 0;
		list->count--;
		 Memory_free(listitem, e);
		 catch(e){
			 return;
		 }
	}
	// else, adjust pointers
	listitem->prev->next = listitem->next;
	if(listitem->next)
		listitem->next->prev = listitem->prev;
	else
		list->last = listitem->prev;
	list->count--;
	Memory_free(listitem, e);
}

//------------------------------------------------------------------------------------
//-- PUBLIC FUNCTIONS ----------------------------------------------------------------
//------------------------------------------------------------------------------------

ListPtr List_create(ExceptionPtr e){
	PLATFORM_ENTER_CRITICAL();
	List* this = (List*)Memory_alloc(sizeof(List), e);
	catch(e){
		PLATFORM_EXIT_CRITICAL();
		return 0;
	}
	this->count = 0;
	this->first = 0;
	this->last = 0;
	this->search = 0;
	this->numItems = UINT32_MAX;
	PLATFORM_EXIT_CRITICAL();
	return (ListPtr)this;
}

//------------------------------------------------------------------------------------
void List_kill(ListPtr* plist, ExceptionPtr e){
	List** pthis = (List**)plist;
	if(!plist || !pthis){
		Exception_throw(e, BAD_ARGUMENT, "List_kill plist is null");
		return;
	}
	PLATFORM_ENTER_CRITICAL();
	List* this;
	this = *pthis;
	if(!this){
		PLATFORM_EXIT_CRITICAL();
		Exception_throw(e, BAD_ARGUMENT, "List_kill *plist is null");
		return;
	}
	// remove all items
	while(this->first){
		removeItem(this, this->last, e);
		catch(e){
			PLATFORM_EXIT_CRITICAL();
			return;
		}
	}
	Memory_free(*pthis, e);
	catch(e){
		PLATFORM_EXIT_CRITICAL();
		return;
	}
	*pthis = 0;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void List_setLimit(ListPtr *plist, uint32_t numItems, ExceptionPtr e){
	List* this = (List*)plist;
	if(!this || !numItems){
		Exception_throw(e, BAD_ARGUMENT, "List_addItem list or numItems are null");
		return;
	}
	PLATFORM_ENTER_CRITICAL();
	this->numItems = numItems;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void List_addItem(ListPtr list, Item item, ExceptionPtr e){
	List* this = (List*)list;
	if(!this || !item){
		Exception_throw(e, BAD_ARGUMENT, "List_addItem list or item are null");
		return;
	}
	if(this->count >= this->numItems){
		Exception_throw(e, MEMORY_ALLOC, "List_addItem no more items allowed");
		return;
	}
	ListItem* listitem;
	PLATFORM_ENTER_CRITICAL();
	listitem = (ListItem*)Memory_alloc(sizeof(ListItem), e);
	catch(e){
		PLATFORM_EXIT_CRITICAL();
		return;
	}
	listitem->item = item;
	// if first element, initializes pointers
	if(!this->count){
		this->first = listitem;
		this->last = listitem;
		listitem->prev = 0;
		listitem->next = 0;
	}
	// else at to the end
	else{
		ListItem* last = this->last;
		last->next = listitem;
		listitem->prev = last;
		listitem->next = 0;
		this->last = listitem;
	}
	this->count++;
	PLATFORM_EXIT_CRITICAL();
}

//------------------------------------------------------------------------------------
void List_removeItem(ListPtr list, Item item, ExceptionPtr e){
	List* this = (List*)list;
	if(!this || !item){
		Exception_throw(e, BAD_ARGUMENT, "List_removeItem list or item are null");
		return ;
	}
	PLATFORM_ENTER_CRITICAL();
	ListItem* listitem;
	// search the item in all elements
	listitem = this->first;
	do{
		if(listitem->item == item){
			removeItem(this, listitem, e);
			PLATFORM_EXIT_CRITICAL();
			return;
		}
		listitem = listitem->next;
	}while(listitem);
	PLATFORM_EXIT_CRITICAL();
	Exception_throw(e, UNKNOWN_EXCEPTION, "List_removeItem item not found");
}

//------------------------------------------------------------------------------------
uint32_t List_getItemCount(ListPtr list, ExceptionPtr e){
	List* this = (List*)list;
	if(!this){
		Exception_throw(e, BAD_ARGUMENT, "List_getItemCount list is null");
		return 0;
	}
	return this->count;
}

//------------------------------------------------------------------------------------
Item List_getFirstItem(ListPtr list, ExceptionPtr e){
	List* this = (List*)list;
	if(!this){
		Exception_throw(e, BAD_ARGUMENT, "List_getFirstItem list is null");
		return 0;
	}
	PLATFORM_ENTER_CRITICAL();
	this->search = this->first;
	if(!this->search){
		PLATFORM_EXIT_CRITICAL();
		Exception_throw(e, UNKNOWN_EXCEPTION, "List_getFirstItem list is empty");
		return 0;
	}
	PLATFORM_EXIT_CRITICAL();
	return this->search->item;

}

//------------------------------------------------------------------------------------
Item List_getNextItem(ListPtr list, ExceptionPtr e){
	List* this = (List*)list;
	if(!this){
		Exception_throw(e, BAD_ARGUMENT, "List_getNextItem list is null");
		return 0;
	}
	PLATFORM_ENTER_CRITICAL();
	if(!this->search){
		PLATFORM_EXIT_CRITICAL();
		Exception_throw(e, UNKNOWN_EXCEPTION, "List_getNextItem no more items");
		return 0;
	}
	this->search = this->search->next;
	if(!this->search){
		PLATFORM_EXIT_CRITICAL();
		return 0;
	}
	PLATFORM_EXIT_CRITICAL();
	return this->search->item;
}

//------------------------------------------------------------------------------------
Item List_searchItem(ListPtr list, Item item, ExceptionPtr e){
	List* this = (List*)list;
	if(!this || !item){
		Exception_throw(e, BAD_ARGUMENT, "List_searchItem list or item are null");
		return 0;
	}
	PLATFORM_ENTER_CRITICAL();
	Item i = List_getFirstItem((ListPtr)this, e);
	catch(e){
		PLATFORM_EXIT_CRITICAL();
		return 0;
	}
	for(;;){
		if(i == item){
			PLATFORM_EXIT_CRITICAL();
			return i;
		}
		i = List_getNextItem((ListPtr)this, e);
		catch(e){
			break;
		}
	}
	PLATFORM_EXIT_CRITICAL();
	return 0;
}

