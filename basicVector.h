#pragma once

#include <Arduino.h>

// #define DEBUG

#ifdef DEBUG
#define CONSOLE_LOG(s)  Serial.print(s)
#define CONSOLE_LOG_LN(s)  Serial.println(s)
#else
#define CONSOLE_LOG(s) 
#define CONSOLE_LOG_LN(s)
#endif

// Minimal class to replace std::vector
template<typename Data>
class Vector {

    size_t _d_size; // Stores no. of actually stored objects
    size_t _d_capacity; // Stores allocated capacity
    Data ** _d_data; // Array of pointers to Data

	const int _id; // Unique ID for this vector	

public:
    Vector() : _d_size(0), _d_capacity(0), _d_data(0), _id((int)&_d_size) {
		
		CONSOLE_LOG(F("Vector: Constructor with ID "));
		CONSOLE_LOG_LN(_id);

	}; // Default constructor

    Vector(Vector const &other) : _d_size(other._d_size), _d_capacity(other._d_capacity), _d_data(0), _id((int)&_d_size)
    {	
		CONSOLE_LOG(F("Vector: Copy start with ID "));
		CONSOLE_LOG_LN(_id);

        // Assign enough space to store the pointers
		_d_data = (Data **)malloc(_d_capacity*sizeof(Data*));

		// Copy each object
		for (int i = 0; i < _d_size; i++) {
			Data * newObj = new Data(*other._d_data[i]);
			_d_data[i] = newObj;
		}

		CONSOLE_LOG_LN(F("Vector: Copy done"));
    };

    ~Vector()
    {
		CONSOLE_LOG(F("Vector: Destructor with ID "));
		CONSOLE_LOG_LN(_id);

		// Clear the vector
		Clear();

		CONSOLE_LOG(F("Vector with ID "));
		CONSOLE_LOG(_id);
		CONSOLE_LOG_LN(F(" is destroyed"));
    };

	// I can't be bothered to code this, and it never gets used anyway
	Vector &operator=(Vector const &other) = delete;

    void push_back(const Data& x)
    {
		CONSOLE_LOG_LN(F("Vector: push_back"));

		// Explicitly copy the object
		Data * pointerToData = new Data(x);

		// Check there's space
        if (_d_capacity == _d_size)
            resize();  // if full - resize

		// Add object to array
        _d_data[_d_size++] = pointerToData;
    };

    void Clear()
    {
		CONSOLE_LOG_LN(F("Vector::Clearing..."));

		// delete each stored object
		for (int i = 0; i < _d_size; i++) {

			CONSOLE_LOG(F("Vector::Deleting Object "));
			CONSOLE_LOG_LN(i);

			Data * thisObj = _d_data[i];
			delete(thisObj);
		}

		// Free the pointer array
		free(_d_data);

		// Set size and capacity to 0
		_d_size = 0;
		_d_capacity = 0;
    }

    size_t size() const { return _d_size; }; // Size getter

    Data const &operator[](size_t idx) const { return *_d_data[idx]; }; // Const getter

    Data &operator[](size_t idx) { return *_d_data[idx]; }; // Changeable getter

private:
    void resize()
    {
        // New capacity is double old capacity
		_d_capacity = _d_capacity ? _d_capacity * 2 : 1;

		// Allocate new memory for pointers
        Data ** newData = (Data **)malloc(_d_capacity*sizeof(Data*));
		
		// Copy all the old memory over
        memcpy(newData, _d_data, _d_size * sizeof(Data));

		// Free the old pointer array
        free(_d_data);

		// Use the new one
        _d_data = newData;
    };
};