// Test command handler

#include "CommandHandler/basicList.h"

typedef List<String> myList;

void loopFuncConst(const myList& testList);

myList testList;

void setup() {

	Serial.begin(250000);
	Serial.println("Launched");

	testList.push_back("Hello");
	testList.push_back("again");
	testList.push_back("world!");

}

void loop() {

	loopFuncConst(testList);

	Serial.print("*** Normal looping ***, ");

	for (myList::Iterator it = testList.begin(); it != testList.end(); it++) {

		String& str = *it;
		const char * buf = str.c_str();

		Serial.print("Normal iterator: '");
		Serial.print(buf);
		Serial.print("' from 0x");
		Serial.print((uint32_t)buf, HEX);
		Serial.print(", ");

	}

	Serial.println("");

}

void loopFuncConst(const myList& testList)  {

	Serial.print("*** Const looping ***, ");

	myList::Iterator_const it = testList.begin();

	for (; it != testList.end(); it++) {

		const String& str = *it;
		const char * buf = str.c_str();

		Serial.print("Normal iterator: '");
		Serial.print(buf);
		Serial.print("' from 0x");
		Serial.print((uint32_t)buf, HEX);
		Serial.print(", ");

	}

	Serial.println("");

}