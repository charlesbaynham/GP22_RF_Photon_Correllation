#pragma once

#define DEBUG

#define COMMAND_SIZE_MAX 128 // num chars
#define DEFAULT_NUM_COMMANDS_MAX 10 // Number of commands that need to be stored

#include "basicList.h"

//////////////////////  COMMAND LOOKUP  //////////////////////

	// Template for the functions we'll be calling
	typedef void (*commandFunction) (List<String> params, bool isQuery);

	// Structure of the data to be stored for each command
	struct dataStruct {
		char * key; // Keyword for this command
		int n; // Number of params this function takes
		int nq; // Number of params when called as a query
		commandFunction f; // Pointer to this function
	};

	// Error messages for executing a command
	enum ExecuteError {
		NO_ERROR = 0,
		COMMAND_NOT_FOUND,
		WRONG_NUM_OF_PARAMS,
		ERROR_PARSING_COMMAND,
		EMPTY_COMMAND_STRING,
		NO_COMMAND_WAITING,
		MALLOC_ERROR
	};

	class CommandLookup
	{
	public:

		CommandLookup() {}

		// Add a new command to the list
		void registerCommand(const char* command, int num_of_parameters,
			int num_of_query_parameters, commandFunction pointer_to_function);

		// Search the list of commands for the given command and execute it with the given parameter array
		ExecuteError callStoredCommand(const char* command, List<String> params, bool isQuery) ;

	protected:

		List<dataStruct> _commandList;
	};

//////////////////////  COMMAND HANDLER  //////////////////////

	class CommandHandler
	{
	public:
		CommandHandler() :
			_lookupList(),
			_command_too_long(false),
			_bufferLength(0) { _inputBuffer[0] = '\0'; }

		// Wipe any char*s left in the queue or buffer
		~CommandHandler() {
			free(_inputBuffer);
		}

		// Is a command waiting in the queue?
		bool commandWaiting() {
			return !_commandQueue.isEmpty();
		}

		// How long is the queue?
		int queueLength() {
			return _commandQueue.size();
		}

		// Dump the whole command queue for debugging
		void debug() {
			_commandQueue.debug();
		}

		// Execute the next command in the queue
		ExecuteError executeCommand() ;

		// Register a command
		void registerCommand(const char* command, int num_of_parameters,
				int num_of_query_parameters, commandFunction pointer_to_function) {
			_lookupList.registerCommand(command, num_of_parameters,
					num_of_query_parameters, pointer_to_function);
		}

		// Add a char from the serial connection to be processed and added to the queue
		void addCommandChar(const char c) ;

	private:
		CommandLookup _lookupList;

		// A queue of commands (char* pointers to char arrays of COMMAND_SIZE_MAX size)
		List <String> _commandQueue;

		// A buffer for receiving new commands
		char _inputBuffer[COMMAND_SIZE_MAX + 1];
		int _bufferLength;

		// A flag to report that the command currently being received has overrun
		bool _command_too_long;

		// Find the location in a command string where the command ends and the params start
		int findEndOfCommand(const char* str) ;

		// Loop from the first space onwards, counting the params
		int numParamsInCommandStr(const char* str, int endOfCommand) ;

		List<String> readParamsFromStr(const char* str, int endOfCommand) ;
		
	};

