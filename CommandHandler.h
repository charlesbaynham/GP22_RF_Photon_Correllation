#pragma once

#define DEBUG

// Basic Hashmap implementation
#include "Hashmap.h"

#define COMMAND_SIZE_MAX 128 // num chars
#define DEFAULT_NUM_COMMANDS_MAX 10 // Number of commands that need to be stored

#include "basicVector.h"
#include "basicList.h"

//////////////////////  COMMAND LOOKUP  //////////////////////

	// Template for the functions we'll be calling
	typedef void (*commandFunction) (Vector<String> params, bool isQuery);

	// Structure of the data to be stored for each command
	struct dataStruct {
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

		CommandLookup(int maxNumCommands) :
			_numCommandsMax(maxNumCommands),
			// Allocate storage space for the hashmap
			_hashRawArray((HashType<dataStruct>*)malloc(maxNumCommands * sizeof(HashType<dataStruct>))),
			// Init the commands hash map using the allocated storage space
			_commandTable(HashMap<dataStruct>( _hashRawArray, maxNumCommands )),
			_numCommandsAdded(0)
		{}

		CommandLookup() : CommandLookup(DEFAULT_NUM_COMMANDS_MAX) {}

		// Add a new command to the list
		bool registerCommand(const char* command, int num_of_parameters,
			int num_of_query_parameters, commandFunction pointer_to_function);

		// Search the list of commands for the given command and execute it with the given parameter array
		ExecuteError callStoredCommand(const char* command, Vector<String> params, bool isQuery) ;

	protected:

		// Storage space for the hashMap 
		HashType<dataStruct> * _hashRawArray;
		// Hash map to store command info
		// Format will be "string" -> struct(int(num_of_parameters), void*(pointer_to_function))
		HashMap<dataStruct> _commandTable;

		// Make sure we don't overflow
		int _numCommandsAdded;

		int _numCommandsMax;
	};

//////////////////////  COMMAND HANDLER  //////////////////////

	class CommandHandler
	{
	public:
		CommandHandler(int maxCommands = DEFAULT_NUM_COMMANDS_MAX) :
			_lookupList(maxCommands),
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

		// Execute the next command in the queue
		ExecuteError executeCommand() ;

		// Register a command
		bool registerCommand(const char* command, int num_of_parameters,
				int num_of_query_parameters, commandFunction pointer_to_function) {
			return _lookupList.registerCommand(command, num_of_parameters,
					num_of_query_parameters, pointer_to_function);
		}

		// Add a char from the serial connection to be processed and added to the queue
		bool addCommandChar(const char c) ;

	private:
		CommandLookup _lookupList;

		// A queue of commands (char* pointers to char arrays of COMMAND_SIZE_MAX size)
		List <String> _commandQueue;

		// A buffer for receiving new commands
		char _inputBuffer[COMMAND_SIZE_MAX + 1];
		int _bufferLength;

		// Find the location in a command string where the command ends and the params start
		int findEndOfCommand(const char* str) ;

		// Loop from the first space onwards, counting the params
		int numParamsInCommandStr(const char* str, int endOfCommand) ;

		Vector<String> readParamsFromStr(const char* str, int endOfCommand) ;
		
	};

