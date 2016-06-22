#include <Arduino.h>

/* Provides CONSOLE_LOG functionality

Include "enable_debugging.h" to enable CONSOLE_LOG in a header file.
Include "disable_debugging.h" to disable

*/

#ifndef SERIAL_READY
#define SERIAL_READY

// Flag to inform debugging scripts that they shoud output from now on
extern bool __serial_is_ready;

extern void debugPrint(int param);
extern void debugPrint(const char* param);
extern void debugPrint(const __FlashStringHelper* param);

extern void debugPrintLn(int param);
extern void debugPrintLn(const char* param);
extern void debugPrintLn(const __FlashStringHelper* param);
#endif

#define CONSOLE_LOG(s)
#define CONSOLE_LOG_LN(s)