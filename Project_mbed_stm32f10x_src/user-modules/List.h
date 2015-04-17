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

//------------------------------------------------------------------------------------
//-- TYPEDEFS ------------------------------------------------------------------------
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//-- CLASS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------

/** \class List
 *  \brief List management class
 */

template<typename T>
class List {
public:

	enum Errors{NULL_POINTER, LIMIT_EXCEEDED, OUT_OF_MEMORY, ITEM_NOT_FOUND};
	typedef int ListException;

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
	 * \param numItems Max number of allowed items in list
	 * \throw error code
	 */
	void setLimit(uint32_t numItems) throw (ListException);

	/**
	 * Add an item to the list
	 * \param item Item to add
	 * \throw error code
	 */
	void addItem(T* item) throw (ListException);

	/**
	 * Removes an item
	 * \param item Item to remove
	 * \throw error code
	 */
	void removeItem(T* item) throw (ListException);

	/**
	 * Get item count
	 * \return item count
	 */
	uint32_t getItemCount();

	/**
	 * Get first item in list
	 * \return Pointer to the first item or NULL if empty
	 */
	T* getFirstItem();

	/**
	 * Get next item in list
	 * \return Pointer to the next item or NULL if no more
	 */
	T* getNextItem();

	/**
	 * Searches an item into the list
	 * \param item Item to search
	 * \throw error code
	 * \return True if item found, else false
	 */
	bool searchItem(T* item) throw (ListException);

private:

	/** List item structure */
	struct ListItem{
		struct ListItem *prev;		///< pointer to the previous ListItem
		struct ListItem *next;		///< pointer to the next ListItem
		T * item;		//!< item
	};

	ListItem *_first;		///< pointer to the first ListItem
	ListItem *_last;		///< pointer to the last ListItem
	ListItem *_search; 		///< auxiliary pointer in search operations
	uint32_t  _count;		///< items count
	uint32_t  _numItems;	///< max number of allowed items on list


	/**
	 * Removes an item
	 * \param item Item to remove
	 * \throw error code
	 */
	void removeItem(ListItem * listitem);

};

#include "List_tpp.h"

#endif
