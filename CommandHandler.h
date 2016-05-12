#pragma once

// Basic FIFO structure
#include "QueueArray.h"

// Basic Hashmap implementation
#include "Hashmap.h"

#define COMMAND_SIZE_MAX 128 // num chars
#define DEFAULT_NUM_COMMANDS_MAX 10 // Number of commands that need to be stored

#include "basicVector.h"

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
		NO_COMMAND_WAITING
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
			int num_of_query_parameters, commandFunction pointer_to_function)
		{
			if (_numCommandsAdded < _numCommandsMax) {

				// Set up a struct containing the number of params and a pointer to the function
				dataStruct d;
				d.n = num_of_parameters;
				d.nq = num_of_query_parameters;
				d.f = pointer_to_function;

				// Store it in the HashMap, referenced by the command name
				_commandTable[_numCommandsAdded++](command, d);

				return true;
			}
			return false;
		}

		// Search the list of commands for the given command and execute it with the given parameter array
		ExecuteError callStoredCommand(const char* command, Vector<String> params, bool isQuery) {

			#ifdef DEBUG
			Serial.print(F("callStoredCommand with n="));
			Serial.print(params.size());
			Serial.print(F(", isquery = "));
			Serial.println(isQuery ? "TRUE" : "FALSE");
			#endif

			if (!_commandTable.exists(command)) { return COMMAND_NOT_FOUND; }

			dataStruct d = _commandTable.getValueOf(command);
			commandFunction f = d.f;

			#ifdef DEBUG
			Serial.print(F("Recalled data: d.n = "));
			Serial.print(d.n);
			Serial.print(F(", d.nq = "));
			Serial.println(d.nq);
			#endif

			// Return error if too few parameters
			if (isQuery) {
				if (d.nq != params.size() && d.nq != -1) {
					return WRONG_NUM_OF_PARAMS;
				}
			}
			else {
				if (d.n != params.size() && d.n != -1) {
					#ifdef DEBUG
					Serial.print("ERROR: Expecting ");
					Serial.print(d.n);
					Serial.print(" parameters but got ");
					Serial.println(params.size());
					#endif

					return WRONG_NUM_OF_PARAMS;
				}
			}

			f(params, isQuery);

			return NO_ERROR;
		}

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

			while (!_commandQueue.isEmpty())
			{
				free(_commandQueue.pop());
			}
		}

		// Is a command waiting in the queue?
		bool commandWaiting() {
			return !_commandQueue.isEmpty();
		}

		// How long is the queue?
		int queueLength() {
			return _commandQueue.count();
		}

		// Execute the next command in the queue
		ExecuteError executeCommand() {
			
			#ifdef DEBUG
			Serial.println(F("Execute command"));
			#endif

			// Return error code -3 if no command waiting
			if (_commandQueue.isEmpty())
				return NO_COMMAND_WAITING;

			// Load and remove the next command from the queue
			char * nextCommand = _commandQueue.pop();

			// Return error code -5 if string is empty
			if (nextCommand[0] == '\0')
			{ 
				free(nextCommand);
				return EMPTY_COMMAND_STRING;
			}

			// The location in the string where the command ends and the params start
			int endOfCommand = findEndOfCommand(nextCommand);

			// If this failed, quit with an error
			if (endOfCommand < 0) {

				#ifdef DEBUG
				Serial.print(F("NextCommand: "));
				Serial.println(nextCommand);
				Serial.print(F("endOfCommand: "));
				Serial.println(endOfCommand);
				#endif

				free(nextCommand);

				return ERROR_PARSING_COMMAND;
			}

			// Allocate a space for the command word
			char * commandWord;
			commandWord = (char*)malloc((endOfCommand + 2) * sizeof(char));
			if (!commandWord)
				// Catastrooooooofic fail! Blink the LED to warn the user
				QueueArray<int>::blink();

			// Copy the command word from the previously found command
			strncpy(commandWord, nextCommand, endOfCommand + 1);

			// NULL terminate the string
			commandWord[endOfCommand+1] = '\0';

			// Check if this is a query SCPI command (command word ends in a '?')
			bool isQuery = (commandWord[endOfCommand] == '?');

			// If so, remove the '?' from the end too
			if (isQuery)
				commandWord[endOfCommand] = '\0';

			// Count the number of parameters in the string
			// int numParamsInCommand = numParamsInCommandStr(nextCommand, endOfCommand);

			// Declare a vector of Strings for the params and loop through command
			Vector<String> paramArray = readParamsFromStr(nextCommand, endOfCommand);

			#ifdef DEBUG
			Serial.print("endOfCommand: ");
			Serial.println(endOfCommand);
			Serial.print("commandWord: ");
			Serial.println(commandWord);
			Serial.print("paramArray size: ");
			Serial.println(paramArray.size());

			for (int i=0; i<paramArray.size(); i++)
				Serial.println(paramArray[i]);

			Serial.println("Executing...");
			#endif

			ExecuteError found = _lookupList.callStoredCommand(commandWord, paramArray, isQuery);

			free(nextCommand);
			free(commandWord);

			return found;
		}

		// Register a command
		bool registerCommand(const char* command, int num_of_parameters,
				int num_of_query_parameters, commandFunction pointer_to_function) {
			return _lookupList.registerCommand(command, num_of_parameters,
					num_of_query_parameters, pointer_to_function);
		}

		// Add a char from the serial connection to be processed and added to the queue
		bool addCommandChar(const char c) {
			
			// If c is a newline, store the buffer in the queue and start a new buffer
			if (c == '\n') {
				// Assign space for the command (this is freed when the command is called)
				char * newCommand;
				newCommand = (char*)calloc(COMMAND_SIZE_MAX, sizeof(char));
				if (!newCommand)
				// Catastrooooooofic fail! Blink the LED to warn the user
					QueueArray<int>::blink();

				// Copy the command into this space
				strcpy(newCommand, _inputBuffer);

				// Add the reference to the queue
				_commandQueue.push(newCommand);

				// Clear the input buffer
				_inputBuffer[0] = '\0';
				_bufferLength = 0;
			}
			// if c is a carridge return, ignore it
			else if (c == '\r') {}
			// else c is a normal char, so add it to the buffer
			else {
				if (_bufferLength >= COMMAND_SIZE_MAX)
					{ return false; }
				else
				{
					_inputBuffer[_bufferLength] = c;
					
					_bufferLength++;

					_inputBuffer[_bufferLength] = '\0';
					
				}
			}

			return true;
		}

		void debug() {
			Serial.println(_commandQueue.peek());
		}

	private:
		CommandLookup _lookupList;

		// A queue of commands (char* pointers to char arrays of COMMAND_SIZE_MAX size)
		QueueArray <char*> _commandQueue;

		// A buffer for receiving new commands
		char _inputBuffer[COMMAND_SIZE_MAX + 1];
		int _bufferLength;

		// Find the location in a command string where the command ends and the params start
		int findEndOfCommand(const char* str) {

			int endOfCommand = -1;

			// Loop until the first space to find the break between command and params
			for (int i = 0; i <= strlen(str); i++) {
				// Is this char a space or a NULL?
				if (str[i] == ' ' || str[i] == '\0') {
					endOfCommand = i-1;
					break;
				}
			}

			return endOfCommand;
		}

		// Loop from the first space onwards, counting the params
		int numParamsInCommandStr(const char* str, int endOfCommand) {
			
			int numParamsInCommand = 0; 
			bool lastWasSpace = true;
			
			for (int i = endOfCommand + 1; i < strlen(str); i++) {

				if (str[i] != ' ') {
					if (lastWasSpace) {
						// We found the start of a param
						// so add 1 to the count
						numParamsInCommand++;
					}

					lastWasSpace = false;
				}
				else
				{
					lastWasSpace = true;
				}
			}

			return numParamsInCommand;
		}

		Vector<String> readParamsFromStr(const char* str, int endOfCommand) {

			// Loop through the string
			bool lastWasSpace = true;
			int startParam = 0;
			int endParam = 0;
			char theParam[128]; // Buffer for the string conversion
			Vector<String> output;

			int destInd = 0;

			for (int i = endOfCommand + 1; i <= strlen(str); i++) {
				if (str[i] != ' ' && str[i] != '\0') {

					if (lastWasSpace) {
						// We found the start of a param
						startParam = i;
					}

					lastWasSpace = false;
				}
				else // if this char is a normal char (not a space or NULL char)
				{
					if (! lastWasSpace) {
						// We found the end of a param
						endParam = i;
						// ...so process it

						// Copy the param into a new string, `theParam`
						strncpy(theParam, str + startParam*sizeof(char), endParam - startParam);

						// Null terminate
						theParam[endParam - startParam] = '\0';

						// Add to vector
						output.push_back(theParam);

						// Increase index
						destInd++;

						// Reset the string
						theParam[0] = '\0';
					}

					lastWasSpace = true;
				}
			}

			return output;

		}
		
	};

