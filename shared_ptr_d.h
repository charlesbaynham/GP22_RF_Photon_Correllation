#pragma once

#include "Microprocessor_Debugging\debugging_disable.h"

template <typename T>
class shared_ptr_d
{

	T* _ptr;
	int* _count;

public:

	// Make an empty smart pointer
	shared_ptr_d() :
		_ptr(0), _count(0)
	{
		outputDebugHeader();
		CONSOLE_LOG_LN(F("Null constuctor"));
	}

	// Make a smart pointer and give it a pointer to look after
	explicit shared_ptr_d(T* ptr) : shared_ptr_d()
	{
		outputDebugHeader();
		CONSOLE_LOG_LN(F("Ptr constuctor"));
		reset(ptr);
	}

	// Copy the given pointer
	shared_ptr_d(const shared_ptr_d& rhs) :
		shared_ptr_d(0)
	{
		outputDebugHeader();
		CONSOLE_LOG_LN(F("Copy constuctor."));

		reset(rhs);
	}


	// Destroy the smart pointer. This will also destroy the managed object if there is one
	// and this is the last instance of this shared_ptr_d
	~shared_ptr_d() {
		outputDebugHeader();
		CONSOLE_LOG_LN(F("Destructor"));
		clear();
	}

	// Checks if a pointer is assigned
	operator bool() const {
		if (_ptr)
			return true;

		return false;
	}

	// Gets the pointer
	T* get() {
		return _ptr;
	}

	// Gets the pointer - const
	const T* get() const {
		return _ptr;
	}

	// Dereferences to the held object
	T& operator * () {
		outputDebugHeader();
		CONSOLE_LOG(F("Deref to "));
		CONSOLE_LOG_LN((int)_ptr);
		return *_ptr;
	}

	// Dereferences to the held object - const
	const T& operator * () const {
		outputDebugHeader();
		CONSOLE_LOG(F("const deref to "));
		CONSOLE_LOG_LN((int)_ptr);

		return *_ptr;
	}

	// Dereferences to the held object
	T* operator -> () {
		outputDebugHeader();
		CONSOLE_LOG(F("Pointer deref to "));
		CONSOLE_LOG_LN((int)_ptr);
		return _ptr;
	}

	// Sets the pointer to the given, releasing the current one if non-null
	void reset(T* newPtr = 0) {
		outputDebugHeader();
		CONSOLE_LOG(F("reset with ptr, from "));
		CONSOLE_LOG((int)_ptr);
		CONSOLE_LOG(F(" to "));
		CONSOLE_LOG_LN((int)newPtr);

		clear();

		if (newPtr) {
			_ptr = newPtr;
			_count = new int(1);
		}
	}

	void debug() {
		outputDebugHeader();
		CONSOLE_LOG(F("debug from ptr "));
		CONSOLE_LOG_LN((int)_ptr);

		outputDebugHeader();
		CONSOLE_LOG(F("count = "));
		if (_count) {
			CONSOLE_LOG_LN(*_count);
		}
		else
		{
			CONSOLE_LOG_LN(F("NULL"));
		}
	}

	// Sets the pointer to a copy of the given, releasing the current one if non-null
	void reset(const shared_ptr_d<T>& newPtr) {
		outputDebugHeader();
		CONSOLE_LOG(F("reset with shared_ptr_d from "));
		CONSOLE_LOG((int)_ptr);
		CONSOLE_LOG(F(" to "));
		CONSOLE_LOG_LN((int)newPtr._ptr);

		clear();

		_ptr = newPtr._ptr;
		_count = newPtr._count;

		increase_count();
	}

	// Copy ownership from given unique_ptr to this one
	shared_ptr_d& operator= (const shared_ptr_d<T>& rhs) {
		reset(rhs);
		return *this;
	}


private:

	// Clear this pointer, deleting the managed object if needed
	void clear() {
		outputDebugHeader();
		CONSOLE_LOG(F("Clear from "));
		CONSOLE_LOG_LN((int)_ptr);

		// Was this object managing anything?
		if (_ptr) {
			decrease_count();

			outputDebugHeader();
			CONSOLE_LOG(F("Count = "));
			CONSOLE_LOG_LN(*_count);

			// Was this the last ref?
			if (*_count <= 0) {
				outputDebugHeader();
				CONSOLE_LOG_LN(F("Deleting objects"));
				T* oldRef = _ptr;

				// It was: delete the managed object and the count too
				delete _ptr;
				delete _count;

				outputDebugHeader();
				CONSOLE_LOG(F(""));
				CONSOLE_LOG((int)oldRef);
				CONSOLE_LOG_LN(F(" is destroyed"));
			}

			// Set refs to null
			_ptr = 0;
			_count = 0;
		}
	}

	void decrease_count() {
		outputDebugHeader();
		CONSOLE_LOG(F("Decrease_count from "));
		if (_count) {
			CONSOLE_LOG_LN(*_count);
		}
		else {
			CONSOLE_LOG_LN(F("NULL"));
		}

		if (_count)
			--(*_count);
	}

	void increase_count() {
		outputDebugHeader();
		CONSOLE_LOG(F("Increase_count from "));
		if (_count) {
			CONSOLE_LOG_LN(*_count);
		}
		else {
			CONSOLE_LOG_LN(F("NULL"));
		}

		if (_count)
			++(*_count);
	}

	void outputDebugHeader() {
		CONSOLE_LOG(F("shared_ptr_d("));
		CONSOLE_LOG((int)_ptr);
		CONSOLE_LOG(F(")::"));
	}
};

// Make a new object and return a shared_ptr_d to it. This is equivalent to
// "shared_ptr_d newPtr = shared_ptr_d(new Object(a, b, c));"
// But it's slightly more efficient and the syntax is more compact
template< class T, class... Args >
shared_ptr_d<T> make_shared(Args&&... args) {
	T* ptr = new T(args...);
	return shared_ptr_d<T>(ptr);
}

#include "Microprocessor_debugging\debugging_disable.h"