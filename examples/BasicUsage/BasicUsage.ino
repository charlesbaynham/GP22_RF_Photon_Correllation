#include <CommandHandler.h>

// Create a CommandHandler object
CommandHandler h;

///////////////////////////////////////////////////////
// Declare functions to be called by serial commands //
///////////////////////////////////////////////////////

commandFunction helloWorld; // "hello"
commandFunction helloWorldQ; // "hello?"
commandFunction echoMe; // "echo"
commandFunction adder; // "add"
commandFunction echoMany; // "echoMany"

///////////////////////////////////////////////////////
//             End function declaration              //
///////////////////////////////////////////////////////

void setup() {

	Serial.begin(250000);

	// Register serial commands for triggering the previously
	// declared functions

	// Commands are case insensitive. Here we choose a serial command, the number of
	// parameters it takes (to be read as strings, delimited by spaces) and the
	// function it links to
	h.registerCommand("hello", 0, &helloWorld);
	h.registerCommand("hello?", 0, &helloWorldQ);
	h.registerCommand("echo", 1, &echoMe);
	h.registerCommand("add", 2, &adder);

	// Setting num params to "-1" will allow any number of params to be passed
	h.registerCommand("echoMany", -1, &echoMany);
}

void loop() {

	// Check for commands
	if (h.commandWaiting()) {

		// Execute first waiting command
		ExecuteError result = h.executeCommand();

		// Report any errors
		switch (result) {
		case COMMAND_NOT_FOUND:

			Serial.println(F("Command not found"));
			break;

		case WRONG_NUM_OF_PARAMS:

			Serial.println(F("Wrong number of params"));
			break;
		}
	}

	// Check for serial input
	while (Serial.available()) {
		// Queue input for processing
		h.addCommandChar(Serial.read());
	}
}

///////////////////////////////////////////////////////
//                 Define functions                  //
///////////////////////////////////////////////////////

// Takes no params
void helloWorld(const List<String>& params) {

	Serial.println(F("Hello world!"));

}

// Takes no params
void helloWorldQ(const List<String>& params) {

	Serial.println(F("You sent a SCPI query"));

}

// Echo a string
// 1 param
void echoMe(const List<String> &params) {

	String str = params.front();

	Serial.print(F("Passed string was: \""));
	Serial.print(str);
	Serial.println('"');
}

// Add two numbers
// 2 params
void adder(const List<String> &params) {

	// Get an iterator to iterate over the list of parameters
	List<String>::Iterator_const it = params.begin();

	// Convert the first parameter to a number
	String thisParam = *it;
	double first = atof(thisParam.c_str());

	// And the second
	it++;
	thisParam = *it;
	double second = atof(thisParam.c_str());

	// Add and output
	Serial.print(F("Sum of "));
	Serial.print(first);
	Serial.print(F(" and "));
	Serial.print(second);
	Serial.print(F(" is "));
	Serial.println(first + second);
}

// Echo unlimited number of parameters
// unlimited paramas
void echoMany(const List<String> &params) {

	// Get an iterator to iterate over the list of parameters
	List<String>::Iterator_const it;

	Serial.print(F("Number of params to echo is: "));
	Serial.println(params.size());

	// Loop until list is done
	for (it = params.begin(); it != params.end(); it++) {
		Serial.print('"');
		Serial.print(*it);
		Serial.println('"');
	}
}