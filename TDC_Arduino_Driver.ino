// Test command handler

#include "CommandHandler/basicList.h"

void loopFunc(const List<String>& testList);
		
void setup() {

	Serial.begin(250000);
	Serial.println("Launched");

	List<String> testList;

	testList.push_back("Hello");
	testList.push_back("world");
	testList.push_back("again!");

	loopFunc(testList);
}

void loop() {}

void loopFunc(const List<String>& testList)  {

	List<String>::Iterator_const it = testList.begin();

	for (; it != testList.end(); it++) {
		Serial.print("Looping: ");
		Serial.println(*it);
	}

}