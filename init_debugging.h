#include <Arduino.h>

/* Provides CONSOLE_LOG functionality

Include "enable_debugging.h" to enable CONSOLE_LOG in a header file.
Include "disable_debugging.h" to disable

*/

// Flag to inform debugging scripts that they shoud output from now on
bool __serial_is_ready = false;

#define CONSOLE_LOG(...)
#define CONSOLE_LOG_LN(...)
