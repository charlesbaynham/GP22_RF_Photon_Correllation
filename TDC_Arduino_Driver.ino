#define TDC_CS 10
#define TDC_INT 12

#include <math.h>
#include <SPI.h>

byte * transferN(const byte * data, uint8_t n, byte * outputPtr = 0);

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


void setup() {

	// Serial
	Serial.begin(115200);
	Serial.println("Starting");

	// Initialize the SPI connection for the GP22 TDC
	SPI.begin(TDC_CS);
	SPI.setDataMode(TDC_CS, SPI_MODE1);
	/* " The TDC-GP22 does only support the following SPI mode (Motorola specification):
	Clock Phase Bit = 1
	Clock Polarity Bit = 0 */
	SPI.setBitOrder(TDC_CS, MSBFIRST);

	// Setup the interrupt pin for input
	pinMode(TDC_INT, INPUT);

	// Wait 1s
	delay(1000);

	// Send reset
	SPI.transfer(TDC_CS, 0x50);

	// Wait 1s
	delay(1000);

	// Write 0101010101000000000000000 into register 1 (the defaults)
	//  == 0xAA8000
#define testData 0xAA
	Serial.print("Writing testdata: ");
	Serial.println(testData, HEX);
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
		Serial.println("Comms test passed");
	}
	else {
		Serial.print("ERROR in SPI test. Response was:");
		Serial.println(commsTest, HEX);
	}

	// Loading config from EEPROM
	SPI.transfer(TDC_CS, TDC_READ_CONFIG_FROM_EEPROM);

	// Wait 500ms
	delay(500);

	// Define config:
#define r0 0xF3076A00
#define r1 0x21420001
#define r2 0xA0251C02
#define r3 0x10000003
#define r4 0x10000004
#define r5 0x40000005
#define r6 0x40004006

	// Check int pin
	bool interrupt = digitalRead(TDC_INT);

	Serial.print("Interrupt is currently: ");
	Serial.println(interrupt ? "HIGH" : "LOW");
}

// the loop function runs over and over again forever
void loop() {

	// Init for start of timing
	SPI.transfer(TDC_CS, TDC_INIT);

	// Check until interrupt goes low
	long start = micros();
	while (digitalRead(TDC_INT)) {
		//if (millis() - start > 500) { return; } // Give up if we've been waiting 500ms
	}

	// Read result
	union {
		byte raw[4];
		uint32_t proc;
	} result;
	SPI.transfer(TDC_CS, TDC_READ_FROM_REGISTER | TDC_RESULT1, SPI_CONTINUE);
	result.raw[3] = SPI.transfer(TDC_CS, 0x00, SPI_CONTINUE);
	result.raw[2] = SPI.transfer(TDC_CS, 0x00, SPI_CONTINUE);
	result.raw[1] = SPI.transfer(TDC_CS, 0x00, SPI_CONTINUE);
	result.raw[0] = SPI.transfer(TDC_CS, 0x00, SPI_LAST);

	long finish = micros();

	float time = (float)result.proc * pow(2, -16) * 1 / 4000000; // in seconds
	float freq = 1 / (2 * time); // in Hz

	char statStr[20];
	sprintf(statStr, "%g s, %g Hz, time: %i", time, freq, finish - start);

	Serial.print("Status: ");
	Serial.println(statStr);

	while (!digitalRead(TDC_INT));

	delay(250);
}

byte * transferN(const byte * data, uint8_t n, byte * outputPtr) {

	int dataIndex, outputIndex;
	dataIndex = n;
	outputIndex = 0;

	do {
		dataIndex--;

		if (outputIndex)
			outputPtr[outputIndex] = SPI.transfer(TDC_CS, data[dataIndex], (dataIndex > 0 ? SPI_CONTINUE : SPI_LAST));
		else
			SPI.transfer(TDC_CS, data[dataIndex], (dataIndex > 0 ? SPI_CONTINUE : SPI_LAST));

		if (outputIndex) outputIndex++;

	} while (dataIndex > 0);

	return outputPtr;
}
