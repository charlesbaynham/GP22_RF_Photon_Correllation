/*------------------------------------

		DRIVER FOR GP22 TDC CHIP

		See README.md for more information

		Current version:			*/

#define PROG_IDN "GP22_DRIVER"
#define PROG_VER "0.3"

/*----------------------------------*/


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
#define TDC_RESET 0x50
#define TDC_START_CAL 0x04

// Is the TDC set to automatically calibrate its results?
// If so, we'll need to read 4 bytes instead of 2
bool autoCalibrate = true;

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

	// Order the GP22 to load its config
	initTDC();

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
		else if (0 == strcmp("MEAS", command)) { // Do many measurements

			// Read the number of ms to time for

			// Advance to the params (do nothing if no params given)
			if (Serial.findUntil(":", "\n")) {

				char timePeriodStr[32];
				timePeriodStr[Serial.readBytesUntil('\n', timePeriodStr, 31)] = 0;

				// Convert string -> long int
				uint32_t timePeriod = atol(timePeriodStr);

				// Calculate stop time
				uint32_t stop = millis() + timePeriod;

				// Loop until stoptime
				while (millis() < stop) {

					// Do the measurement
					uint32_t result = measure();

					// Check we didn't timeout
					if (result != 0xFFFFFFFF) {
						// Report result
						Serial.print(result);
						Serial.print('\t');
					}
				}

				// newline to terminate
				Serial.println("DONE");
			}
		}
		else if (0 == strcmp("SETU", command)) { // Setup the registers. 
			// Command: "SETUp:REG1:REG2:REG3:REG4:REG5:REG6:REG7" Registers as base 10 numbers

			// Reset the TDC
			SPI.transfer(TDC_CS, TDC_RESET);

			// The registers to be read from the serial port
			uint32_t reg[7] = { 0 };
			// Was this register read?
			bool wasRead[7] = { false };

			// Read all the params
			for (int i = 0; i < 7 && Serial.findUntil(":", "\n"); i++) { // Advance to the next ':'
				// Read the number that should follow
				reg[i] = Serial.parseInt();
				wasRead[i] = true;
			}

			// Store whether we're in calibration mode or not (bit 13 in reg 0)
			if (wasRead[0])
				autoCalibrate = (bool)(reg[0] & (1 << 13));

			// Write the values to the TDC's registers
			for (int i = 0; i < 7 && wasRead[i]; i++) {
				writeConfigReg(i, reg[i]);
			}

			delay(1);

			// Read back from the first 8 bits of register 1 (should match reg[1])
			// Command:
			SPI.transfer(TDC_CS, TDC_READ_FROM_REGISTER | TDC_REG5, SPI_CONTINUE);
			// Data:
			byte commsCheck = SPI.transfer(TDC_CS, 0x00);
			byte shouldBe = (reg[1] & 0xFF000000) >> 24;

			if (commsCheck == shouldBe) {
				Serial.print("DONE - CALIBRATION MODE ");
				if (autoCalibrate)
					Serial.print("ON - ");
				else
					Serial.print("OFF - ");

				Serial.println(commsCheck, HEX);

			}
			else {
				Serial.print("Error. Read: 0x");
				Serial.print(commsCheck, HEX);
				Serial.print(" instead of 0x");
				Serial.println(shouldBe, HEX);
			}

		}
		else if (0 == strcmp("SING", command)) { // Do a single measurement

			// Do the measurement
			uint32_t result = measure();

			// Report result
			Serial.println(result);

		}
		else if (0 == strcmp("CALI", command)) { // Calibrate the TDC and report the result

			// Do the calibration
			uint16_t calib = calibrate();

			// Report result
			Serial.println(calib);

		}
		else if (0 == strcmp("*TST", command)) { // Test connection

			// Run the test
			bool testResult = testTDC();

			// Restore the values changed during the test
			initTDC();

			// Report the result
			if (testResult) {
				Serial.println("PASSED");
			}
			else {
				Serial.print("FAILED");
			}
		}

		// Empty the serial input
		while (Serial.available()) Serial.read();

	}

}

// Write to the GP22 then read to check comms
bool testTDC() {

	// Send reset
	SPI.transfer(TDC_CS, 0x50);

	// Wait 100ms
	delay(100);

	// Write 0101010101000000000000000 into register 1 (the defaults)
	//  == 0xAA8000
#define testData 0xAA
#define reg1Config 0x8000

	writeConfigReg(TDC_REG1, ((testData << 16) | reg1Config) << 8);

	// Read back from the first 8 bits of register 1 (should be 0xAA)
	// Command:
	SPI.transfer(TDC_CS, TDC_READ_FROM_REGISTER | TDC_REG5, SPI_CONTINUE);
	// Data:
	byte commsTest = SPI.transfer(TDC_CS, 0x00);

	return (commsTest == testData);
}

// Setup the GP22 with the data stored in its EEPROM
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

// Perform a single measurement & return the outcome
uint32_t measure() {

	// Send the INIT opcode to start waiting for a timing event
	SPI.transfer(TDC_CS, TDC_INIT);

	// Wait until interrupt goes low indicating a successful read
	uint32_t start = millis();
	while (HIGH == digitalRead(TDC_INT)) {
		if (millis() - start > 500) { return 0xFFFFFFFF; } // Give up if we've been waiting 500ms
	}

	// Read result
	// The device's format is a 32 bit fixed point number with 16 bits for the 
	// fractional part. The following reads the 4 bytes MSB first and then interprets the four
	// together as a unsigned integer of 32 bits. 
	union {
		byte raw[4];
		uint32_t proc32;
		uint16_t proc16[2];
	} result;
	SPI.transfer(TDC_CS, TDC_READ_FROM_REGISTER | TDC_RESULT1, SPI_CONTINUE);
	result.raw[3] = SPI.transfer(TDC_CS, 0x00, SPI_CONTINUE);
	result.raw[2] = SPI.transfer(TDC_CS, 0x00, (autoCalibrate ? SPI_CONTINUE : SPI_LAST));
	// (autoCalibrate ? SPI_CONTINUE : SPI_LAST) means continue if autoCalibrate, stop if not

	if (!autoCalibrate) { // If the TDC isn't automatically calibrating, stop here after 16 bits
		// Check for timeout
		if (result.proc16[1] == 0xFFFF)
			return 0xFFFFFFFF;

		// Else return the result
		// N.B we're only interested in proc16[1]
		//    proc16[0] was never read into and will contain zeros / garbage
		return result.proc16[1];
	}

	// Otherwise, read 32 bits
	result.raw[1] = SPI.transfer(TDC_CS, 0x00, SPI_CONTINUE);
	result.raw[0] = SPI.transfer(TDC_CS, 0x00, SPI_LAST);

	return result.proc32;
}

// Perform a calibration routine and then return the number of LSBs in 2 clock cycles
// The default clock setting is 4 MHz, so a measurement of x LSBs in 2 clock cycles corresponds to
//	a precision of 1/4MHz * 2 cycles / x
uint16_t calibrate() {

	// Accept hits on the channels
	SPI.transfer(TDC_CS, TDC_INIT, SPI_LAST);

	delay(1);

	// Send the START_CAL_TDC opcode to start the calibration routine
	SPI.transfer(TDC_CS, TDC_START_CAL, SPI_LAST);

	delay(1);

	// Request that the ALU calculates the calibration difference by writing
	// into register 1. This tells the ALU what to calculate and also triggers the calculation
	// See p.52 of the ACAM manual
	writeConfigReg(TDC_REG1, 0x67410000);

	delay(1);

	// Read result
	// 16 bits
	union {
		byte raw[2];
		uint16_t proc;
	} calibration;
	SPI.transfer(TDC_CS, TDC_READ_FROM_REGISTER | TDC_RESULT1, SPI_CONTINUE);
	calibration.raw[1] = SPI.transfer(TDC_CS, 0x00, SPI_CONTINUE);
	calibration.raw[0] = SPI.transfer(TDC_CS, 0x00, SPI_LAST);

	delay(1);


	// Restore register 1 to normal settings (HIT1 - START, 1 hit on STOP1, 0 hits on STOP2)
	writeConfigReg(TDC_REG1, 0x01410000);

	delay(1);


	return calibration.proc;
}


// Read status
// The device's format is a 16 bit number
// See p.36 of the ACAM datasheet
uint16_t readStatus() {
	union {
		byte raw[2];
		uint16_t proc;
	} status;

	// Send command to read status
	SPI.transfer(TDC_CS, TDC_READ_FROM_REGISTER | TDC_STATUS, SPI_CONTINUE);

	// Read in the data
	status.raw[1] = SPI.transfer(TDC_CS, 0x00, SPI_CONTINUE);
	status.raw[0] = SPI.transfer(TDC_CS, 0x00, SPI_LAST);

	return status.proc;
}

// Write the given data into the target register
void writeConfigReg(uint8_t targetReg, uint32_t data) {

	// Store the 32 bit int in the same memory as 4 bytes
	union {
		uint32_t raw;
		byte bytes[4];
	} conversion;

	conversion.raw = data;

	// Command to start transfer into the given register
	SPI.transfer(TDC_CS, TDC_WRITE_TO_REGISTER | targetReg, SPI_CONTINUE);

	// The data to write
	SPI.transfer(TDC_CS, conversion.bytes[3], SPI_CONTINUE);
	SPI.transfer(TDC_CS, conversion.bytes[2], SPI_CONTINUE);
	SPI.transfer(TDC_CS, conversion.bytes[1], SPI_CONTINUE);
	SPI.transfer(TDC_CS, conversion.bytes[0], SPI_LAST);

}