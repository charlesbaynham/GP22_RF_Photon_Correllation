uint32_t testRegistersData[2];

namespace testReg {

	enum testRegisters
	{
		REG1 = 0x00, // UART status register
		REG2  = 0x01, // Transmit control
	};
	 
	inline volatile uint8_t *regAddress(testRegisters reg)
	{
		return reinterpret_cast<volatile uint8_t*>(&testRegistersData + 
				reg * sizeof(testRegistersData));
	}

	inline uint8_t regRead(testRegisters reg)
	{
		return *regAddress(reg);
	}
	 
	inline void regWrite(testRegisters reg, uint8_t value)
	{
		*regAddress(reg) = value;
	}

	// A macro that defines enumeration values for a bitset
	// You supply the start and end bit positions
	// The result is a 4 byte uint:
	// the first 2 bytes store the index of the first bit
	// the last two store the index of the last bit
	#define REG_BIT_DEFN(start, end) ((uint32_t(start)<<16)|(end-start+1))
	enum testReg1Bits {
	  FIRST     		= REG_BIT_DEFN(0, 0),
	  SECOND_TWO 		= REG_BIT_DEFN(1, 2),
	  FOURTH_TO_SEVENTH	= REG_BIT_DEFN(3, 6)
	};

//^\s*(REG\d\w+)_\d\s*=\s*REG_BIT_DEFN\(\d+,\s*\d+\),$\n(?:^\s*\1_\d\s*=\s*REG_BIT_DEFN\(\d+,\s*\d+\),$\n)+
// ^\s*(?<theReg>REG\d\w+)_\d\s*=\s*REG_BIT_DEFN\((?<firstNum>\d+),\s*\d+\),$\n(?:^\s*\1_\d\s*=\s*REG_BIT_DEFN\((?<lastNum>\d+),\s*\d+\),$\n)+

// ^(?<theReg>\s*REG\d\w+)_\d+\s*=\s*BITMASK\((?<firstNum>\d+)\),$\n(?:^\s*\1_\d+\s*=\s*BITMASK\((?<lastNum>\d+)\)(?<lastComma>,?)$\n)+
// $+{theReg} = REG_BIT_DEF\($+{firstNum}, $+{lastNum}\)$+{lastComma}\n

enum REG0 {
    REG0_ID0 = REG_BIT_DEF(0, 7),
    REG0_NEG_START                = REG_BIT_DEFN(8, 8),
    REG0_NEG_STOP1                = REG_BIT_DEFN(9, 9),
    REG0_NEG_STOP2                = REG_BIT_DEFN(10, 10),
    REG0_MESSB2                   = REG_BIT_DEFN(11, 11),
    REG0_NO_CAL_AUTO              = REG_BIT_DEFN(12, 12),
    REG0_CALIBRATE                = REG_BIT_DEFN(13, 13),
    REG0_SEL_ECLK_TMP             = REG_BIT_DEFN(14, 14),
    REG0_ANZ_FAKE                 = REG_BIT_DEFN(15, 15),
    REG0_TCYCLE                   = REG_BIT_DEFN(16, 16),
    REG0_ANZ_PORT                 = REG_BIT_DEFN(17, 17),
    REG0_START_CLKHS_START = REG_BIT_DEF(18, 19),
    REG0_DIV_CLKHS = REG_BIT_DEF(20, 21),
    REG0_ANZ_PER_CALRES = REG_BIT_DEF(22, 23),
    REG0_DIV_FIRE = REG_BIT_DEF(24, 27),
    REG0_ANZ_FIRE_START = REG_BIT_DEF(28, 31)
};

enum REG1 {
    REG1_ID1 = REG_BIT_DEF(0, 7),
    REG1_SEL_TSTO1 = REG_BIT_DEF(8, 10),
    REG1_SEL_TSTO2 = REG_BIT_DEF(11, 13),
    REG1_SEL_START_FIRE           = REG_BIT_DEFN(14, 14),
    REG1_CURR32K                  = REG_BIT_DEFN(15, 15),
    REG1_HITIN1 = REG_BIT_DEF(16, 18),
    REG1_HITIN2 = REG_BIT_DEF(19, 21),
    REG1_KEEP_DEFAULT             = REG_BIT_DEFN(22, 22),
    REG1_EN_FAST_INIT             = REG_BIT_DEFN(23, 23),
    REG1_HIT1 = REG_BIT_DEF(24, 27),
    REG1_HIT2 = REG_BIT_DEF(28, 31)
};

enum REG2 {
    REG2_ID2 = REG_BIT_DEF(0, 7),
    REG2_DELVAL1 = REG_BIT_DEF(8, 26),
    REG2_RFEDGE1                  = REG_BIT_DEFN(27, 27),
    REG2_RFEDGE2                  = REG_BIT_DEFN(28, 28),
    REG2_EN_INT_ALU           = REG_BIT_DEFN(29, 29),
    REG2_EN_INT_HITS           = REG_BIT_DEFN(30, 30),
    REG2_EN_INT_TDC_TIMEOUT           = REG_BIT_DEFN(31, 31)
};

enum REG3 {
    REG3_ID3 = REG_BIT_DEF(0, 7),
    REG3_DELVAL2 = REG_BIT_DEF(8, 26),
    REG3_SEL_TIMO_MB2 = REG_BIT_DEF(27, 28),
    REG3_EN_ERR_VAL               = REG_BIT_DEFN(29, 29),
    REG3_EN_FIRST_WAVE            = REG_BIT_DEFN(30, 30),
    REG3_EN_AUTOCALC_MB2          = REG_BIT_DEFN(31, 31)
};

enum REG4 {
    REG4_ID4 = REG_BIT_DEF(0, 7),
    REG4_DELVAL3 = REG_BIT_DEF(8, 26),
    REG4_KEEP_DEFAULT = REG_BIT_DEF(27, 31)
};


enum REG3 {
    REG3_ID3 = REG_BIT_DEF(0, 7),
    REG3_DELREL1 = REG_BIT_DEF(8, 13),
    REG3_DELREL2 = REG_BIT_DEF(14, 19),
    REG3_DELREL3 = REG_BIT_DEF(20, 25),
    REG3_KEEP_DEFAULT             = REG_BIT_DEFN(26, 26),
    REG3_SEL_TIMO_MB2 = REG_BIT_DEF(27, 28),
    REG3_EN_ERR_VAL               = REG_BIT_DEFN(29, 29),
    REG3_EN_FIRST_WAVE            = REG_BIT_DEFN(30, 30),
    REG3_EN_AUTOCALC_MB2          = REG_BIT_DEFN(31, 31)
};

enum REG4 {
    REG4_ID4 = REG_BIT_DEF(0, 7),
    REG4_OFFS = REG_BIT_DEF(8, 12),
    REG4_OFFSRNG1                 = REG_BIT_DEFN(13, 13),
    REG4_OFFSRNG2                 = REG_BIT_DEFN(14, 14),
    REG4_EDGE_FW                  = REG_BIT_DEFN(15, 15),
    REG4_DIS_PW                   = REG_BIT_DEFN(16, 16),
    REG4_KEEP_DEFAULT = REG_BIT_DEF(17, 31)
};

enum REG5 {
    REG5_ID5 = REG_BIT_DEF(0, 7),
    REG5_PHFIRE = REG_BIT_DEF(8, 23),
    REG5_REPEAT_FIRE = REG_BIT_DEF(24, 26),
    REG5_DIS_PHASESHIFT           = REG_BIT_DEFN(27, 27),
    REG5_EN_STARTNOISE            = REG_BIT_DEFN(28, 28),
    REG5_CON_FIRE = REG_BIT_DEF(29, 31)
};

enum REG6 {
    REG6_ID6 = REG_BIT_DEF(0, 7),
    REG6_ANZ_FIRE_END = REG_BIT_DEF(8, 10),
    REG6_TEMP_PORTDIR             = REG_BIT_DEFN(11, 11),
    REG6_DOUBLE_RES               = REG_BIT_DEFN(12, 12),
    REG6_QUAD_RES                 = REG_BIT_DEFN(13, 13),
    REG6_FIREO_DEF                = REG_BIT_DEFN(14, 14),
    REG6_HZ60                     = REG_BIT_DEFN(15, 15),
    REG6_CYCLE_TOF = REG_BIT_DEF(16, 17),
    REG6_CYCLE_TEMP = REG_BIT_DEF(18, 19),
    REG6_START_CLKHS_END          = REG_BIT_DEFN(20, 20),
    REG6_EN_INT_END               = REG_BIT_DEFN(21, 21),
    REG6_TW2 = REG_BIT_DEF(22, 23),
    REG6_EMPTY_0                  = REG_BIT_DEFN(24, 24),
    REG6_DA_KORR = REG_BIT_DEF(25, 28),
    REG6_EMPTY_1                  = REG_BIT_DEFN(29, 29),
    REG6_NEG_STOP_TEMP            = REG_BIT_DEFN(30, 30),
    REG6_EN_ANALOG                = REG_BIT_DEFN(31, 31)
};



	#undef REG_BIT_DEFN
	 
	inline uint32_t bitRead_s(testRegisters reg, uint32_t bits)
	{
		uint32_t       regval = *regAddress(reg);
		const uint32_t width  = bits & 0xff;
		const uint32_t bitno  = bits >> 16;
		regval >>= bitno;
		regval  &= ((1<<width)-1);
		return regval;
	}
	inline void bitWrite_s(testRegisters reg, uint32_t bits, uint32_t value)
	{
		uint32_t           regval = *regAddress(reg);
		const uint32_t     width  = bits & 0xff;
		const uint32_t     bitno  = bits >> 16;
		regval &= ~(((1<<width)-1) << bitno);
		regval |=  value << bitno;
		*regAddress(reg) = regval;
	}
}