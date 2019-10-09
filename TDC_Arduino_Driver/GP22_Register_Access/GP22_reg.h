#pragma once

/*
	TYPESAFE REGISTER ACCESS

	This file allows for safe access and modification of a set of registers, typically
	used to control a device. Use of this code has zero overhead compared with manually
	typing out bit-by-bit access, but is safer and easier. 

	This particular customisation contains settings for control of the GP22 TDC chip. 

	Usage:

		Initialisation - 
			Include this file and define `uint32_t GP22::registers_data[7]` in your code.
			e.g.
				#include "GP22_reg.h"

				uint32_t GP22::registers_data[7] = {0};

		regWrite - Overwrites an entire register.
		e.g.
			regWrite(GP22::REG0, 0x12345678);

		regRead - Reads from an entire register
		e.g.
			uint32_t reg2Contents = regRead(GP22::REG2);

		bitmaskWrite - Overwrites the specified bits in the given register
		e.g.
			bitmaskWrite(GP22::REG0, GP22::REG0_ID0, 0xab);

		Warning: it is possible to give the wrong mask to this function without
		triggering a compiler error. e.g. 
			bitmaskWrite(GP22::REG2, GP22::REG0_ID0, 0xab);
		would compile fine, even though mask REG0_ID0 does not refer to register REG0. 

		bitmaskRead - Reads the specified bits from the given register
		e.g.
			uint8_t IDBits = bitmaskRead(GP22::REG0, GP22::REG0_ID0);
			// Gives IDBits = 0xab after previous call

	Customisation / copying:
		1. Copy this file and rename
		2. Change the namespace to an appropriate value
		3. Define the register size using the `T_register` typedef
		4a. Update the `registers` enum to hold the correct number of register labels
    4b. Update the `extern T_register registers_data[<NUM_REGISTERS>];` definition to match the 
      number of registers defined in 4a.
		5. Update the bitset enums: one for each register, defining the location and names
			of available settings.
		6. Include this file in your code and initialise the data before calling any of the functions.
			e.g.
				T_register MY_NAMESPACE::registers_data[<NUM_REGISTERS>];
		7. Call the functions as described above in order to read / write to the registers. 

*/

// Change the namespace title for other applications
namespace GP22 {

	// This defines the size of a register
	// E.g. for a 32 bit register, use 
	// 		typedef uint32_t T_register;
	typedef uint32_t T_register;

	// This is the location of the data. It is declared "extern" here, meaning that it must be defined
	// and initialised elsewhere. 
	//
	// On initialisation, ensure that the size matches the size of `registers` or there will
	// be buffer overflows
	extern T_register registers_data[7];

	// The registers
	// The number of labels here must match the size of `registers_data`
	enum registers
	{
		REG0  = 0x00,
		REG1  = 0x01,
		REG2  = 0x02,
		REG3  = 0x03,
		REG4  = 0x04,
		REG5  = 0x05,
		REG6  = 0x06
	};

	// A macro that defines encodes values for a bitset
	// This encodes the start and stop positions of a bitset for a given register into
	// the first two and last two bytes of a 32 bit int respectively
	// See below for usage
	#define REG_BIT_DEFN(start, end) ((uint32_t(start)<<16)|(end-start+1))

	// The settings
	//
	// Use the `REG_BIT_DEFN` macro to define the start and stop positions of all the settings
	// available on your device
	//
	// It is important that the names of these enum values contain the register to which they
	// relate. This is the only protection against erroneously matching register x's bitmask with
	// register y, as this will not throw a compile error. The enums below are grouped by register
	// for readability, but the code would work equally well if they were instead lumped into one
	// large enum. 
	//
	// Once defined here, the bitset functions can be called with these values. E.g.
	// 		bitmaskWrite(GP22:REG0, GP22:REG0_DIV_FIRE, 0b1011);
	enum {
	    REG0_ID0                = REG_BIT_DEFN(0, 7),
	    REG0_NEG_START          = REG_BIT_DEFN(8, 8),
	    REG0_NEG_STOP1          = REG_BIT_DEFN(9, 9),
	    REG0_NEG_STOP2          = REG_BIT_DEFN(10, 10),
	    REG0_MESSB2             = REG_BIT_DEFN(11, 11),
	    REG0_NO_CAL_AUTO        = REG_BIT_DEFN(12, 12),
	    REG0_CALIBRATE          = REG_BIT_DEFN(13, 13),
	    REG0_SEL_ECLK_TMP       = REG_BIT_DEFN(14, 14),
	    REG0_ANZ_FAKE           = REG_BIT_DEFN(15, 15),
	    REG0_TCYCLE             = REG_BIT_DEFN(16, 16),
	    REG0_ANZ_PORT           = REG_BIT_DEFN(17, 17),
	    REG0_START_CLKHS_START  = REG_BIT_DEFN(18, 19),
	    REG0_DIV_CLKHS          = REG_BIT_DEFN(20, 21),
	    REG0_ANZ_PER_CALRES     = REG_BIT_DEFN(22, 23),
	    REG0_DIV_FIRE           = REG_BIT_DEFN(24, 27),
	    REG0_ANZ_FIRE_LSB       = REG_BIT_DEFN(28, 31)
	};

	enum {
	    REG1_ID1                = REG_BIT_DEFN(0, 7),
	    REG1_SEL_TSTO1          = REG_BIT_DEFN(8, 10),
	    REG1_SEL_TSTO2          = REG_BIT_DEFN(11, 13),
	    REG1_SEL_START_FIRE     = REG_BIT_DEFN(14, 14),
	    REG1_CURR32K            = REG_BIT_DEFN(15, 15),
	    REG1_HITIN1             = REG_BIT_DEFN(16, 18),
	    REG1_HITIN2             = REG_BIT_DEFN(19, 21),
	    REG1_KEEP_DEFAULT       = REG_BIT_DEFN(22, 22),
	    REG1_EN_FAST_INIT       = REG_BIT_DEFN(23, 23),
	    REG1_HIT1               = REG_BIT_DEFN(24, 27),
	    REG1_HIT2               = REG_BIT_DEFN(28, 31),
		REG1_TEST_DATA			= REG_BIT_DEFN(24, 31)
	};

	enum {
	    REG2_ID2                = REG_BIT_DEFN(0, 7),
	    REG2_DELVAL1            = REG_BIT_DEFN(8, 26),
	    REG2_RFEDGE1            = REG_BIT_DEFN(27, 27),
	    REG2_RFEDGE2            = REG_BIT_DEFN(28, 28),
	    REG2_EN_INT_ALU         = REG_BIT_DEFN(29, 29),
	    REG2_EN_INT_HITS        = REG_BIT_DEFN(30, 30),
	    REG2_EN_INT_TDC_TIMEOUT = REG_BIT_DEFN(31, 31)
	};

	enum {
	    REG3_ID3                = REG_BIT_DEFN(0, 7),
	    REG3_DELVAL2            = REG_BIT_DEFN(8, 26),
	    REG3_SEL_TIMO_MB2       = REG_BIT_DEFN(27, 28),
	    REG3_EN_ERR_VAL         = REG_BIT_DEFN(29, 29),
	    REG3_EN_FIRST_WAVE      = REG_BIT_DEFN(30, 30),
	    REG3_EN_AUTOCALC_MB2    = REG_BIT_DEFN(31, 31)
	};

	enum {
	    REG4_ID4                = REG_BIT_DEFN(0, 7),
	    REG4_DELVAL3            = REG_BIT_DEFN(8, 26),
	    REG4_KEEP_DEFAULT       = REG_BIT_DEFN(27, 31)
	};


	enum {
	    REG3FW_ID3              = REG_BIT_DEFN(0, 7),
	    REG3FW_DELREL1          = REG_BIT_DEFN(8, 13),
	    REG3FW_DELREL2          = REG_BIT_DEFN(14, 19),
	    REG3FW_DELREL3          = REG_BIT_DEFN(20, 25),
	    REG3FW_KEEP_DEFAULT     = REG_BIT_DEFN(26, 26),
	    REG3FW_SEL_TIMO_MB2     = REG_BIT_DEFN(27, 28),
	    REG3FW_EN_ERR_VAL       = REG_BIT_DEFN(29, 29),
	    REG3FW_EN_FIRST_WAVE    = REG_BIT_DEFN(30, 30),
	    REG3FW_EN_AUTOCALC_MB2  = REG_BIT_DEFN(31, 31)
	};

	enum {
	    REG4FW_ID4              = REG_BIT_DEFN(0, 7),
	    REG4FW_OFFS             = REG_BIT_DEFN(8, 12),
	    REG4FW_OFFSRNG1         = REG_BIT_DEFN(13, 13),
	    REG4FW_OFFSRNG2         = REG_BIT_DEFN(14, 14),
	    REG4FW_EDGE_FW          = REG_BIT_DEFN(15, 15),
	    REG4FW_DIS_PW           = REG_BIT_DEFN(16, 16),
	    REG4FW_KEEP_DEFAULT     = REG_BIT_DEFN(17, 31)
	};

	enum {
	    REG5_ID5                = REG_BIT_DEFN(0, 7),
	    REG5_PHFIRE             = REG_BIT_DEFN(8, 23),
	    REG5_REPEAT_FIRE        = REG_BIT_DEFN(24, 26),
	    REG5_DIS_PHASESHIFT     = REG_BIT_DEFN(27, 27),
	    REG5_EN_STARTNOISE      = REG_BIT_DEFN(28, 28),
	    REG5_CONF_FIRE          = REG_BIT_DEFN(29, 31)
	};

	enum {
	    REG6_ID6                = REG_BIT_DEFN(0, 7),
	    REG6_ANZ_FIRE_MSB       = REG_BIT_DEFN(8, 10),
	    REG6_TEMP_PORTDIR       = REG_BIT_DEFN(11, 11),
	    REG6_DOUBLE_RES         = REG_BIT_DEFN(12, 12),
	    REG6_QUAD_RES           = REG_BIT_DEFN(13, 13),
	    REG6_FIREO_DEF          = REG_BIT_DEFN(14, 14),
	    REG6_HZ60               = REG_BIT_DEFN(15, 15),
	    REG6_CYCLE_TOF          = REG_BIT_DEFN(16, 17),
	    REG6_CYCLE_TEMP         = REG_BIT_DEFN(18, 19),
	    REG6_START_CLKHS_END    = REG_BIT_DEFN(20, 20),
	    REG6_EN_INT_END         = REG_BIT_DEFN(21, 21),
	    REG6_TW2                = REG_BIT_DEFN(22, 23),
	    REG6_EMPTY_0            = REG_BIT_DEFN(24, 24),
	    REG6_DA_KORR            = REG_BIT_DEFN(25, 28),
	    REG6_EMPTY_1            = REG_BIT_DEFN(29, 29),
	    REG6_NEG_STOP_TEMP      = REG_BIT_DEFN(30, 30),
	    REG6_EN_ANALOG          = REG_BIT_DEFN(31, 31)
	};
	 
	inline volatile T_register* regAddress(registers reg)
	{
		return reinterpret_cast<volatile T_register*>( 
			(unsigned int)registers_data
			+ reg * sizeof(T_register) );
	}

	inline T_register regRead(registers reg)
	{
		return *regAddress(reg);
	}
	 
	inline void regWrite(registers reg, T_register value)
	{
		*regAddress(reg) = value;
	}

	// Functions to read the requested bits from a register, decoding the information
	// stored by the above macro
	inline T_register bitmaskRead(registers reg, uint32_t bits)
	{
		// Get the current value of the register
		T_register regval = *regAddress(reg);

		// Extract the width of the bits required, stored in the last two bytes
		// of `bits`
		const uint32_t width  = bits & 0xff;

		// Extract the position of the first bit requested, stored in the first
		// two bytes of `bits`
		const uint32_t bitno  = bits >> 16;

		// Shift so that the first interesting bit is in position `1`
		regval >>= bitno;

		// Mask out only the bits below position `width`
		regval  &= (T_register(1<<width)-1);

		// Return
		return regval;
	}
	inline void bitmaskWrite(registers reg, uint32_t bits, T_register value)
	{
		// Get the current value of the register
		T_register regval = *regAddress(reg);

		// Extract the width of the bits required, stored in the last two bytes
		// of `bits`
		const uint32_t width  = bits & 0xff;

		// Extract the position of the first bit requested, stored in the first
		// two bytes of `bits`
		const uint32_t bitno  = bits >> 16;

		// Make a positive mask of just this value's position
		const T_register val_mask = T_register((1<<width)-1) << bitno;

		// Set all the bits of this value to zero
		regval &= ~val_mask;

		// Set all the bits of this value to their required new value
		regval |=  (value << bitno) & val_mask;

		// Store
		*regAddress(reg) = regval;
	}

	// Clean up after ourselves
	#undef REG_BIT_DEFN	
}
