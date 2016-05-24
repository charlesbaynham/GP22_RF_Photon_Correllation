#pragma once

#include <Arduino.h>

// Minimal class to replace std::list

#define LIST_DEBUG

#ifdef LIST_DEBUG
#define CONSOLE_LOG(s)  Serial.print(s)
#define CONSOLE_LOG_LN(s)  Serial.println(s)
#else
#define CONSOLE_LOG(s) 
#define CONSOLE_LOG_LN(s)
#endif

template<typename Data>
class List {

protected:
	// Base class for iterators to iterate over this list
	class Iterator_base;

public:
	// Class for a non-constant iterator
	class Iterator;
	// Class for a constant iterator, guaranteeing no changes to the list
	class Iterator_const;

protected:
	// Class for items in this list
	class ListItem;

	size_t _list_size; // Stores no. of actually stored objects

	// Pointers to first and last objects
	ListItem *_first;
	ListItem *_last;

#ifdef LIST_DEBUG
	// An ID for this list for debugging purposes
	int _ID;
	static int _nextID;
#endif

public:
	// Default constructor
	List() : _list_size(0), _first(NULL), _last(NULL)
#ifdef LIST_DEBUG
		, _ID(_nextID++)
#endif
	{
#ifdef LIST_DEBUG
		if (Serial) {
			CONSOLE_LOG("*** List created with ID: ");
			CONSOLE_LOG_LN(_ID);
		}
#endif
	};

	// Copy constuctor. This makes a copy of each element in the list. 
	// If the list is large, this takes up lots of memory. To avoid this being called, use the const functions and const iterator
	List(List const& other) = delete;

	// Destructor: iterate through list and delete each ListItem
	~List() {
		
		if (Serial) {
			CONSOLE_LOG(F("*** ~List called on list with ID: "));
			CONSOLE_LOG_LN(_ID);
		}

		ListItem * currentItem = _first;

		while (currentItem != NULL) {
			ListItem * nextItem = currentItem->next();
			delete currentItem;
			currentItem = nextItem;
		}
	}


	void push_front(Data item) {
		// Get pointer to current first item
		ListItem *previousFirst = _first;

		CONSOLE_LOG(F("push_front: Pointer to current first item is: "));
#ifdef LIST_DEBUG
		Serial.println((uint32_t)previousFirst, HEX);
#endif

		// Make new ListItem
		ListItem *newFirst = new ListItem (item, NULL, previousFirst);

		// Point old front item at the new one
		if (NULL != previousFirst) {
			previousFirst->setPrevPointer(newFirst);
		} else {
			// If there was no other element, this is the only one so it's also the last
			_last = newFirst;
		}

		// Save pointer
		_first = newFirst;

		// Increment size
		_list_size++;

#ifdef LIST_DEBUG
		debug_front_back("push_front");
#endif
   	}

   	void pop_front() {

		// Get pointer to second item
		ListItem *second = _first->next();

		// Free first item
		delete _first;

		// If there's a second element, update its pointers
		if (NULL != second) {
			second->setPrevPointer(NULL);
		} // If there was no other element, this is the only one so it's also the last
		else {
			_last = NULL;
		}
		
		_first = second;

		// Update count
		_list_size--;

#ifdef LIST_DEBUG
		debug_front_back("pop_front");
#endif
   	}

   	void push_back(Data item) {
		// Get pointer to current last item
		ListItem *previousLast = _last;

		// Make new ListItem
		ListItem *newLast = new ListItem (item, previousLast, NULL);

		// Point old back item at the new one
		if (NULL != previousLast) {
			previousLast->setNextPointer(newLast);
		} else {
			// If there was no other element, this is the only one so it's also the first
			_first = newLast;
		}

		// Save pointer
		_last = newLast;

		// Increment size
		_list_size++;

#ifdef LIST_DEBUG
		debug_front_back("push_back");
#endif
   	}

   	void pop_back() {

		// Get pointer to penultimate item
		ListItem *penult = _last->prev();

		// Free Last item
		delete _last;

		// If there's a penultimate element, update its pointers
		if (NULL != penult) {
			penult->setNextPointer(NULL);
		} // If there was no other element, this is the only one so it's also the first
		else {
			_first = NULL;
		}

		_last = penult;

		// Update count
		_list_size--;

#ifdef LIST_DEBUG
		debug_front_back("pop_back");
#endif
   	}

   	Data front() {
   		return _first->getData();
   	}
	Data back() {
   		return _last->getData();
   	}

	Iterator begin() {
		// Return the first iterator
		return Iterator(_first);
	}

	Iterator_const begin() const {
		// Return the first iterator
		return Iterator_const(_first);
	}

	Iterator end() {
		// Return a "past the end" iterator
		return Iterator(true, _last);
	}

	Iterator_const end() const{
		// Return a "past the end" iterator
		return Iterator_const(true, _last);
	}

   	bool isEmpty() { return _list_size == 0; }

   	size_t size() { return _list_size; }

   	void debug() const {
   		ListItem *p = _first;

   		while (NULL != p) {
			Serial.print("'");
			Serial.print(p->getConstData());
			Serial.print("', ");
   			p = p->next();
   		}
		Serial.println();
   	}

	void debug_front_back(const char * ident) const {
#ifdef LIST_DEBUG
		Serial.print(F("List (id="));
		Serial.print(_ID);
		Serial.print(F("): "));
#else
		Serial.print(F("List: "));
#endif
		Serial.print(ident);
		Serial.print(F(" - first: 0x"));
		Serial.print((uint32_t)_first, HEX);
		Serial.print(", ");
		Serial.print(F(": last: 0x"));
		Serial.print((uint32_t)_last, HEX);
		Serial.print(", ");
		Serial.print(F(": length: "));
		Serial.println(_list_size);
	}

	

};

template<typename Data>
class List<Data>::ListItem {

	Data _d;
	ListItem *_nextItem;
	ListItem *_prevItem;

	public:

		ListItem(Data d, ListItem *prev, ListItem *next) :
			_d(d), _nextItem(next), _prevItem(prev) {}

		Data& getData() {

#ifdef LIST_DEBUG
			CONSOLE_LOG(F("ListItem: returning non-const reference to data in location: 0x"));
			Serial.println((uint32_t) &_d, HEX);
#endif
			
			return _d;
		}

		const Data& getConstData() const {

#ifdef LIST_DEBUG
			CONSOLE_LOG(F("ListItem: returning const reference to data in location: 0x"));
			Serial.println((uint32_t) &_d, HEX);
#endif

			return _d;
		}

		void setPrevPointer(ListItem * newPrev) { _prevItem = newPrev; }
		void setNextPointer(ListItem * newNext) { _nextItem = newNext; }
		
		ListItem *next() { return _nextItem; }
		ListItem *prev() { return _prevItem; }

};

template<typename Data>
class List<Data>::Iterator_base {

protected:

	ListItem *_prevItem, *_nextItem, *_currentItem;
	bool _isPastTheEnd;

	// Reset the microprocessor in case of emergency segfault
	void(*resetFunc) (void) = 0;

public:

	// Normal constructor
	Iterator_base(ListItem * startingItem) :
		_currentItem(startingItem),
		_prevItem(startingItem->prev()),
		_nextItem(startingItem->next()), 
		_isPastTheEnd(false) 
	{}

	// Constuctor for "past the end" iterators
	Iterator_base(bool pastTheEnd, ListItem * lastItem) :
		_currentItem(NULL),
		_prevItem(lastItem),
		_nextItem(NULL),
		_isPastTheEnd(true)
	{}
	
	// Postfix increment
	Iterator_base operator++(int) {
		
		// If there's a next element, iterate to it
		if (NULL != _nextItem) {

			_prevItem = _currentItem;
			_currentItem = _nextItem;
			_nextItem = _nextItem->next();

		}
		else {
			// If not, we've reached the end, so become a past the end iterator
			_prevItem = _prevItem->next();
			_currentItem = NULL;
			_nextItem = NULL;

			_isPastTheEnd = true;
		}

		return *this;
	}

	// Postfix decrement
	Iterator_base operator--(int) {
		
		// If there's a previous element, iterate to it
		if (NULL != _prevItem) {
			
			_nextItem = _currentItem;
			_currentItem = _prevItem;
			_prevItem = _prevItem->prev();

			_isPastTheEnd = false;
		}
		else {
			// If not, we've reached the front so this behaviour is undefined! 
			_prevItem = NULL;
			_currentItem = NULL;
			_nextItem = _currentItem;
		}

		return *this;
	}

	// Check equality
	bool operator==(const Iterator_base a) const {
		
		if (_isPastTheEnd && a._isPastTheEnd) {
			return _prevItem == a._prevItem;
		}

		return a._currentItem == _currentItem;
	}

	// Check inequality
	bool operator!=(const Iterator_base a) const { return !(a == *this); }

};

template<typename Data>
class List<Data>::Iterator : public List<Data>::Iterator_base {

public:
	// Normal constructor
	Iterator(ListItem * startingItem) :
		Iterator_base(startingItem) {}

	// Constuctor for "past the end" iterators
	Iterator(bool pastTheEnd, ListItem * lastItem) :
		Iterator_base(pastTheEnd, lastItem) {}

	// Return the current item
	Data& operator*() {
		if (NULL != this->_currentItem) {
			CONSOLE_LOG_LN(F("List: Returning current object (non-const)"));
			return this->_currentItem->getData();
		}
		else
		{
			// Error! This should never happen but, if it does, reset the microprocessor
			CONSOLE_LOG_LN("Error! Dereferenced a non-existant iterator");
			this->resetFunc();
		}
	}

	// Cast to const iterator
	operator Iterator_const() const {
		
		CONSOLE_LOG_LN(F("Iterator: casting Iterator to Iterator_Const..."));

		if (this->_isPastTheEnd) {
			return Iterator_const(true, this->_prevItem);
		}
		else {
			return Iterator_const(this->_currentItem);
		}
	}

};
	
	
template<typename Data>
class List<Data>::Iterator_const : public List<Data>::Iterator_base {

public:
	// Normal constructor
	Iterator_const(ListItem * startingItem) :
		Iterator_base(startingItem) {}

	// Constuctor for "past the end" iterators
	Iterator_const(bool pastTheEnd, ListItem * lastItem) :
		Iterator_base(pastTheEnd, lastItem) {}
	
	// Return a const reference to the current item
	const Data& operator*() const {
		if (NULL != this->_currentItem) {
			CONSOLE_LOG_LN(F("List: Returning const reference to ListItem's Data"));
			return this->_currentItem->getConstData();
		}
		else
		{
			// Error! This should never happen but, if it does, reset the microprocessor
			CONSOLE_LOG_LN("Error! Dereferenced a non-existant iterator");
			this->resetFunc();
		}
	}

	// Explicitly disallow typecast to non-const Iterator
	operator Iterator() = delete;
};

// Start the List ID off at 0
template<typename Data>
int List<Data>::_nextID = 0;
