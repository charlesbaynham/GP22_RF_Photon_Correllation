#pragma once

// #define DEBUG

#ifdef DEBUG
#define CONSOLE_LOG(s)  Serial.print(s)
#define CONSOLE_LOG_LN(s)  Serial.println(s)
#include <Arduino.h>
#else
#define CONSOLE_LOG(s) 
#define CONSOLE_LOG_LN(s)
#endif


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
		CONSOLE_LOG_LN(F("shared_ptr_d::Null constuctor"));
	}

	// Make a smart pointer and give it a pointer to look after
	explicit shared_ptr_d(T* ptr) : shared_ptr_d()
	{
		CONSOLE_LOG_LN(F("shared_ptr_d::Ptr constuctor"));
		reset(ptr);
	}

	// Copy the given pointer
	shared_ptr_d(const shared_ptr_d& rhs) :
		shared_ptr_d(0)
	{
		CONSOLE_LOG_LN(F("shared_ptr_d::Copy constuctor."));

		reset(rhs);
	}


	// Destroy the smart pointer. This will also destroy the managed object if there is one
	// and this is the last instance of this shared_ptr_d
	~shared_ptr_d() {
		CONSOLE_LOG_LN(F("shared_ptr_d::Destructor"));
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

	// Dereferences to the held object
	T& operator * () {
		CONSOLE_LOG_LN(F("shared_ptr_d::Deref"));
		return *_ptr;
	}

	// Dereferences to the held object - const
	const T& operator * () const {
		CONSOLE_LOG_LN(F("shared_ptr_d::Deref const"));
		return *_ptr;
	}

	// Dereferences to the held object
	T* operator -> () {
		CONSOLE_LOG_LN(F("shared_ptr_d::Pointer deref"));
		return _ptr;
	}

	// Sets the pointer to the given, releasing the current one if non-null
	void reset(T* newPtr) {
		CONSOLE_LOG_LN(F("shared_ptr_d::reset from ptr"));
		clear();

		if (newPtr) {
			_ptr = newPtr;
			_count = new int(1);
		}
	}

	// Sets the pointer to a copy of the given, releasing the current one if non-null
	void reset(const shared_ptr_d<T>& newPtr) {
		CONSOLE_LOG_LN(F("shared_ptr_d::reset from shared_ptr_d"));
		clear();

		_ptr = newPtr._ptr;
		_count = newPtr._count;

		increase_count();

		CONSOLE_LOG(F("shared_ptr_d::Count = "));
		CONSOLE_LOG_LN(*_count);
	}

	// Copy ownership from given unique_ptr to this one
	shared_ptr_d& operator= (const shared_ptr_d<T>& rhs) {
		reset(rhs);
		return *this;
	}

	
private:

	// Clear this pointer, deleting the managed object if needed
	void clear() {
		CONSOLE_LOG_LN(F("shared_ptr_d::Clear"));
		// Was this object managing anything?
		if (_ptr) {
			decrease_count();

			CONSOLE_LOG(F("shared_ptr_d::Count = "));
			CONSOLE_LOG_LN(*_count);

			// Was this the last ref?
			if (*_count <= 0) {
				CONSOLE_LOG_LN(F("shared_ptr_d::Deleting objects"));

				// It was: delete the managed object and the count too
				delete _ptr;
				delete _count;
			}
		}
	}

	void decrease_count() {
		CONSOLE_LOG_LN(F("shared_ptr_d::Decrease_count"));
		--(*_count);
	}

	void increase_count() {
		CONSOLE_LOG_LN(F("shared_ptr_d::Increase_count"));
		++(*_count);
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
