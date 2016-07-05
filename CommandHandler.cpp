#pragma once

#include "CommandHandler.h"

#include "Microprocessor_Debugging\debugging_disable.h"

// Add a new command to the list
void CommandLookup::registerCommand(const char* command, int num_of_parameters,
	commandFunction pointer_to_function)
{

	// Set up a struct containing the number of params and a pointer to the function
	dataStruct d;

	// Get hash of command
	long keyHash = djbHash(command);

	// Save params
	d.hash = keyHash;
	d.n = num_of_parameters;
	d.f = pointer_to_function;

	// Store it in the List
	_commandList.push_back(d);

}

// Search the list of commands for the given command and execute it with the given parameter array
ExecuteError CommandLookup::callStoredCommand(const ParameterLookup& params) {

	CONSOLE_LOG(F("callStoredCommand with n="));
	CONSOLE_LOG_LN(params.size());

	// Get hash of command requested
	const unsigned long reqHash = djbHash(params[0]);

	// Iterate through list searching for hash
	List<dataStruct>::const_iterator it = _commandList.begin();
	for (; it != _commandList.end(); it++) {
		if (reqHash == (*it).hash) {
			break;
		}
	}

	if (it == _commandList.end()) { return COMMAND_NOT_FOUND; }

	dataStruct d = *it;
	commandFunction* f = d.f;

	CONSOLE_LOG(F("Recalled data: d.n = "));
	CONSOLE_LOG_LN(d.n);

	// Return error if wrong number of parameters
	if (d.n != params.size() - 1 && d.n != -1) {
		CONSOLE_LOG(F("ERROR: Expecting "));
		CONSOLE_LOG(d.n);
		CONSOLE_LOG(F(" parameters but got "));
		CONSOLE_LOG_LN(params.size() - 1);

		return WRONG_NUM_OF_PARAMS;
	}

	CONSOLE_LOG_LN(F("Calling function..."));
	f(params);

	return NO_ERROR;
}

// CommandHandler constuctor
CommandHandler::CommandHandler() :
	_lookupList(), // Not needed, but just to be explicit
	_command_too_long(false),
	_bufferFull(false),
	_bufferLength(0)
{
	CONSOLE_LOG_LN(F("CommandHandler::CommandHandler()"));

	// Start the input buffer empty
	_inputBuffer[0] = '\0';
}

// Execute the next command in the queue
ExecuteError CommandHandler::executeCommand() {

	ExecuteError error = NO_ERROR;

	CONSOLE_LOG_LN(F("Execute command"));

	// Return error code if no command waiting
	if (!_bufferFull) {
		CONSOLE_LOG_LN(F("No command error"));
		error = NO_COMMAND_WAITING;
	}

	CONSOLE_LOG(F("Command is: "));
	CONSOLE_LOG_LN(_inputBuffer);

	// Return error code if string is empty
	if (_bufferLength == 0 && !error)
	{
		CONSOLE_LOG_LN(F("Empty command error"));
		error = EMPTY_COMMAND_STRING;
	}

	// Constuct a parameter lookup object from the command string
	// This invalidates the string for future use
	CONSOLE_LOG_LN(F("Creating ParameterLookup object..."));
	ParameterLookup lookupObj = ParameterLookup(_inputBuffer);
	
	CONSOLE_LOG_LN(F("Running callStoredCommand..."));
	ExecuteError found = _lookupList.callStoredCommand(lookupObj);

	// Mark buffer as ready again
	_bufferFull = false;
	_inputBuffer[0] = '\0';
	_bufferLength = 0;

	return found;
}

// Add a char from the serial connection to be processed and added to the queue
ExecuteError CommandHandler::addCommandChar(const char c) {

	// Check if the buffer is already full
	if (_bufferFull) {
		return BUFFER_FULL;
	}

	// If c is a newline, mark the buffer as full
	if (c == '\n') {

		CONSOLE_LOG(F("Newline received. Command: "));
		CONSOLE_LOG_LN(_inputBuffer);

		// Should we ignore this command?
		if (_command_too_long) {
			// Reset the `_command_too_long` flag
			_command_too_long = false;

			CONSOLE_LOG_LN(F("Ignoring command since too long"));

			return COMMAND_TOO_LONG;
		}

		// If not, we are already null terminated so mark the string as ready
		_bufferFull = true;
	}
	// if c is a carridge return, ignore it
	else if (c == '\r') {
		CONSOLE_LOG_LN(F("Ignoring a \r"));
		return NO_ERROR;
	}
	// else c is a normal char, so add it to the buffer
	else {
		if (_command_too_long || _bufferLength >= COMMAND_SIZE_MAX)
		{
			// Command was too long! Set the `_command_too_long` flag to chuck away all subsequent chars until next newline
			CONSOLE_LOG_LN(F("ERROR: command too long!"));

			_command_too_long = true;

			return COMMAND_TOO_LONG;
		}
		else
		{
			// The normal case. Add the new char to the buffer

			_inputBuffer[_bufferLength] = c;

			_bufferLength++;

			_inputBuffer[_bufferLength] = '\0';

			CONSOLE_LOG(F("Char received: '"));
			CONSOLE_LOG(c);
			CONSOLE_LOG(F("', Buffer length: "));
			CONSOLE_LOG_LN(_bufferLength);

			return NO_ERROR;
		}
	}

	return UNKNOWN_ERROR; // We should never get here
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
bool CommandHandler::executeStartupCommands() {

	CONSOLE_LOG_LN(F("CommandHandler::executeStartupCommands"));

	// Buffer for command from EEPROM
	char storedCmd[COMMAND_SIZE_MAX];

	// Read into buffer
	getStartupCommand(storedCmd);

	// Is the string empty?
	if (storedCmd[0] == '\0') {
		// Fail. No command stored
		CONSOLE_LOG_LN(F("CommandHandler::executeStartupCommands: No command stored"));
		return false;
	}

	// If not, queue each char into the command buffer
	// If we reach a newline, execute the command
	// If we reach a NULL, end
	int i = 0;
	while (storedCmd[i]) {

		CONSOLE_LOG(F("CommandHandler::executeStartupCommands: Queueing "));
		CONSOLE_LOG_LN(storedCmd[i]);

		addCommandChar(storedCmd[i++]);

		if (commandWaiting()) {
			executeCommand();
		}
	}

	return true;

}
#endif

unsigned long CommandLookup::djbHash(const char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = tolower(*str++)) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

	return hash;
}

// Constuctor.
// Replace spaces in commandStr with NULL and terminate the whole lot with
// a [0x03] char.
// The buffer pointed to by commandStr must be at least COMMAND_SIZE_MAX+1 in length
ParameterLookup::ParameterLookup(char * commandStr) :
	_theCommand(commandStr)
{
	CONSOLE_LOG(F("ParameterLookup::Constuctor with command: "));
	CONSOLE_LOG_LN(commandStr);

	// Loop through _theCommand counting params and subbing out
	// spaces or tabs for NULLs
	char * loop = _theCommand;
	_size = 1;

	while (*loop) {

		if (' ' == *loop || '\t' == *loop) {

			CONSOLE_LOG(F("ParameterLookup::Replacing char '"));
			CONSOLE_LOG(*loop);
			CONSOLE_LOG(F("' at pos "));
			CONSOLE_LOG(loop - _theCommand);
			CONSOLE_LOG_LN(F(" with \\0"));

			// Replace spaces with NULL chars
			*loop = '\0';

			// If the preceeding char wasn't also a space,
			// increment the param count
			if (loop > _theCommand && // Don't look too far back!
				'\0' != *(loop - sizeof(char))) {
				_size++;
			}
		}

		loop++;
	}

	// We looped to the last char which is a NULL.
	// Leave it as a NULL and make the next char a [0x03] too
	loop++;
	*loop = 0x03;

	CONSOLE_LOG(F("ParameterLookup::[0x03] stored at pos "));
	CONSOLE_LOG_LN(loop - _theCommand);

}

// Get parameter indexed. Parameter 0 is the command itself
// Requesting a non-existent parameter will return a NULL ptr
const char * ParameterLookup::operator [] (int idx) const {

	// The string is setup as described in "CommandHandler.h", so now all we need
	// to do is get and return a pointer to the start of the requested param
	// `_theCommand` is terminated by a [0x03] char, so don't go past this

	CONSOLE_LOG(F("ParameterLookup::Looking for param "));
	CONSOLE_LOG_LN(idx);
	
	char * paramPtr = _theCommand;
	int count = idx;

	while (0x03 != *paramPtr) {

		if (count == 0 && *paramPtr) {
			// We found a non null char after passing the required 
			// number of nulls, so return a pointer to it

			CONSOLE_LOG(F("ParameterLookup::Returning ptr to pos "));
			CONSOLE_LOG_LN(paramPtr - _theCommand);

			return paramPtr;
		}

		if ('\0' == *paramPtr && // We found a null...
								 // ... and the previous char wasn't a null
			paramPtr > _theCommand && '\0' != *(paramPtr - 1)) {
			// Decrement the count, as we've passed a delimiter
			count--;

			CONSOLE_LOG(F("ParameterLookup::Break found at pos "));
			CONSOLE_LOG_LN(paramPtr - _theCommand);
		}

		// Next char
		paramPtr++;
	}

	CONSOLE_LOG_LN(F("ParameterLookup::Not found"));

	// We got to the end without finding the requested param number
	// return a Null ptr
	return 0;
}