#pragma once

// #define DEBUG

#ifdef DEBUG
#define CONSOLE_LOG(s)  Serial.print(s)
#define CONSOLE_LOG_LN(s)  Serial.println(s)
#else
#define CONSOLE_LOG(s) 
#define CONSOLE_LOG_LN(s)
#endif

#include "CommandHandler.h"

// Add a new command to the list
void CommandLookup::registerCommand(const char* command, int num_of_parameters,
	int num_of_query_parameters, commandFunction pointer_to_function)
{
	
	// Set up a struct containing the number of params and a pointer to the function
	dataStruct d;

	// Get length of command
	int keyLen = strlen(command);

	// Allocate enough space for string
	d.key = (char*)calloc(keyLen + 1, sizeof(char));

	// Copy string into data struct
	strcpy(d.key, command);

	// Make lower case
	for (int i = 0; d.key[i]; i++) {
		d.key[i] = tolower(d.key[i]);
	}

	// Save other params
	d.n = num_of_parameters;
	d.nq = num_of_query_parameters;
	d.f = pointer_to_function;

	// Store it in the Vector
	_commands.push_back(d);
	
}
// Destructor: remove all the dataStructs' strings
CommandLookup::~CommandLookup() {
	for (int i = 0; i < _commands.size(); i++) {
		free(_commands[i].key);
	}
}

// Search the list of commands for the given command and execute it with the given parameter array
ExecuteError CommandLookup::callStoredCommand(const char* command, List<String> params, bool isQuery) {

	CONSOLE_LOG(F("callStoredCommand with n="));
	CONSOLE_LOG(params.size());
	CONSOLE_LOG(F(", isquery = "));
	CONSOLE_LOG_LN(isQuery ? "TRUE" : "FALSE");
	
	// Make lower case copy of command
	char *lower_command;
	lower_command = (char*)calloc(strlen(command) + 1, sizeof(char));
	strcpy(lower_command, command);
	
	for (int i = 0; lower_command[i]; i++) {
		lower_command[i] = tolower(lower_command[i]);
	}

	int foundInd = -1;
	// Loop over vector searching for key
	for (int i = 0; i < _commands.size(); i++) {
		if (0 == strcmp(lower_command, _commands[i].key)) {
			foundInd = i;
			break;
		}
	}

	if (foundInd == -1) { return COMMAND_NOT_FOUND; }

	dataStruct d = _commands[foundInd];
	commandFunction f = d.f;

	CONSOLE_LOG(F("Recalled data: d.n = "));
	CONSOLE_LOG(d.n);
	CONSOLE_LOG(F(", d.nq = "));
	CONSOLE_LOG_LN(d.nq);

	// Return error if too few parameters
	if (isQuery) {
		if (d.nq != params.size() && d.nq != -1) {
			return WRONG_NUM_OF_PARAMS;
		}
	}
	else {
		if (d.n != params.size() && d.n != -1) {
			CONSOLE_LOG(F("ERROR: Expecting "));
			CONSOLE_LOG(d.n);
			CONSOLE_LOG(F(" parameters but got "));
			CONSOLE_LOG_LN(params.size());

			return WRONG_NUM_OF_PARAMS;
		}
	}

	f(params, isQuery);

	return NO_ERROR;
}

// Execute the next command in the queue
ExecuteError CommandHandler::executeCommand() {
	
	CONSOLE_LOG_LN(F("Execute command"));

	// Return error code -3 if no command waiting
	if (_commandQueue.isEmpty())
		return NO_COMMAND_WAITING;

	// Load and remove the next command from the queue
	String nextCommand = _commandQueue.front();
	_commandQueue.pop_front();

	CONSOLE_LOG(F("Command is: "));
	CONSOLE_LOG_LN(nextCommand);

	CONSOLE_LOG_LN(F("_commandQueue now contains:"));
#ifdef DEBUG
	_commandQueue.debug();
#endif

	// Return error code if string is empty
	if (nextCommand.length() == 0)
	{
		return EMPTY_COMMAND_STRING;
	}

	// The location in the string where the command ends and the params start
	int endOfCommand = findEndOfCommand(nextCommand.c_str());

	// If this failed, quit with an error
	if (endOfCommand < 0) {

		CONSOLE_LOG(F("NextCommand: "));
		CONSOLE_LOG_LN(nextCommand);
		CONSOLE_LOG(F("endOfCommand: "));
		CONSOLE_LOG_LN(endOfCommand);

		return ERROR_PARSING_COMMAND;
	}

	// Copy the command word from the previously found command
	String commandWord = nextCommand.substring(0, endOfCommand+1);

	// Check if this is a query SCPI command (command word ends in a '?')
	bool isQuery = (commandWord[endOfCommand] == '?');

	// If so, remove the '?' from the end
	if (isQuery)
		commandWord.remove(endOfCommand);

	// Count the number of parameters in the string
	// int numParamsInCommand = numParamsInCommandStr(nextCommand, endOfCommand);

	// Declare a vector of Strings for the params and loop through command
	List<String> params = readParamsFromStr(nextCommand.c_str(), endOfCommand);

	CONSOLE_LOG(F("endOfCommand: "));
	CONSOLE_LOG_LN(endOfCommand);
	CONSOLE_LOG(F("commandWord: "));
	CONSOLE_LOG_LN(commandWord);
	CONSOLE_LOG(F("paramArray size: "));
	CONSOLE_LOG_LN(params.size());

	CONSOLE_LOG_LN(F("Executing..."));

	ExecuteError found = _lookupList.callStoredCommand(commandWord.c_str(), params, isQuery);

	return found;
}

// Add a char from the serial connection to be processed and added to the queue
void CommandHandler::addCommandChar(const char c) {
	
	// If c is a newline, store the buffer in the queue and start a new buffer
	if (c == '\n') {

		CONSOLE_LOG(F("Newline received: storing in _commandQueue: "));
		CONSOLE_LOG_LN(String(_inputBuffer));

		// Should we ignore this command?
		if (_command_too_long) {
			// Reset the `_command_too_long` flag
			_command_too_long = false;

			CONSOLE_LOG_LN(F("Ignoring command since too long"));
		}
		else {
			CONSOLE_LOG_LN(F("Adding command to queue"));
			// Add the new command to the queue
			_commandQueue.push_back(String(_inputBuffer));
		}

		CONSOLE_LOG_LN(F("_commandQueue contains:"));
		_commandQueue.debug();

		// Clear the input buffer
		_inputBuffer[0] = '\0';
		_bufferLength = 0;
	}
	// if c is a carridge return, ignore it
	else if (c == '\r') {
		CONSOLE_LOG_LN(F("Ignoring a \r"));
	}
	// else c is a normal char, so add it to the buffer
	else {
		if (_command_too_long || _bufferLength >= COMMAND_SIZE_MAX)
		{
			// Command was too long! Set the `_command_too_long` flag to chuck away all subsequent chars until next newline
			CONSOLE_LOG_LN(F("ERROR: command too long!"));

			_command_too_long = true;

			return;
		}
		else
		{
			_inputBuffer[_bufferLength] = c;
			
			_bufferLength++;

			_inputBuffer[_bufferLength] = '\0';

			CONSOLE_LOG(F("Char received: '"));
			CONSOLE_LOG(c);
			CONSOLE_LOG(F("', Buffer length: "));
			CONSOLE_LOG_LN(_bufferLength);
			
		}
	}
}


// Find the location in a command string where the command ends and the params start
int CommandHandler::findEndOfCommand(const char* str) {

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
int CommandHandler::numParamsInCommandStr(const char* str, int endOfCommand) {
	
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

List<String> CommandHandler::readParamsFromStr(const char* str, int endOfCommand) {

	// Loop through the string
	bool lastWasSpace = true;
	int startParam = 0;
	int endParam = 0;
	char theParam[128]; // Buffer for the string conversion
	List<String> output;

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

				// Add to List
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
