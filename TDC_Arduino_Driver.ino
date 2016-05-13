// Test command handler

#include "CommandHandler/CommandHandler.h"

CommandHandler handler;

void setup() {
	Serial.begin(250000);
	Serial.println("Launched");
	registerCommands(&handler);
}

void loop() {
	if (handler.commandWaiting()) {

		Serial.println(F("Command received."));

		ExecuteError out = handler.executeCommand();

		if (out) {
			Serial.print(F("Error in command: ExecuteError code "));
			Serial.println(out);
		}
	}
}

void serialEvent() {
	while (Serial.available()) {
		char c = Serial.read();

		Serial.print(F("Adding: ["));
		Serial.print(c, DEC);
		Serial.println(F("]"));

		handler.addCommandChar(c);
	}
}

void doNothing(List<String> params, bool isQuery) {
	Serial.println("Nothing done");
}

void registerCommands(CommandHandler* h) {
	// N.B. commands are not case sensitive

	h->registerCommand("*IDN", 0, 0, *doNothing);
	h->registerCommand("*TST", 0, 0, *doNothing);
	h->registerCommand("*RST", 0, 0, *doNothing);
	h->registerCommand("MEAS", 1, 1, *doNothing);
	h->registerCommand("SING", 1, 1, *doNothing);
	h->registerCommand("STAT", 0, 0, *doNothing);
	h->registerCommand("SETU", -1, 0, *doNothing);
	h->registerCommand("*MEM", 0, 0, *doNothing);
	h->registerCommand("HCAL", 0, 0, *doNothing);
	h->registerCommand("CALI", 0, 0, *doNothing);

}
