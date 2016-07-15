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