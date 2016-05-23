#pragma once

#include <Arduino.h>

// Minimal class to replace std::list

// #define DEBUG

#ifdef DEBUG
#define CONSOLE_LOG(s)  Serial.print(s)
#define CONSOLE_LOG_LN(s)  Serial.println(s)
#else
#define CONSOLE_LOG(s) 
#define CONSOLE_LOG_LN(s)
#endif

template<typename Data>
class listItem;

template<typename Data>
class List {

    size_t _list_size; // Stores no. of actually stored objects
    
    // Pointers to first and last objects
    listItem<Data> *_first;
    listItem<Data> *_last;
public:
    List() : _list_size(0), _first(NULL), _last(NULL) {}; // Default constructor

	void push_front(Data item) {
		// Get pointer to current first item
		listItem<Data> *previousFirst = _first;

		CONSOLE_LOG(F("push_front: Pointer to current first item is: "));
#ifdef DEBUG
		Serial.println((uint32_t)previousFirst, HEX);
#endif

		// Make new listItem
		listItem<Data> *newFirst = new listItem<Data> (item, NULL, previousFirst);

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

#ifdef DEBUG
		debug_front_back("push_front");
#endif
   	}

   	void pop_front() {

		// Get pointer to second item
		listItem<Data> *second = _first->next();

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

#ifdef DEBUG
		debug_front_back("pop_front");
#endif
   	}

   	void push_back(Data item) {
		// Get pointer to current last item
		listItem<Data> *previousLast = _last;

		// Make new listItem
		listItem<Data> *newLast = new listItem<Data> (item, previousLast, NULL);

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

#ifdef DEBUG
		debug_front_back("push_back");
#endif
   	}

   	void pop_back() {

		// Get pointer to penultimate item
		listItem<Data> *penult = _last->prev();

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

#ifdef DEBUG
		debug_front_back("pop_back");
#endif
   	}

   	Data front() {
   		return _first->getData();
   	}
	Data back() {
   		return _last->getData();
   	}

   	bool isEmpty() { return _list_size == 0; }

   	size_t size() { return _list_size; }

   	void debug() {
   		listItem<Data> *p = _first;

   		while (NULL != p) {
			Serial.print("'");
			Serial.print(p->getData());
			Serial.print("', ");
   			p = p->next();
   		}
		Serial.println();
   	}

	void debug_front_back(const char * ident) {
		Serial.print(ident); 
		Serial.print(F(": first: 0x"));
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
class listItem {

	friend class List<Data>;

	Data _d;
	listItem<Data> *_nextItem;
	listItem<Data> *_prevItem;

	public:

		listItem(Data d, listItem *prev, listItem *next) :
		_d(d), _nextItem(next), _prevItem(prev) {}

		Data getData() { return _d; }

	private:

		void setPrevPointer(listItem * newPrev) { _prevItem = newPrev; }
		void setNextPointer(listItem * newNext) { _nextItem = newNext; }
		
		listItem *next() { return _nextItem; }
		listItem *prev() { return _prevItem; }

};
