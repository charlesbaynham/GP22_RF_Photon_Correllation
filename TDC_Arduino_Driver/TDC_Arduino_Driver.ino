/*------------------------------------

		DRIVER FOR GP22 TDC CHIP

		See README.md for more information

		Current version:			*/

#define PROG_IDN "GP22_DRIVER"
#define PROG_VER "0.6"
#include "latest_hash.h"

		/*----------------------------------*/


#define TDC_CS 10
#define TDC_INT A2

const double HF_CLOCK_FREQ = 4E6;
const double LF_CLOCK_FREQ = 32768;

// #define DEBUG

#include <math.h>
#include <SPI.h>

#include "CommandHandler\CommandHandler.h"

// Debugging:
#include "CommandHandler\Microprocessor_Debugging\debugging_init.h"

// Setup the GP22 with the currently stored config
void updateTDC(const uint32_t * registers);

// Number of commands to be registered
const uint8_t numCommands = 25;

// Create a command handler
CommandHandler<numCommands> handler;

// Registers all the above functions with the command handler, thus defining the commands required to call them
bool registerCommands(CommandHandler<numCommands>& h);

// Hold TDC settings and define methods to access them
#include "GP22_Register_Access\GP22_reg.h"
uint32_t GP22::registers_data[7] = { 0 };

#define TDC_WRITE_TO_REGISTER 0x80
#define TDC_READ_FROM_REGISTER 0xB0
#define TDC_REG0 0x00
#define TDC_REG1 0x01
#define TDC_REG2 0x02
#define TDC_REG3 0x03
#define TDC_REG4 0x04
#define TDC_REG5 0x05
#define TDC_REG6 0x06
#define TDC_STATUS 0x04
#define TDC_RESULT1 0x00
#define TDC_RESULT2 0x01
#define TDC_RESULT3 0x02
#define TDC_RESULT4 0x03

#define TDC_READ_CONFIG_FROM_EEPROM 0xF0
#define TDC_INIT 0x70
#define TDC_RESET 0x50
#define TDC_START_CAL 0x04
#define TDC_START_CAL_RES 0x03

enum class MEASUREMENT_ERROR {
	NO_ERROR = 0,
	TIMEOUT_START,
	TIMEOUT_STOP,
	OVERFLOW
};

// Function to reset the arduino:
void(*resetFunc) (void) = 0;

/*
 * A note on the board layout
 * ==========================
 *
 * The TDC board has three possible inputs: "TTL", "Sine" and
 * "Straight through". These three inputs are converted into TTL pulses in
 * different ways, before being fed to the GP22. (Henceforth referred to as
 * Signals)
 *
 * * The TTL input is buffered, one line goes to the GP22, one goes to a
 *   hardware counter and three are made available to the user
 * * The Sine input is converted into a square TTL wave which is fed to the GP22
 * * The Straight Through input is connected directly to the GP22, without any
 *   pre-processing
 *
 * All these three inputs go to a set of rotary switches. The position of these
 * switches decides which signal arrives at the three ports of the GP22: START,
 * STOP1 AND STOP2, henceforth referred to as Channels. Starting at the furthest
 * anticlockwise position, the three possible positions of the switches are A, B
 * and C:
 *
 * A -> TTL, B -> Straight Through, C -> Sine
 *
 * There is one switch for each GP22 Channel: starting from the bottom, the
 * order is START, STOP1, STOP2.
 *
 * It is perfectly possible to have one Signal arriving at several Channels.
 * E.g. if you intend to measure arrival times between successive photons that
 * are heralded by a TTL Signal, you may set both the START and STOP1 switches
 * to position A (= TTL). The position of switch STOP2 is unimportant in this
 * case as Channel STOP2 is not in use.
 */

void setup() {

	// Default values for TDC settings, as listed by ACAM in datasheet Note that
	// these won't result in a measurement that works: to get anything
	// interesting these defaults need to be changed, which is done by the
	// functions "setupForXXX"
	//
	// Changing these register changes the value stored in the ATMega's memory
	// only (in the array GP22::registers_data). In order to update the
	// registers actually on the TDC, you must call
	// "updateTDC(GP22::registers_data)"
	regWrite(GP22::REG0, 0x2206680C);
	regWrite(GP22::REG1, 0x5540000F);
	regWrite(GP22::REG2, 0x2000000A);
	regWrite(GP22::REG3, 0x1800000B);
	regWrite(GP22::REG4, 0x20000001);
	regWrite(GP22::REG5, 0x00000002);
	regWrite(GP22::REG6, 0x00000003);

	// Serial connection
	Serial.begin(250000);
	__serial_is_ready = true;

	// Setup the interrupt pin for input
	pinMode(TDC_INT, INPUT);

	// Setup the Chip Select pin for output
	pinMode(TDC_CS, OUTPUT);
	digitalWrite(TDC_CS, HIGH);

	// Load the possible serial commands
	if (!registerCommands(handler)) {
		Serial.println(F("Error in command registration"));
	}

	// Initialize the SPI connection for the GP22 TDC:

	SPI.begin();
	/* " The TDC-GP22 does only support the following SPI mode (Motorola specification):
	Clock Phase Bit = 1
	Clock Polarity Bit = 0 "    =>          */
	SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE1));

	// Reset the TDC
	digitalWrite(TDC_CS, LOW);
	SPI.transfer(TDC_RESET);
	digitalWrite(TDC_CS, HIGH);

	// Load all the values stored in reg into the TDC's registers
	updateTDC(GP22::registers_data);

	// Go into RF - photon mode
	setupForSTARTToSTOP1Mode();
	 
	// Setup complete. Move to waiting for a command
}

void loop() {
	if (handler.commandWaiting()) {

#ifdef DEBUG
		Serial.println(F("Command received."));
#endif

		CommandHandlerReturn out = handler.executeCommand();

		if (out != CommandHandlerReturn::NO_ERROR) {
			Serial.print(F("Error in command: CommandHandlerReturn code "));
			Serial.println((int)out);
		}
	}
}

void serialEvent() {
	while (Serial.available()) {
		char c = Serial.read();

#ifdef DEBUG
		Serial.print(F("Adding: ["));
		Serial.print(c, DEC);
		Serial.println(F("]"));
#endif

		handler.addCommandChar(c);
	}
}

void reset(const ParameterLookup& params) {
	Serial.println(F("Resetting"));
	Serial.flush();
	resetFunc();
}

void identity(const ParameterLookup& params) {
	Serial.print(F(PROG_IDN));
	Serial.print(F(" - "));
	Serial.println(F(PROG_VER));
}
void gitVersion(const ParameterLookup& params) {
	Serial.println(F(LATEST_HASH));
}

void timedMeasure(const ParameterLookup& params) {

	// Number of ms to read for
	uint32_t timePeriod = atoi(params[1]);

	// Calculate stop time
	uint32_t stop = millis() + timePeriod;

	// Loop until stoptime
	while (millis() < stop) {

		// Do the measurement
		union {
			uint32_t Whole;
			uint16_t Unsigned[2];
			int32_t Signed;
		} result;
		
		MEASUREMENT_ERROR stat = measure(result.Whole);

		// Check we didn't timeout
		if (stat == MEASUREMENT_ERROR::NO_ERROR) {
			
			// Check if calibration mode is on
			if (bitmaskRead(GP22::REG0, GP22::REG0_CALIBRATE)) {
				
				// If calibrated, convert to nanoseconds
				const double nanoseconds = double(result.Signed) / HF_CLOCK_FREQ * 1e9 / double(uint32_t(1)<<16);
				Serial.print(nanoseconds, 3);

			} else {

				// If not calibrated, output raw LSBs
				Serial.print(result.Unsigned[0]); // We only need the first 2 bytes
			}

			Serial.print('\t');
		}
	}

	// newline to terminate
	Serial.println("");
}

// Command: "SETUp REG1 REG2 REG3 REG4 REG5 REG6 REG7" Registers as base 10 numbers
void setupRegisters(const ParameterLookup& params) {

	// Reset the TDC
	digitalWrite(TDC_CS, LOW);
	SPI.transfer(TDC_RESET);
	digitalWrite(TDC_CS, HIGH);

	// Iterate over param list and reg list
	for (int i = 1; i < params.size() && i <= sizeof(GP22::registers_data) / sizeof(GP22::registers_data[0]); i++) {

		// Get the next String, convert to a long and save in the appropriate register
		uint32_t newRegVal = strtoul(params[i], NULL, 0);
		GP22::regWrite(GP22::registers(i - 1), newRegVal);

#ifdef DEBUG
		Serial.print(F("Setting REG "));
		Serial.print(i - 1);
		Serial.println(F(": "));
		Serial.print(newRegVal);
		Serial.print(F(" = 0x"));
		Serial.print(newRegVal, HEX);
		Serial.print(F(", parsed from : "));
		Serial.println(params[i]);
#endif
	}

	// Write the values to the TDC's registers
	updateTDC(GP22::registers_data);

	delay(1);

	// Read back from the Most Significant 8 bits of register 1 (should match GP22::registers_data[1])
	// Command:
	digitalWrite(TDC_CS, LOW);
	SPI.transfer(TDC_READ_FROM_REGISTER | TDC_REG5);
	// Data:
	byte commsCheck = SPI.transfer(0x00);
	digitalWrite(TDC_CS, HIGH);

	byte shouldBe = (regRead(GP22::REG1) & 0xFF000000) >> 24;

	if (commsCheck == shouldBe) {
		Serial.print("DONE - CALIBRATION MODE ");
		if ((bool)bitmaskRead(GP22::REG0, GP22::REG0_CALIBRATE))
			Serial.print("ON - ");
		else
			Serial.print("OFF - ");

		Serial.println(commsCheck, HEX);

	}
	else {
		Serial.print("Error. Read: 0x");
		Serial.print(commsCheck, HEX);
		Serial.print(" instead of 0x");
		Serial.print(shouldBe, HEX);
		Serial.print(" from reg[1] == ");
		Serial.println(regRead(GP22::REG1), HEX);

	}
}

// Enable / disable measurement mode 2 Measurement mode 2 allows for much longer
// time periods to be measured, but requires the use of calibration. This means
// that extremely fast readout is not possible. 
// 
// Params - <"1" or "2"> sets mode 1 or 2
void setMeasMode(const ParameterLookup& params) {
	char paramChar = params[1][0];

	if (paramChar == '1') {
		setMeasurementMode2(false);
		Serial.println(F("MODE 1"));
	}
	else if (paramChar == '2') {
		setMeasurementMode2(true);
		Serial.println(F("MODE 2"));
	}
	else {
		Serial.println(F("Invalid option: options are '1' or '2'"));
	}
}

void getMeasMode(const ParameterLookup& params) {
	if (bitmaskRead(GP22::REG0, GP22::REG0_MESSB2)) {
		Serial.println(F("MODE 2"));
	} else {
		Serial.println(F("MODE 1"));
	}
}

void getRegisters(const ParameterLookup& params) {

	// Output the current register state
	for (int i = 0; i < 7; i++) {
		Serial.print("0x");
		Serial.print(regRead(GP22::registers(i)), HEX);
		Serial.print('\t');
	}

	Serial.println("");

}

// void RFMode(const ParameterLookup& params) {
// 	setupForRFMode();
// 	Serial.println(F("RF - RF MODE"));
// }
// void photonRFMode(const ParameterLookup& params) {
// 	setupForRF_PhotonMode();
// 	Serial.println(F("RF - PHOTON MODE"));
// }
// void photonPhotonMode(const ParameterLookup& params) {
// 	setupForPhotonPhotonMode();
// 	Serial.println(F("PHOTON - PHOTON MODE"));
// }
// void ORTECMode(const ParameterLookup& params) {
// 	setupForOrtecMode();
// 	Serial.println(F("ORTEC MODE"));
// }
// 
void start_stop1_x1(const ParameterLookup& params) {
	setupForSTARTToSTOP1Mode();
	Serial.println(F("START - STOP1 MODE"));
}

void singleMeasure(const ParameterLookup& params) {

	// Do the measurement
	
	union {
		uint32_t Whole;
		uint16_t Unsigned[2];
		int32_t Signed;
	} result;

	MEASUREMENT_ERROR stat = measure(result.Whole);

	// If we get an error level, report the timeout
	if (stat != MEASUREMENT_ERROR::NO_ERROR) {
		Serial.print(F("MEASUREMENT ERROR "));
		Serial.println((int)stat);
		return;
	}

	if (bitmaskRead(GP22::REG0, GP22::REG0_CALIBRATE)) {
		
		// If calibrated, convert to nanoseconds
		const double nanoseconds = double(result.Signed) / HF_CLOCK_FREQ * 1e9 / double(uint32_t(1)<<16);
		Serial.print(nanoseconds, 3);
		Serial.println("ns");

	} else {

		// If not calibrated, output raw LSBs
		Serial.println(result.Unsigned[0]); // We only need the first 2 bytes
	}

	// DEBUG:
	// Output all registers
	// for (int i = 0; i<4; i++) {
		
	// 	uint32_t r = read_bytes(i, false);

	// 	Serial.print("0x");
	// 	Serial.println(r);
	// }

}

// Take multiple measurements for a given time period, outputting the results as a histogram
// Histogram bins will be evenly spaced from 0 to <max val>. 
// Any results equal to or greater than <max val> will be discarded
// Params - 
//		<time period> in ms
//		<num bins>
//		<min val> in ns
//		<max val> in ns
void histogramMeasure(const ParameterLookup& params) {

	// Num ms to read for
	const unsigned long timePeriod = strtoul(params[1], NULL, 0);

	// Number of bins in histogram
	const size_t numBins = atoi(params[2]);

	// Maximum and minimum values that will fit into the histogram
	// If calibration mode is on, these values should be in nanoseconds
	// Otherwise, they are an integer number of gates passed by the signal
	const double minVal = atof(params[3]);
	const double maxVal = atof(params[4]);

	if (numBins < 1) {
		Serial.println(F("Error: numBins must be > 0"));
		return;
	}

	// Reserve memory for an array to hold the histogram
	typedef unsigned int histType;
	histType * hist = (histType *)calloc(numBins, sizeof(histType));

	// If calloc failed, quit
	if (NULL == hist) {
		Serial.print(F("Mem allocation of "));
		Serial.print(numBins * sizeof(histType));
		Serial.println(F(" failed"));
		return;
	}

	CONSOLE_LOG(F("histogramMeasure::"));
	CONSOLE_LOG(numBins * sizeof(histType));
	CONSOLE_LOG_LN(F(" bytes allocated for histogram"));

	uint32_t minValFixedPoint, maxValFixedPoint;
	double minValFixedPoint_D, maxValFixedPoint_D;

	if (bitmaskRead(GP22::REG0, GP22::REG0_CALIBRATE)) {
		// The measurements returned by the TDC are (in calibrated mode) 32 bit fixed point numbers
		// These represent multiples of the HF clock, where the first 16 bits are fractional
		// So 0x00010000 is 1x the ref clock period, usually 4MHz. 
		// We won't do this scaling during the histogram measurement since it takes too long, 
		// instead we'll just scale the maxVal in the opposite way.
		// 
		// First, scale to multiples of the ref clock
		// Then, multiply by 2^16 due to the 16-16 fixed point format
		// And convert to unsigned integer
		maxValFixedPoint_D = maxVal * 1e-9 * HF_CLOCK_FREQ * double( uint32_t(1)<<16 );
		minValFixedPoint_D = minVal * 1e-9 * HF_CLOCK_FREQ * double( uint32_t(1)<<16 );

		if (minValFixedPoint > 0x7FFFFFFF || maxValFixedPoint_D > 0x7FFFFFFF) {
			Serial.println(F("Error: limits too large"));
			free(hist);
			return;
		}

		maxValFixedPoint = maxValFixedPoint_D;
		minValFixedPoint = minValFixedPoint_D;
	} else {
		// In uncalbrated mode, they will be signed integers representing the number of
		// gates passed by the signal (~90ps in single res mode)
		// They can in general be negative, but our histogram function does not support
		// this for now
		maxValFixedPoint = maxVal;
		minValFixedPoint = minVal;
	}

	const int32_t range = maxValFixedPoint - minValFixedPoint;

	// Calculate stop time
	uint32_t stop = millis() + timePeriod;

	/**
	 * START PERFORMANCE CRITICAL SECTION
	 * 
	 * The following code must be efficient as it will be the bottleneck in 
	 * the data gathering rate
	 * 
	 * Max time for RF edge ~ 100ns
	 * Avg. time for photon arrival, assuming 10k photons per second ~ 100us
	 * Time to call getHistIndex ~ 60us
	 * Comparison and increment ~ 23 clock cycles = 1.5us
	 * 
	 * Implies a potential 30% saving if I can have TDC working in parallel
	 * with microprocessor stuff
	 */

	// Loop until stoptime
	while (millis() < stop) {

		CONSOLE_LOG_LN(F("Start loop"));

		union {
			uint32_t Unsigned;
			int32_t Signed;
			int16_t Signed16[2];
		} result;

		// Do a measurement and store it in `result`
		if (MEASUREMENT_ERROR::NO_ERROR == measure(result.Unsigned)) {

			// We didn't timeout
			CONSOLE_LOG(F("No timeout, reading = "));
			CONSOLE_LOG_LN(result.Signed);

			// Work out to which bin this reading belongs
			size_t histIndex;
			if (bitmaskRead(GP22::REG0, GP22::REG0_CALIBRATE)) {
				histIndex = getHistIndex(numBins, minValFixedPoint, range, result.Signed);
			} else {
				// In uncalibrated mode, only the first 16 bits have meaning
				histIndex = getHistIndex(numBins, minValFixedPoint, range, result.Signed16[0]);
			}

			CONSOLE_LOG(F("histIndex = "));
			CONSOLE_LOG_LN(histIndex);

			// Increment that bin if it exists
			if (histIndex < numBins)
			{
				CONSOLE_LOG(histIndex);
				CONSOLE_LOG_LN(F(" incremented"));
				hist[histIndex]++;

				if (hist[histIndex] == 0) {
					// We overflowed. Whoops!
					hist[histIndex]--;
					CONSOLE_LOG(histIndex);
					CONSOLE_LOG_LN(F(" overflow!"));
				}
			}
		}
		else {
			CONSOLE_LOG_LN(F("Timeout"));
		}
	}

	/**
	 * END PERFORMANCE CRITICAL SECTION
	 */

	CONSOLE_LOG_LN(F("Loop done"));

	// Return the histogram
	for (size_t i = 0; i < numBins-1; i++) {
		Serial.print(hist[i]);
		Serial.print('\t');
	}
	Serial.println(hist[numBins-1]); // No '\t' on final one, but a newline instead

	CONSOLE_LOG_LN(F("Freeing histogram memory"));

	// Free the histogram memory
	free(hist);

	return;
}

// Calculate which bin in a histogram a number belongs to
// Inputs - 
//		<num bins>
//		<min val>
//		<max val>
//		<number>
// Returns the index of the correct bin. Return value can be greater than num_bins so beware overflows!
// 
// This function has been timed with "testhist" command: 
// calling with 500 bins results in an average execution time of 60us
inline size_t getHistIndex(const size_t numBins, const int32_t minVal, const int32_t maxMinusMin, const int32_t reading) {

	const double scaled = double(reading-minVal) * double(numBins) / double(maxMinusMin);

	return (size_t)floor(scaled);

}

// Calibrate the TDC against the reference 4MHz clock and report the result
void calibrateTDC(const ParameterLookup& params) {

	// Do the calibration
	uint16_t calib = calibrate();

	// Report result
	Serial.println(calib);

}

// Calibrate the highspeed clock against the low speed clock and report the result
// (i.e. number of high speed clock cycles in `ANZ_PER_CALRES` cycles of the ref clock)
void calibrateResonator(const ParameterLookup& params) {

	// Do the calibration
	uint32_t calib = calibrateHF();

	// Report result
	if (calib == 0xFFFFFFFF)
		Serial.println(F("TIMEOUT"));
	else
		Serial.println(calib);

}

void testConnection(const ParameterLookup& params) {

	// Run the test
	uint8_t testResult = testTDC();

	// Report the result
	if (testResult) {
		Serial.print("FAILED with code returned 0x");
		Serial.println(testResult, HEX);
	}
	else {
		Serial.println("PASSED");
	}
}

void getStatus(const ParameterLookup& params) {

	Serial.print(F("0b"));
	Serial.println(readStatus(), BIN);

}

// ******* END OF COMMANDS ******* //

// ******* START OF INTERNAL FUNCTIONS ******* //

// Write to the GP22 then read to check comms
uint8_t testTDC() {

	// Send reset
	digitalWrite(TDC_CS, LOW);
	SPI.transfer(TDC_RESET);
	digitalWrite(TDC_CS, HIGH);

	// Wait 100ms
	delay(100);

	// Get previous value of reg1's top 8 bits
	uint8_t prevReg1 = GP22::bitmaskRead(GP22::REG1, GP22::REG1_TEST_DATA);

	// Write test data (0xAB) into reg1
	const uint8_t testdata = 0xAB;
	GP22::bitmaskWrite(GP22::REG1, GP22::REG1_TEST_DATA, testdata);
	updateTDC(GP22::registers_data);

	// Read back from the first 8 bits of register 1 (should match testdata)
	// Command:
	digitalWrite(TDC_CS, LOW);
	SPI.transfer(TDC_READ_FROM_REGISTER | TDC_REG5);
	// Data:
	uint8_t commsTest = SPI.transfer(0x00);
	digitalWrite(TDC_CS, HIGH);


	// Restore settings to previous
	GP22::bitmaskWrite(GP22::REG1, GP22::REG1_TEST_DATA, prevReg1);
	updateTDC(GP22::registers_data);

	// Return 0 for success
	if (commsTest == testdata)
		return 0;

	// If we failed, return the value we just read, unless that value is 0 in which case return "0xFF"
	else
		if (commsTest != 0)
			return commsTest;
		else
			return 0xFF;
}

// Setup the GP22 with the currently stored config
// registers should be a 7 element array
void updateTDC(const uint32_t * registers) {

	// Write the values to the TDC's registers
	for (int i = 0; i < 7; i++) {
		writeConfigReg(i, registers[i]);
	}

}

// Perform a single measurement & output to the passed variable
// The uint32_t readings returned by this function are taken directly from the GP22's
// output registers. They represent either a raw, signed integer number of gates passed for a measurement,
// or a calibrated floating point time based on the internal clock, depending on the state
// of the GP22's registers.
// This function does not care: it returns the value as an unsigned 32 bit unsigned int. It is 
// up to the user to convert into whatever format is applicable.
//
// Return error code for measurement status
MEASUREMENT_ERROR measure(uint32_t& out) {

	// Send the INIT opcode to start waiting for a timing event
	digitalWrite(TDC_CS, LOW);
	SPI.transfer(TDC_INIT);
	digitalWrite(TDC_CS, HIGH);

	// Wait until interrupt goes low indicating a successful read
	uint32_t start = millis();
	while (HIGH == digitalRead(TDC_INT)) {
		// Give up if we've been waiting 500ms
		if (millis() - start > 500) {
			return MEASUREMENT_ERROR::TIMEOUT_START;
		} 
	}

	// Read the result
	out = read_bytes(TDC_RESULT1, !(bool)bitmaskRead(GP22::REG0, GP22::REG0_CALIBRATE));

	// Get the status
	uint16_t TDC_stat = readStatus();

	// const int nextAdr = TDC_stat & 0b111;
	const bool stopTimeout = TDC_stat & 0b11000000000;
	const bool counterOverflow = out == 0xFFFFFFFF;

	if (stopTimeout)
		return MEASUREMENT_ERROR::TIMEOUT_STOP;
	if (counterOverflow)
		return MEASUREMENT_ERROR::OVERFLOW;

	return MEASUREMENT_ERROR::NO_ERROR;
}

// Perform a calibration routine and then return the number of LSBs in 2 clock cycles
// The default clock setting is 4 MHz, so a measurement of x LSBs in 2 clock cycles corresponds to
//	a precision of 1/4MHz * 2 cycles / x
uint16_t calibrate() {

	// This sequence is adapted from the ACAM eval software source code (in Labview)

	// Backup registers
	uint32_t backup[7];
	for (int i = 0; i < 7; i++) {
		backup[i] = regRead(GP22::registers(i));
	}

	// Goto single res. mode
	//writeConfigReg(TDC_REG6, reg6 & 0xFFFFCFFF);

	// Goto double res. mode
	//writeConfigReg(TDC_REG6, (reg6 & 0xFFFFCFFF) | 0x1000);

	// Goto quad res. mode
	GP22::bitmaskWrite(GP22::REG0, GP22::REG0_MESSB2, true); // Mes. mode. 2
	GP22::bitmaskWrite(GP22::REG0, GP22::REG0_CALIBRATE, false); // No auto cal
	GP22::bitmaskWrite(GP22::REG6, GP22::REG6_QUAD_RES, true); // Quad res on
	GP22::bitmaskWrite(GP22::REG6, GP22::REG6_DOUBLE_RES, false); // Double res off
	updateTDC(GP22::registers_data);

	// Send INIT so that the TDC is ready to give a response
	digitalWrite(TDC_CS, LOW);
	SPI.transfer(TDC_INIT);
	digitalWrite(TDC_CS, HIGH);

	// Send the START_CAL_TDC opcode to measure the calibration data
	digitalWrite(TDC_CS, LOW);
	SPI.transfer(TDC_START_CAL);
	digitalWrite(TDC_CS, HIGH);

	// Request that the ALU calculates the calibration difference by writing
	// into register 1. This tells the ALU what to calculate and also triggers the calculation
	// See p.52 of the ACAM manual
	// Our calculation is CALI2 - CALI1 == T_ref
	GP22::bitmaskWrite(GP22::REG1, GP22::REG1_HIT1, 7); // Request Cal2...
	GP22::bitmaskWrite(GP22::REG1, GP22::REG1_HIT2, 6); // ...minus Cal1

	GP22::bitmaskWrite(GP22::REG1, GP22::REG1_HITIN1, 0); // Expect 0 hits
	GP22::bitmaskWrite(GP22::REG1, GP22::REG1_HITIN2, 0);

	writeConfigReg(GP22::REG1, regRead(GP22::REG1)); // Go!
	delay(1);

	// Read ALU_PTR from the status and subtract 1 to get the location of the most recently written
	//   measurement (the calibration)
	uint8_t storageLocation = (readStatus() & 0x7) - 1;

	// Read result
	uint16_t calibration;

	// Check that we actually took a measurement. If ALU_PTR was 0, ALU_PTR-1 == 0xFF and we failed
	if (storageLocation == 0xFF) { calibration = 0xFFFF; } // Return error
	else { // Read and return data
		calibration = read_bytes(storageLocation, true);
	}

	// Restore registers
	for (int i = 0; i < 7; i++) {
		regWrite(GP22::registers(i), backup[i]);
	}
	updateTDC(GP22::registers_data);

	return calibration;
}

uint32_t calibrateHF() {

	// Backup registers
	uint32_t backupReg3;
	backupReg3 = regRead(GP22::REG3);

	// Set EN_AUTOCALC=0
	GP22::bitmaskWrite(GP22::REG3, GP22::REG3_EN_AUTOCALC_MB2, false);
	writeConfigReg(GP22::REG3, regRead(GP22::REG3));
	updateTDC(GP22::registers_data);

	// Init
	digitalWrite(TDC_CS, LOW);
	SPI.transfer(TDC_INIT);
	digitalWrite(TDC_CS, HIGH);

	// Start the calibration
	digitalWrite(TDC_CS, LOW);
	SPI.transfer(TDC_START_CAL_RES);
	digitalWrite(TDC_CS, HIGH);

	// Wait until interrupt goes low indicating a successful read
	uint32_t start = millis();
	bool timeout = false;
	while (HIGH == digitalRead(TDC_INT)) {
		if (millis() - start > 500) { timeout=true; break; } // Give up if we've been waiting 500ms
	}

	// The time interval to be measured is set by ANZ_PER_CALRES
	// which defines the number of periods of the 32.768 kHz clock:
	// 2 periods = 61.03515625 us
	// But labview / the user will handle this, we just output the raw data
	uint32_t result = timeout ? 0xFFFFFFFF : read_bytes(TDC_RESULT1, false);

	// Restore reg3
	writeConfigReg(GP22::REG3, backupReg3);
	updateTDC(GP22::registers_data);

	return result;
}

// Read status
// The device's format is a 16 bit number
// See p.36 of the ACAM datasheet
uint16_t readStatus() {

	// Read in the data
	uint16_t status = read_bytes(TDC_STATUS, true);

	return status;
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
	digitalWrite(TDC_CS, LOW);
	SPI.transfer(TDC_WRITE_TO_REGISTER | targetReg);

	// The data to write
	SPI.transfer(conversion.bytes[3]);
	SPI.transfer(conversion.bytes[2]);
	SPI.transfer(conversion.bytes[1]);
	SPI.transfer(conversion.bytes[0]);
	digitalWrite(TDC_CS, HIGH);

}

// Read result
// The device's format is a 32 bit fixed point number with 16 bits for the 
// fractional part. The following reads either 16 or 32 bits, MSBs first. 
// If it reads 16, the return value will be 0x0000RRRR where RRRR is the result bytes
// If the 16 bits are 0xFFFF, the return value will be 0xFFFFFFFF
uint32_t read_bytes(uint8_t reg, bool read16bits) {

	union {
		byte raw[4];
		uint32_t proc32;
		uint16_t proc16[2];
	} result;

	// Select slave
	digitalWrite(TDC_CS, LOW);

	SPI.transfer(TDC_READ_FROM_REGISTER | reg);

	result.raw[3] = SPI.transfer(0x00);
	result.raw[2] = SPI.transfer(0x00);

	if (read16bits) { // If only reading 16 bits, stop here

		// Deselect slave
		digitalWrite(TDC_CS, HIGH);

		// Check for timeout
		if (result.proc16[1] == 0xFFFF)
			return 0xFFFFFFFF;

		// Else return the result
		// N.B we're only interested in proc16[1]
		//    proc16[0] was never read into and will contain zeros / garbage
		return result.proc16[1];
	}

	// Otherwise, read all 32 bits
	result.raw[1] = SPI.transfer(0x00);
	result.raw[0] = SPI.transfer(0x00);

	// Deselect slave
	digitalWrite(TDC_CS, HIGH);

	return result.proc32;
}

// this function will return the number of bytes currently free in RAM
// written by David A. Mellis
// based on code by Rob Faludi http://www.faludi.com
void availableMemory(const ParameterLookup& params) {
	//int size = 1024; // Use 2048 with ATmega328
	int size = 2048;
	byte *buf;

	while ((buf = (byte *)malloc(--size)) == NULL)
		;

	free(buf);

	Serial.print(size);
	Serial.println(F(" bytes remain"));
}

// Run the getHistIndex function many times to work out its performance
void testHistFunc(const ParameterLookup& params) {
	
	// Get parameters
	const size_t arraySize = strtol(params[1], 0, 0);
	const size_t numBins = strtol(params[2], 0, 0);
	const uint32_t maxVal = RAND_MAX;
	const size_t numLoops = strtol(params[3], 0, 0);

	// Allocate space for data
	uint32_t * vals = (uint32_t*)malloc(arraySize * sizeof(uint32_t));
	if (vals == 0) {
		Serial.println(F("Mem alloc failed"));
		return;
	}

	long totalTime = 0;
  double timeVar = 0;

	for (int loop = 0; loop < numLoops; loop++) {
		// Generate fake data
		for (int i = 0; i < arraySize; i++) {
			vals[i] = rand();
		}

		// Start the clock!
		long timerStart = micros();

    // Run the histogram function, throwing away its return value
    // "volatile" here tells the compiler not to do any funny business 
    // like optimising away the function call completely
		for (int i = 0; i < arraySize; i++) {
			volatile size_t throwAway = getHistIndex(numBins, 0, maxVal, vals[i]);
		}

		// Stop the clock
		long timerEnd = micros();

		long thisCycleTime = timerEnd - timerStart;

    // Store time
		totalTime += thisCycleTime;

    // Store varience
    timeVar += thisCycleTime*thisCycleTime;
   
	}

	const double timePerCycle = double(totalTime) / double(numLoops);
	const double timePerCall = timePerCycle / double(arraySize);  
  
  timeVar = timeVar - numLoops * timePerCycle*timePerCycle;

  const double loopStdDev = sqrt(timeVar);

	Serial.print(F("Time taken per call (us): "));
	Serial.print(timePerCall);

  Serial.print(F("  = "));
  Serial.print(timePerCall / 1E6 * F_CPU);
  Serial.print(F(" clock cycles @ "));
  Serial.print(F_CPU);
  Serial.println(F(" Hz"));

  Serial.print(F("Frac. std.dev. of loop times : "));
  Serial.println(loopStdDev / timePerCycle);

	free(vals);
}

void setAutoCal(const ParameterLookup& params) {

	bool setCal = strtol(params[1], NULL, 0);

	if (bitmaskRead(GP22::REG0, GP22::REG0_MESSB2)) {
		Serial.println(F("Calibration mode can only be turned off in measurement mode 1"));
		return;
	}

	GP22::bitmaskWrite(GP22::REG0, GP22::REG0_CALIBRATE, 	setCal);
	GP22::bitmaskWrite(GP22::REG0, GP22::REG0_NO_CAL_AUTO, !setCal);

	Serial.print(F("Calibration mode "));
	if (setCal) {
		// Auto calibrate on
		Serial.println(F("on"));
	} else {
		// Auto calibrate off
		Serial.println(F("off"));
	}

	updateTDC(GP22::registers_data);
}

void setClockDivision(const ParameterLookup& params) {
	
	int division = strtol(params[1], NULL, 0);

	unsigned int setting;

	if (division == 1)
		setting = 0x00;
	else if (division == 2)
		setting = 0x01;
	else if (division == 4)
		setting = 0x02;
	else
	{
		// Invalid setting. Error!
		Serial.println(F("Invalid setting! Choose 1, 2 or 4"));
		return;
	}

	// Set the register bit appropriately
	bitmaskWrite(GP22::REG0, GP22::REG0_DIV_CLKHS, setting);

	// Write to device
	updateTDC(GP22::registers_data);
}

void getClockDivision(const ParameterLookup&) {
	
	// Get the register bits
	uint8_t setting = bitmaskRead(GP22::REG0, GP22::REG0_DIV_CLKHS);

	int division;

	if (setting == 0x00)
		division = 1;
	else if (setting == 0x01)
		division = 2;
	else if (setting == 0x02 || setting == 0x03)
		division = 4;
	else
	{
		// Invalid setting. Error!
		Serial.println(F("Invalid setting! Board in strange config, reset recommended"));
		return;
	}

	Serial.println(division);
}

bool registerCommands(CommandHandler<numCommands>& h) {
	// N.B. commands are not case sensitive

	h.registerCommand(COMMANDHANDLER_HASH("*IDN"), 0, &identity);
	h.registerCommand(COMMANDHANDLER_HASH("*IDN?"), 0, &identity);
	h.registerCommand(COMMANDHANDLER_HASH("*GIT"), 0, &gitVersion);
	h.registerCommand(COMMANDHANDLER_HASH("*GIT?"), 0, &gitVersion);
	h.registerCommand(COMMANDHANDLER_HASH("*TST"), 0, &testConnection);
	h.registerCommand(COMMANDHANDLER_HASH("*TST?"), 0, &testConnection);
	h.registerCommand(COMMANDHANDLER_HASH("*RST"), 0, &reset);
	h.registerCommand(COMMANDHANDLER_HASH("MEAS"), 1, &timedMeasure);
	h.registerCommand(COMMANDHANDLER_HASH("SING"), 0, &singleMeasure);
	h.registerCommand(COMMANDHANDLER_HASH("HIST"), 4, &histogramMeasure);
	h.registerCommand(COMMANDHANDLER_HASH("STAT"), 0, &getStatus);
	h.registerCommand(COMMANDHANDLER_HASH("STAT?"), 0, &getStatus);
	h.registerCommand(COMMANDHANDLER_HASH("SETU"), -1, &setupRegisters);
	h.registerCommand(COMMANDHANDLER_HASH("SETU?"), 0, &getRegisters);
	h.registerCommand(COMMANDHANDLER_HASH("*MEM"), 0, &availableMemory);
	h.registerCommand(COMMANDHANDLER_HASH("HCAL"), 0, &calibrateResonator);
	h.registerCommand(COMMANDHANDLER_HASH("CALI"), 0, &calibrateTDC);
	h.registerCommand(COMMANDHANDLER_HASH("NORM1X"), 0, &start_stop1_x1);
	h.registerCommand(COMMANDHANDLER_HASH("MODE"), 1, &setMeasMode);
	h.registerCommand(COMMANDHANDLER_HASH("MODE?"), 0, &getMeasMode);
	h.registerCommand(COMMANDHANDLER_HASH("AUTOCAL"), 1, &setAutoCal);
	h.registerCommand(COMMANDHANDLER_HASH("DIVI"), 1, &setClockDivision);
	h.registerCommand(COMMANDHANDLER_HASH("DIVI?"), 0, &getClockDivision);

	CommandHandlerReturn finalError = h.registerCommand(COMMANDHANDLER_HASH("testhist"), 3, &testHistFunc);

	return finalError == CommandHandlerReturn::NO_ERROR;
}

void setupForSTARTToSTOP1Mode() {

	// Read time from trigger on START until trigger on STOP1
	bitmaskWrite(GP22::REG1, GP22::REG1_HIT1, 1);
	bitmaskWrite(GP22::REG1, GP22::REG1_HIT2, 0);
	bitmaskWrite(GP22::REG1, GP22::REG1_HITIN1, 2); // 1 hit on STOP1 + 1 on START = 2
	bitmaskWrite(GP22::REG1, GP22::REG1_HITIN2, 0); // No hits on STOP2

	// Trigger interrupt on timeout or finishing calculation
	bitmaskWrite(GP22::REG2, GP22::REG2_EN_INT_ALU, 1);
	bitmaskWrite(GP22::REG2, GP22::REG2_EN_INT_TDC_TIMEOUT, 1);

	// Double res mode: fine since we don't need channel 2
	bitmaskWrite(GP22::REG6, GP22::REG6_DOUBLE_RES, true);
	bitmaskWrite(GP22::REG6, GP22::REG6_QUAD_RES, false);

	// Measurement mode will be determined by the user.
	// Call setMeasurementMode2(bool enable) to change

	// Update the TDC's settings
	updateTDC(GP22::registers_data);
}

void setMeasurementMode2(bool enabled) {
	
	// Enable / disable measurement mode 2
	bitmaskWrite(GP22::REG0, GP22::REG0_MESSB2, enabled);

	if (enabled) {
		// Calibration must be on in mode 2
		// We'll also turn on autocalibration (calibrates after every shot)
		bitmaskWrite(GP22::REG0, GP22::REG0_CALIBRATE, 	true);
		bitmaskWrite(GP22::REG0, GP22::REG0_NO_CAL_AUTO, false);
	}

	// Send settings
	updateTDC(GP22::registers_data);

}
