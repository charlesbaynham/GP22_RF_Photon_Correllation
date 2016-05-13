#pragma once

// Minimal class to replace std::list

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
   	}

   	void pop_front() {

		// Get pointer to second item
		listItem<Data> *second = _first->next();

		// Free first item
		delete _first;

		// Update pointers
		second->setPrevPointer(NULL);
		_first = second;

		// Update count
		_list_size--;
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

		debug();
   	}

   	void pop_back() {

		// Get pointer to penultimate item
		listItem<Data> *penult = _last->prev();

		// Free Last item
		delete _last;

		// Update pointers
		penult->setNextPointer(NULL);
		_last = penult;

		// Update count
		_list_size--;
   	}

   	Data front() {
   		return _first->getData();
   	}
	Data back() {
   		return _last->getData();
   	}

   	bool isEmpty() { return _list_size == 0; }

   	size_t size() { return _list_size; }

   	#ifdef DEBUG
   	void debug() {
   		listItem<Data> *p = _first;

   		while (NULL != p) {
   			Serial.println(p->getData());
   			p = p->next();
   		}
   	}
   	#endif

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
