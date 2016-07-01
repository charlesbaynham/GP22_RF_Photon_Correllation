#pragma once

#include "CommandHandler.h"

#include "Microprocessor_Debugging\debugging_disable.h"

// Add a new command to the list
void CommandLookup::registerCommand(const char* command, int num_of_parameters,
	commandFunction pointer_to_function)
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
	d.f = pointer_to_function;

	// Store it in the List
	_commandList.push_back(d);

}

// Search the list of commands for the given command and execute it with the given parameter array
ExecuteError CommandLookup::callStoredCommand(const char* command, const List<String>& params) {

	CONSOLE_LOG(F("callStoredCommand with n="));
	CONSOLE_LOG_LN(params.size());

	// Make lower case copy of command requested
	char *lower_command;
	lower_command = (char*)calloc(strlen(command) + 1, sizeof(char));
	strcpy(lower_command, command);

	for (int i = 0; lower_command[i]; i++) {
		lower_command[i] = tolower(lower_command[i]);
	}

	// Iterate through list searching for key
	List<dataStruct>::const_iterator it = _commandList.begin();
	for (; it != _commandList.end(); it++) {
		if (0 == strcmp(lower_command, (*it).key)) {
			break;
		}
	}

	// Discard the lower case copy string
	free(lower_command);

	if (it == _commandList.end()) { return COMMAND_NOT_FOUND; }

	dataStruct d = *it;
	commandFunction* f = d.f;

	CONSOLE_LOG(F("Recalled data: d.n = "));
	CONSOLE_LOG_LN(d.n);

	// Return error if too few parameters
	if (d.n != params.size() && d.n != -1) {
		CONSOLE_LOG(F("ERROR: Expecting "));
		CONSOLE_LOG(d.n);
		CONSOLE_LOG(F(" parameters but got "));
		CONSOLE_LOG_LN(params.size());

		return WRONG_NUM_OF_PARAMS;
	}

	f(params);

	return NO_ERROR;
}

// CommandHandler constuctor
CommandHandler::CommandHandler() :
	_lookupList(), // Not needed, but just to be explicit
	_command_too_long(false),
	_bufferLength(0)
{
	CONSOLE_LOG_LN(F("CommandHandler::CommandHandler()"));

	// Start the input buffer empty
	_inputBuffer[0] = '\0';

#ifndef EEPROM_DISABLED
	// Queue any command that may be stored in the EEPROM
	queueStartupCommand();
#endif
}

// Execute the next command in the queue
ExecuteError CommandHandler::executeCommand() {

	CONSOLE_LOG_LN(F("Execute command"));

	// Return error code if no command waiting
	if (_commandQueue.isEmpty())
		return NO_COMMAND_WAITING;

	CONSOLE_LOG_LN(F("executeCommand: Popping from _commandQueue"));

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
	int startOfCommand = findStartOfCommand(nextCommand.c_str());

	// The location in the string where the command ends and the params start
	int endOfCommand = findEndOfCommand(nextCommand.c_str(), startOfCommand);

	// If this failed, quit with an error
	if (startOfCommand < 0 || endOfCommand < 0) {
		CONSOLE_LOG_LN(F("findEndOfCommand failed. Quitting with error"));
		return ERROR_PARSING_COMMAND;
	}

	// Copy the command word from the previously found command
	String commandWord = nextCommand.substring(startOfCommand, endOfCommand + 1);

	//// Check if this is a query SCPI command (command word ends in a '?')
	//bool isQuery = (commandWord[endOfCommand] == '?');

	//// If so, remove the '?' from the end
	//if (isQuery)
	//	commandWord.remove(endOfCommand);
	//
	// Count the number of parameters in the string
	// int numParamsInCommand = numParamsInCommandStr(nextCommand, endOfCommand);

	CONSOLE_LOG_LN(F("Running readParamsFromStr..."));

	// Declare a List of Strings for the params and loop through command to parse it for parameters
	List<String> params;
	readParamsFromStr(nextCommand.c_str(), endOfCommand, params);

	CONSOLE_LOG(F("commandWord: "));
	CONSOLE_LOG_LN(commandWord);
	CONSOLE_LOG(F("paramArray size: "));
	CONSOLE_LOG_LN(params.size());

	CONSOLE_LOG_LN(F("Running callStoredCommand..."));

	ExecuteError found = _lookupList.callStoredCommand(commandWord.c_str(), params);

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

#ifdef DEBUG
		CONSOLE_LOG_LN(F("_commandQueue contains:"));
		_commandQueue.debug();
#endif

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

// Find the location in a command string where the command starts
// i.e. the first char that isn't a space or a tab
int CommandHandler::findStartOfCommand(const char* str) {

	int i = 0;

	// Loop until the first non-space to find the start of the command
	while (i <= strlen(str)) {

		CONSOLE_LOG(F("CommandHandler::findStartOfCommand: first loop, char "));
		CONSOLE_LOG(i);
		CONSOLE_LOG(F(" is "));
		CONSOLE_LOG_LN(str[i]);

		// Is this anything other than a space or a tab?
		if (str[i] != ' ' && str[i] != '\t') {
			CONSOLE_LOG_LN(F("Breaking"));
			
			return i;
		}

		i++;
	}

	return -1;
}

// Find the location in a command string where the command ends and the params start
int CommandHandler::findEndOfCommand(const char* str, int startPoint) {

	int i = startPoint;

	// Loop until the first (post-command) space to find the break between command and params
	while (i <= strlen(str)) {

		CONSOLE_LOG(F("CommandHandler::findEndOfCommand: second loop, char "));
		CONSOLE_LOG(i);
		CONSOLE_LOG(F(" is "));
		CONSOLE_LOG_LN(str[i]);

		// Is this char a space or a NULL?
		if (str[i] == ' ' || str[i] == '\0') {
			CONSOLE_LOG(F("Second find at i = "));
			CONSOLE_LOG_LN(i);

			int out = i - 1;

			CONSOLE_LOG(F("CommandHandler::returning "));
			CONSOLE_LOG_LN(out);

			return out;
		}

		i++;
	}

	CONSOLE_LOG_LN(F("CommandHandler::endOfCommand not found"));

	return -1;
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


// Parse the string to extract the parameters and store them in destList
void CommandHandler::readParamsFromStr(const char* str, int endOfCommand, List<String>& destList) {

	// Loop through the string
	bool lastWasSpace = true;
	int startParam = 0;
	int endParam = 0;
	char theParam[COMMAND_SIZE_MAX]; // Buffer for the string conversion

	CONSOLE_LOG(F("readParamsFromStr: Running with command str: '"));
	CONSOLE_LOG(str);
	CONSOLE_LOG("', strlen = ");
	CONSOLE_LOG_LN(strlen(str));

	for (int i = endOfCommand + 1; i <= strlen(str); i++) {
		if (str[i] != ' ' && str[i] != '\0') { // if this char is NOT a space or NULL char

			if (lastWasSpace) {
				// We found the start of a param
				startParam = i;
				CONSOLE_LOG(F("readParamsFromStr: Start of param found at "));
				CONSOLE_LOG_LN(startParam);

				lastWasSpace = false;
			}
		}
		else // if this char is a space or NULL char
		{
			if (!lastWasSpace) {
				// We found the end of a param
				endParam = i;

				CONSOLE_LOG(F("readParamsFromStr: End of param found at "));
				CONSOLE_LOG_LN(endParam);

				// ...so process it

				// Copy the param into a new string, `theParam`
				strncpy(theParam, str + startParam*sizeof(char), endParam - startParam);

				// Null terminate
				theParam[endParam - startParam] = '\0';

				CONSOLE_LOG(F("readParamsFromStr: theParam contains: "));
				CONSOLE_LOG(theParam);
				CONSOLE_LOG(F(" with strlen: "));
				CONSOLE_LOG_LN(strlen(theParam));

				// Add to List
				destList.push_back(String(theParam));

				CONSOLE_LOG(F("readParamsFromStr: output contains: "));
#ifdef DEBUG
				destList.debug();
#endif

				// Reset the string
				theParam[0] = '\0';
			}

			lastWasSpace = true;
		}
	}
}

#ifndef EEPROM_DISABLED
// Store a command to be executed on startup in the EEPROM
// This command can include newlines: it will be copied verbatim into the
// buffer and then executed as a normal command would be
// It should include a terminating newline if you expect it to be run!
// Returns false on fail
bool CommandHandler::storeStartupCommand(const String& command) {

	// Check that the string is small enough to fit into the buffer, including null char
	if (command.length() > COMMAND_SIZE_MAX - 2) {
		CONSOLE_LOG_LN(F("CommandHandler::Command too long for EEPROM"));
		return false;
	}

	// Store it
	return storeStartupCommand(command.c_str());
}

// Store a command to be executed on startup in the EEPROM
// This command can include newlines: it will be copied verbatim into the
// buffer and then executed as a normal command would be
// It should include a terminating newline if you expect it to be run!
// Returns false on fail
bool CommandHandler::storeStartupCommand(const char * command) {

	// Store a flag indicating that a command exists
	const bool trueFlag = true;
	EEPROM.update(EEPROM_STORED_COMMAND_FLAG_LOCATION, trueFlag);

	// Loop through the command and store it
	int commandIdx = 0;
	int eeprom_ptr = 0;

	while (command[commandIdx] != '\0' && eeprom_ptr < COMMAND_SIZE_MAX - 2) {

		char toBeStored;
		
		// Check if it's a semicolon delimiter
		if (command[commandIdx] != ';') {
			// Nope. Just copy it
			toBeStored = command[commandIdx];
		}
		else {
			// It was a semicolon, so store a newline
			toBeStored = '\n';
		}

		// Store the char in EEPROM
		CONSOLE_LOG(F("CommandHandler::Update EEPROM ("));
		CONSOLE_LOG(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr);
		CONSOLE_LOG(F("): [0x"));
		CONSOLE_LOG(toBeStored, HEX);
		CONSOLE_LOG_LN(']');

		EEPROM.update(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr, toBeStored);

		// Increment string pointer and eeprom pointer
		commandIdx++;
		eeprom_ptr += sizeof(char);
	}

	// Terminate with a newline and a null
	CONSOLE_LOG(F("CommandHandler::Terminate EEPROM ("));
	CONSOLE_LOG(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr);
	CONSOLE_LOG('&');
	CONSOLE_LOG(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr + 1);
	CONSOLE_LOG_LN(')');

	EEPROM.update(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr, '\n');
	eeprom_ptr++;
	EEPROM.update(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr, '\0');

	return true;
}


// Remove any startup commands from the EEPROM
bool CommandHandler::wipeStartupCommand() {
	// There is no need to wipe the command from the EEPROM: we simply set the flag to false

	// Store a flag indicating that no command exists
	const bool falseFlag = false;

	EEPROM.update(EEPROM_STORED_COMMAND_FLAG_LOCATION, falseFlag);

	return true;
}

// Return any stored startup command. Returns "" if no command stored
String CommandHandler::getStartupCommand() {

	CONSOLE_LOG_LN(F("CommandHandler::getStartupCommand(String)"))

	// Buffer for retreived command
	char buffer[COMMAND_SIZE_MAX];

	// Read into buffer
	getStartupCommand(buffer);

	return String(buffer);
}

// Return any stored startup command by copying into buf.
// This has the same functionality as the other form and 
// avoids memory allocations on the heap, but places responsibility
// for memory management on the user
// buf must point to a buffer of at least COMMAND_SIZE_MAX chars
void CommandHandler::getStartupCommand(char * buf) {

	CONSOLE_LOG_LN(F("CommandHandler::getStartupCommand(char*)"))

	// Index of location in buffer
	int bufIdx = 0; // Start at start of buffer

	// There should be a bool stored in EEPROM_STORED_COMMAND_FLAG_LOCATION if this program has run before
	// It will tell us if there's a command to be read or not
	// Read it as a byte though, since the memory location will be 0xFF if it has never been written to
	// We only want to use it if it's exactly a bool
	char fromEEPROM;
	EEPROM.get(EEPROM_STORED_COMMAND_FLAG_LOCATION, fromEEPROM);

	CONSOLE_LOG(F("CommandHandler::EEPROM flag contains : [0x"));
	CONSOLE_LOG(fromEEPROM, HEX);
	CONSOLE_LOG_LN(']');

	if (fromEEPROM == (char)true) {

		CONSOLE_LOG_LN(F("CommandHandler::EEPROM flag true"));

		// Index of location in EEPROM
		int EEPROM_idx = EEPROM_STORED_COMMAND_LOCATION;

		// At most, go to the end of the buffer - 1, to leave space for a null terminator
		while (bufIdx < COMMAND_SIZE_MAX - 1)
		{
			char c;
			EEPROM.get(EEPROM_idx, c);

			CONSOLE_LOG(F("CommandHandler::Read from EEPROM ("));
			CONSOLE_LOG(EEPROM_idx);
			CONSOLE_LOG(F("): [0x"));
			CONSOLE_LOG(c, HEX);
			CONSOLE_LOG_LN(']');

			if (c == '\0') {
				// Found the end

				CONSOLE_LOG(F("CommandHandler::Stored command ends at "));
				CONSOLE_LOG_LN(EEPROM_idx);

				break;
			}

			// Store the char
			buf[bufIdx] = c;

			EEPROM_idx++;
			bufIdx++;
		}
	}
	else if (fromEEPROM == (char)false) {
		CONSOLE_LOG_LN(F("CommandHandler::EEPROM flag false"));
	}
	else {
		CONSOLE_LOG_LN(F("CommandHandler::EEPROM flag undefined"));
	}

	// Null terminate
	buf[bufIdx] = '\0';
}

// Queue the startup command stored in the EEPROM
// Returns true on success, false on failure or if no command is stored
bool CommandHandler::queueStartupCommand() {

	CONSOLE_LOG_LN(F("CommandHandler::queueStartupCommand"));

	// Buffer for command in EEPROM
	char storedCmd[COMMAND_SIZE_MAX];

	// Read into buffer
	getStartupCommand(storedCmd);

	// Is the string empty?
	if (storedCmd[0] == '\0') {
		// Fail. No command stored
		CONSOLE_LOG_LN(F("CommandHandler::queueStartupCommand: No command stored"));
		return false;
	}

	// If not, queue each char into the command queue. 
	int i = 0;
	while (true) {

		CONSOLE_LOG(F("CommandHandler::queueStartupCommand: Queueing "));
		CONSOLE_LOG_LN(storedCmd[i]);

		addCommandChar(storedCmd[i]);

		i++;

		if (storedCmd[i] == '\0') break;
	}

	return true;

}
#endif