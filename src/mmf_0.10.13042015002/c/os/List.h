/*
 * List.h
 *
 *  Created on:	09/04/2015
 *  Author: 	raulMrello
 *
 *  List module, manages double-linked lists.
 */

#ifndef __LIST_H
#define __LIST_H

#ifdef __cplusplus
 extern "C" {
#endif


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include "Memory.h"
#include "Exception.h"
#include "../port/platforms.h"	///< required for critical sections management

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

 /** Pointer to Timer object */
typedef struct List_t *	ListPtr;

/** Void Pointer to reference an Item of any type */
typedef void *	Item;

//------------------------------------------------------------------------------------
//-- PROTOTYPES ----------------------------------------------------------------------
//------------------------------------------------------------------------------------

/**
 * Creates an empty list
 * @param e Exception handler
 * @return Pointer to the allocated object
 */
ListPtr List_create(ExceptionPtr e);

/**
 * Deallocates a list. The variable that points to the List objects then is set to 0, to
 * indicate that List is deallocated and now it points to NULL.
 * @param plist	Variable that points to a list object
 * @param e Exception handler
 */
void List_kill(ListPtr* plist, ExceptionPtr e);

/**
 * Set a limit of list items
 * @param plist	Variable that points to a list object
 * @param numItems Max number of allowed items in list
 * @param e Exception handler
 */
void List_setLimit(ListPtr *plist, uint32_t numItems, ExceptionPtr e);

/**
 * Add an item to the list
 * @param list List object
 * @param item Item to add
 * @param e Exception handler
 */
void List_addItem(ListPtr list, Item item, ExceptionPtr e);

/**
 * Removes an item
 * @param list List
 * @param item Item to remove
 * @param e Exception handler
 */
void List_removeItem(ListPtr list, Item item, ExceptionPtr e);

/**
 * Get item count
 * @param list List
 * @param e Exception handler
 * @return Item count
 */
uint32_t List_getItemCount(ListPtr list, ExceptionPtr e);

/**
 * Get first item in list
 * @param list List
 * @param e Exception handler
 * @return Pointer to the first item
 */
Item List_getFirstItem(ListPtr list, ExceptionPtr e);

/**
 * Get next item in list
 * @param list List
 * @param e Exception handler
 * @return Pointer to the next item
 */
Item List_getNextItem(ListPtr list, ExceptionPtr e);

/**
 * Searchs an item into the list
 * @param list List object
 * @param item Item to search
 * @param e Exception handler
 * @return Pointer to the item found
 */
Item List_searchItem(ListPtr list, Item item, ExceptionPtr e);

#ifdef __cplusplus
}
#endif


#endif
