#pragma once

#define COMMAND_SIZE_MAX 128 // num chars
#define DEFAULT_NUM_COMMANDS_MAX 10 // Number of commands that need to be stored

// Storage locations in EEPROM for commands
#include <EEPROM.h>
#define EEPROM_STORED_COMMAND_FLAG_LOCATION 0
#define EEPROM_STORED_COMMAND_LOCATION EEPROM_STORED_COMMAND_FLAG_LOCATION + sizeof(bool)

// To disable EEPROM features, set this flag:
// #define EEPROM_DISABLED

#include "basicList.h"
#include "shared_ptr_d.h"

//////////////////////  COMMAND LOOKUP  //////////////////////

// This class is responsible for matching strings -> commands
// It maintains a List of hashes, associated commands and number of
// parameters required for those commands
// `callStoredCommand` performs the lookup and calls the appropriate
// command, passing through a parameter lookup object

	// Template for the functions we'll be calling
	typedef void commandFunction (const List<shared_ptr_d<String>>& params);

	// Structure of the data to be stored for each command
	struct dataStruct {
		unsigned long hash; // Hash of the keyword (case insensitive)
		int n; // Number of params this function takes
		commandFunction* f; // Pointer to this function
	};

	// Error messages for executing a command
	enum ExecuteError {
		NO_ERROR = 0,
		COMMAND_NOT_FOUND,
		WRONG_NUM_OF_PARAMS,
		ERROR_PARSING_COMMAND,
		EMPTY_COMMAND_STRING,
		NO_COMMAND_WAITING,
		MALLOC_ERROR, 
		OUT_OF_MEM,
		BUFFER_FULL,
		COMMAND_TOO_LONG,
		UNKNOWN_ERROR
	};

	class CommandLookup
	{
	public:

		CommandLookup() {}

		// Add a new command to the list
		void registerCommand(const char* command, int num_of_parameters,
			commandFunction* pointer_to_function);

		// Search the list of commands for the given command and execute it with the given parameter array
		ExecuteError callStoredCommand(const char* command, const List<shared_ptr_d<String>>& params);

	protected:

		List<dataStruct> _commandList;

		// Hash string (case insensitive)
		// Uses	the djb2 algorithm by Dan Bernstein
		// See http://www.cse.yorku.ca/~oz/hash.html
		unsigned long djbHash(const char *str);
	};

//////////////////////  COMMAND HANDLER  //////////////////////

// This class handle the receiving and executing of commands. It should be
// fed chars from the serial input by `addCommandChar()`
// When a command is ready it will flag `commandWaiting()`
// When convenient, `executeCommand()` should then be called. This will
// invoke the nested CommandLookup object in order to get the right command
// and execute it
//
// This class also contains methods for storing commands in the EEPROM in 
// order to queue a command on device startup
// These can be disabled by adding the line:
// #define EEPROM_DISABLED

	class CommandHandler
	{
	public:
		// Constuctor
		// Initialise private members and queue any stored command in the EEPROM
		CommandHandler();

		// Execute the waiting command
		ExecuteError executeCommand();

		// Register a command
		void registerCommand(const char* command, int num_of_parameters,
				commandFunction* pointer_to_function) {
			_lookupList.registerCommand(command, num_of_parameters,
					pointer_to_function);
		}

		// Add a char from the serial connection to be processed and added to the queue
		// Returns BUFFER_FULL if buffer is full and char wasn't added
		ExecuteError addCommandChar(const char c) ;

		// Check to see if the handler is ready for more incoming chars
		inline bool bufferFull() { return _bufferFull; }

		// Is a command waiting?
		inline bool commandWaiting() { return bufferFull(); }

#ifndef EEPROM_DISABLED
		// Store a command to be executed on startup in the EEPROM
		// This command should not include newlines: it will be copied verbatim into the
		// buffer and then executed as a normal command would be
		// Multiple commands can be seperated by ';' chars
		// Max length is COMMAND_LENGTH_MAX - 2 (1 char to append a newline, 1 for the null term)
		// Returns false on fail
		bool storeStartupCommand(const String& command);

		// Store a command to be executed on startup in the EEPROM
		// This command should not include newlines: it will be copied verbatim into the
		// buffer and then executed as a normal command would be
		// Multiple commands can be seperated by ';' chars
		// Max length is COMMAND_LENGTH_MAX - 2 (1 char to append a newline, 1 for the null term)
		// Returns false on fail
		bool storeStartupCommand(const char* command);

		// Remove any startup commands from the EEPROM
		bool wipeStartupCommand();

		// Return any stored startup command. Returns "" if no command stored
		String getStartupCommand();

		// Return any stored startup command by copying into buf.
		// This has the same functionality as the other form and 
		// avoids memory allocations on the heap, but places responsibility
		// for memory management on the user
		// buf must point to a buffer of at least COMMAND_SIZE_MAX chars
		void getStartupCommand(char * buf);

		// Queue the startup command stored in the EEPROM
		// Returns true on success, false on failure or if no command is stored
		bool queueStartupCommand();
#endif

	private:
		// An object for handling the matching of commands -> functions
		CommandLookup _lookupList;

		// Flag to warn that the command handler cannot handle more incoming chars
		// until the current command is processed
		bool _bufferFull;

		// A buffer for receiving new commands
		char _inputBuffer[COMMAND_SIZE_MAX + 1];
		unsigned int _bufferLength;

		// A flag to report that the command currently being received has overrun
		bool _command_too_long;

		// Find the location in a command string where the command starts
		int findStartOfCommand(const char* str);

		// Find the location in a command string where the command ends and the params start
		int findEndOfCommand(const char* str, int startPoint = 0);

		// Loop from the first space onwards, counting the params
		int numParamsInCommandStr(const char* str, int endOfCommand) ;

		// Parse a string to extract the parameters and store them in destList
		int readParamsFromStr(const char* str, int endOfCommand, List<shared_ptr_d<String>>& destList);
		
	};

	class ParameterLookup {

	public:

		// Get parameter indexed. Parameter 0 is the command itself
		// Requesting a non-existent parameter will return an empty string
		const char * operator [] (int idx);

		// Number of stored params, including the command itself
		unsigned int size();

	};