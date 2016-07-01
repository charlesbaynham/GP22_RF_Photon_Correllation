#include <CommandHandler.h>

// Create a CommandHandler object
CommandHandler h;

///////////////////////////////////////////////////////
// Declare functions to be called by serial commands //
///////////////////////////////////////////////////////

// Store a string in EEPROM
// 1 param: the string to store
void storeCommand(const List<String>& params) {

	String command = params.front() + '\n';

	Serial.print(F("Storing string: \""));
	Serial.print(command);
	Serial.println('"');
	
	h.storeStartupCommand(command);
}

// Echo a string from EEPROM
// 0 params
void readCommand(const List<String> &params) {

	String str = h.getStartupCommand();

	Serial.print(F("Stored string was: \""));
	Serial.print(str);
	Serial.println('"');
}

// Clear command in EEPROM
// 0 params
void clearCommand(const List<String> &params) {

	h.wipeStartupCommand();

	Serial.println(F("Command cleared."));
}

///////////////////////////////////////////////////////
//             End function declaration              //
///////////////////////////////////////////////////////

void setup() {

	Serial.begin(250000);

	// Register serial commands
	h.registerCommand("put", 1, &storeCommand);
	h.registerCommand("get", 0, &readCommand);
	h.registerCommand("clear", 0, &clearCommand);
}

void loop() {

	// Check for commands
	if (h.commandWaiting()) {
		
		// Execute first waiting command
		ExecuteError result = h.executeCommand();

		if (result) {
			Serial.print("Error code ");
			Serial.println(result);
		}
	}

	// Check for serial input
	while (Serial.available()) {
		// Queue input for processing
		h.addCommandChar(Serial.read());
	}
}