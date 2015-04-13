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


//------------------------------------------------------------------------------------
//-- DEPENDENCIES --------------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <stdint.h>
#include "Exception.h"

namespace MMF {

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** Void Pointer to reference an Item of any type */
typedef void *	Item;

//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \class List
 *  \brief List management class
 */
class List {
public:
	/**
	 * Creates an empty list
	 */
	List();

	/**
	 * Deallocates a list
	 */
	~List();

	/**
	 * Set a limit of list items
	 * @param numItems Max number of allowed items in list
	 * @throw Exception
	 */
	void setLimit(uint32_t numItems) throw (Exception);

	/**
	 * Add an item to the list
	 * @param item Item to add
	 * @throw Exception
	 */
	void addItem(Item item) throw (Exception);

	/**
	 * Removes an item
	 * @param item Item to remove
	 * @throw Exception
	 */
	void removeItem(Item item) throw (Exception);

	/**
	 * Get item count
	 */
	uint32_t getItemCount();

	/**
	 * Get first item in list
	 * @return Pointer to the first item or NULL if empty
	 */
	Item getFirstItem();

	/**
	 * Get next item in list
	 * @return Pointer to the next item or NULL if no more
	 */
	Item getNextItem();

	/**
	 * Searchs an item into the list
	 * @param item Item to search
	 * @throw Exception
	 * @return Pointer to the item found
	 */
	Item searchItem(Item item) throw (Exception);

private:

	/** List item structure */
	typedef struct _ListItem_t{
		struct _ListItem_t *prev;		///< pointer to the previous ListItem
		struct _ListItem_t *next;		///< pointer to the next ListItem
		Item	 		    item;		//!< item
	}ListItem;

	ListItem *_first;		///< pointer to the first ListItem
	ListItem *_last;		///< pointer to the last ListItem
	ListItem *_search; 		///< auxiliary pointer in search operations
	uint32_t  _count;		///< items count
	uint32_t  _numItems;	///< max number of allowed items on list

	void removeItem(ListItem *listitem) throw (Exception);
};
}


#endif
