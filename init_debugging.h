#include <Arduino.h>

/* Provides CONSOLE_LOG functionality

Include "enable_debugging.h" to enable CONSOLE_LOG in a header file. 
Include "disable_debugging.h" to disable

*/

// Flag to inform debugging scripts that they shoud output from now on
bool __serial_is_ready = false;

void debugPrint(int param) {
	if (__serial_is_ready) {
		Serial.print(param);
	}
}
void debugPrint(float param) {
	if (__serial_is_ready) {
		Serial.print(param);
	}
}
void debugPrint(double param) {
	if (__serial_is_ready) {
		Serial.print(param);
	}
}
void debugPrint(const char* param) {
	if (__serial_is_ready) {
		Serial.print(param);
	}
}
void debugPrint(const __FlashStringHelper* param) {
	if (__serial_is_ready) {
		Serial.print(param);
	}
}

void debugPrintLn(int param) {
	if (__serial_is_ready) {
		Serial.println(param);
	}
}
void debugPrintLn(float param) {
	if (__serial_is_ready) {
		Serial.println(param);
	}
}
void debugPrintLn(double param) {
	if (__serial_is_ready) {
		Serial.println(param);
	}
}
void debugPrintLn(const char* param) {
	if (__serial_is_ready) {
		Serial.println(param);
	}
}
void debugPrintLn(const __FlashStringHelper* param) {
	if (__serial_is_ready) {
		Serial.println(param);
	}
}

#define CONSOLE_LOG(s)
#define CONSOLE_LOG_LN(s)