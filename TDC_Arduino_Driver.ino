// Test command handler

#include "CommandHandler/basicList.h"

typedef List<String> myList;

void loopFuncConst(const myList& testList);

void setup() {

	Serial.begin(250000);
	Serial.println("Launched");

	myList testList;

	testList.push_back("Hello");
	testList.push_back("again");
	testList.push_back("world!");

	loopFuncConst(testList);

	Serial.println("*** Normal looping ***");

	for (myList::Iterator it = testList.begin(); it != testList.end(); it++) {
		
		String& str = *it;
		const char * buf = str.c_str();
		
		Serial.print("Normal iterator: '");
		Serial.print(buf);
		Serial.print("' from 0x");
		Serial.println((uint32_t)buf, HEX);

		str.remove(0,1);

	}

	loopFuncConst(testList);
}

void loop() {}

void loopFuncConst(const myList& testList)  {

	Serial.println("*** Const looping ***");

	myList::Iterator_const it = testList.begin();

	for (; it != testList.end(); it++) {

		const String& str = *it;
		const char * buf = str.c_str();

		Serial.print("Normal iterator: '");
		Serial.print(buf);
		Serial.print("' from 0x");
		Serial.println((uint32_t)buf, HEX);

	}

}