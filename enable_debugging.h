#include <Arduino.h>

/* Provides CONSOLE_LOG functionality

Include "enable_debugging.h" to enable CONSOLE_LOG in a header file.
Include "disable_debugging.h" to disable

*/

#ifndef SERIAL_READY
#define SERIAL_READY

// Flag to inform debugging scripts that they shoud output from now on
extern bool __serial_is_ready;

#endif

#define CONSOLE_LOG(...)  Serial.print(__VA_ARGS__)
#define CONSOLE_LOG_LN(...) Serial.println(__VA_ARGS__)
