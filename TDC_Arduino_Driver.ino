#define TDC_CS 10
#define TDC_INT 12

#include <math.h>
#include <SPI.h>

byte * transferN(const byte * data, uint8_t n, byte * outputPtr = 0);

#define PROG_IDN "GP22_DRIVER"
#define PROG_VER "0.1"

#define TDC_WRITE_TO_REGISTER 0x80
#define TDC_READ_FROM_REGISTER 0xB0
#define TDC_REG0 0x00
#define TDC_REG1 0x01
#define TDC_REG2 0x02
#define TDC_REG3 0x03
#define TDC_REG4 0x04
#define TDC_REG5 0x05
#define TDC_STATUS 0x04
#define TDC_RESULT1 0x00
#define TDC_RESULT2 0x01
#define TDC_RESULT3 0x02
#define TDC_RESULT4 0x03

#define TDC_READ_CONFIG_FROM_EEPROM 0xF0
#define TDC_INIT 0x70
#define TDC_RESET 0x50

// Function to reset the arduino:
void(*resetFunc) (void) = 0;

void setup() {

	// Serial connection
	Serial.begin(115200);

	// Setup the interrupt pin for input
	pinMode(TDC_INT, INPUT);

	// Initialize the SPI connection for the GP22 TDC:

	SPI.begin(TDC_CS);
	/* " The TDC-GP22 does only support the following SPI mode (Motorola specification):
	Clock Phase Bit = 1
	Clock Polarity Bit = 0 "    =>          */
	SPI.setDataMode(TDC_CS, SPI_MODE1);
	SPI.setBitOrder(TDC_CS, MSBFIRST);

	// Setup complete. Move to waiting for a command
}

void loop() {

	if (Serial.available()) {
		// If there's a command, read it.
		char command[5];
		command[Serial.readBytesUntil('\n', command, 4)] = 0;

		// Identify and execute the command
		if (0 == strcmp("*RST", command)) {
			Serial.println("Resetting");
			Serial.flush();
			resetFunc(); // Reset
		}
		else if (0 == strcmp("*IDN", command)) { // Identify
			Serial.print(PROG_IDN);
			Serial.print(" - ");
			Serial.println(PROG_VER);
		}
		else if (0 == strcmp("MEAS", command)) { // Do the measurements

			initTDC(); // Load the EEPROM config

			// Read the number of ms to time for

			// Advance to the params
			Serial.readBytesUntil(':', (char*)0, 100);

			char timePeriodStr[32];
			timePeriodStr[Serial.readBytesUntil('\n', timePeriodStr, 31)] = 0;

			// Convert string -> long int
			uint32_t timePeriod = atol(timePeriodStr);

			// Calculate stop time
			uint32_t stop = millis() + timePeriod;

			// Loop and report
			while (millis() < stop) {

				// Do the measurement
				uint32_t result = measure();

				// Check we didn't timeout
				if (result != 0xFFFFFFFF) {
					// Report
					Serial.println(result);
				}
			}
		}
		else if (0 == strcmp("*TST", command)) { // Test connection

			int testResult = testTDC();
			if (testResult) {
				Serial.print("FAILED with response ");
				Serial.println(testResult);
			}
			else {
				Serial.println("PASSED");
			}
		}


	}

}

int testTDC() {

	// Send reset
	SPI.transfer(TDC_CS, 0x50);

	// Wait 100ms
	delay(100);

	// Write 0101010101000000000000000 into register 1 (the defaults)
	//  == 0xAA8000
#define testData 0xAA

	// Command:
	SPI.transfer(TDC_CS, TDC_WRITE_TO_REGISTER | TDC_REG1, SPI_CONTINUE);
	// Data:
	SPI.transfer(TDC_CS, testData, SPI_CONTINUE);
	SPI.transfer(TDC_CS, 0x80, SPI_CONTINUE);
	SPI.transfer(TDC_CS, 0x00);

	// Read back from the first 8 bits of register 1 (should be 0xAA)
	// Command:
	SPI.transfer(TDC_CS, TDC_READ_FROM_REGISTER | TDC_REG5, SPI_CONTINUE);
	// Data:
	byte commsTest = SPI.transfer(TDC_CS, 0x00);

	if (commsTest == testData) {
		return 0;
	}
	else {
		return commsTest;
	}

}

void initTDC() {

	// Send reset
	SPI.transfer(TDC_CS, TDC_RESET);

	// Wait 100ms
	delay(100);

	// Load config from EEPROM
	SPI.transfer(TDC_CS, TDC_READ_CONFIG_FROM_EEPROM);

	// Wait 500ms for load
	delay(500);
}

// the loop function runs over and over again forever
uint32_t measure() {

	// Send the INIT opcode to start waiting for a timing event
	SPI.transfer(TDC_CS, TDC_INIT);

	// Wait until interrupt goes low indicating a successful read
	long start = micros();
	while (digitalRead(TDC_INT)) {
		if (millis() - start > 500) { return 0xFFFFFFFF; } // Give up if we've been waiting 500ms
	}

	// Read result
	// The device's format is a 32 bit fixed point number with 16 bits for the 
	// fractional part. The following reads the 4 bytes MSB first and then interprets the four
	// together as a unsigned integer of 32 bits. 
	union {
		byte raw[4];
		uint32_t proc;
	} result;
	SPI.transfer(TDC_CS, TDC_READ_FROM_REGISTER | TDC_RESULT1, SPI_CONTINUE);
	result.raw[3] = SPI.transfer(TDC_CS, 0x00, SPI_CONTINUE);
	result.raw[2] = SPI.transfer(TDC_CS, 0x00, SPI_CONTINUE);
	result.raw[1] = SPI.transfer(TDC_CS, 0x00, SPI_CONTINUE);
	result.raw[0] = SPI.transfer(TDC_CS, 0x00, SPI_LAST);

	//long finish = micros();

	//float time = (float)result.proc * pow(2, -16) * 1 / 4000000; // in seconds
	//float freq = 1 / (2 * time); // in Hz (/2 because we're only sampling half a wave)

	//char statStr[20];
	//sprintf(statStr, "%g s, %g Hz, time: %i", time, freq, finish - start);
	//sprintf(statStr, "Freq: %10e Hz", freq);

	//Serial.println(statStr);

	return result.proc;
}

//byte * transferN(const byte * data, uint8_t n, byte * outputPtr) {
//
//	int dataIndex, outputIndex;
//	dataIndex = n;
//	outputIndex = 0;
//
//	do {
//		dataIndex--;
//
//		if (outputIndex)
//			outputPtr[outputIndex] = SPI.transfer(TDC_CS, data[dataIndex], (dataIndex > 0 ? SPI_CONTINUE : SPI_LAST));
//		else
//			SPI.transfer(TDC_CS, data[dataIndex], (dataIndex > 0 ? SPI_CONTINUE : SPI_LAST));
//
//		if (outputIndex) outputIndex++;
//
//	} while (dataIndex > 0);
//
//	return outputPtr;
//}
