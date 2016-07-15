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

enum REG0_BITS {
	REG0_ID0 						= REG_BIT_DEFN(0, 7),
	REG0_NEG_START                	= REG_BIT_DEFN(8, 8),
	REG0_NEG_STOP1                	= REG_BIT_DEFN(9, 9),
	REG0_NEG_STOP2                = REG_BIT_DEFN(10, 10),
	REG0_MESSB2                   = REG_BIT_DEFN(11, 11),
	REG0_NO_CAL_AUTO              = REG_BIT_DEFN(12, 12),
	REG0_CALIBRATE                = REG_BIT_DEFN(13, 13),
	REG0_SEL_ECLK_TMP             = REG_BIT_DEFN(14, 14),
	REG0_ANZ_FAKE                 = REG_BIT_DEFN(15, 15),
	REG0_TCYCLE                   = REG_BIT_DEFN(16, 16),
	REG0_ANZ_PORT                 = REG_BIT_DEFN(17, 17),
	REG0_START_CLKHS 				= REG_BIT_DEFN(18, 19),
	REG0_DIV_CLKHS              	= REG_BIT_DEFN(20, 21),
	REG0_ANZ_PER_CALRES         	= REG_BIT_DEFN(22, 23),
	REG0_DIV_FIRE               	= REG_BIT_DEFN(24, 27),
	REG0_ANZ_FIRE_START         	= REG_BIT_DEFN(28, 31)
};

enum REG1_BITS {
	REG1_ID1                    	= REG_BIT_DEFN(0, 7),
	REG1_SEL_TSTO1_0              = REG_BIT_DEFN(8, 8),
	REG1_SEL_TSTO1_1              = REG_BIT_DEFN(9, 9),
	REG1_SEL_TSTO1_2              = REG_BIT_DEFN(10, 10),
	REG1_SEL_TSTO2_0              = REG_BIT_DEFN(11, 11),
	REG1_SEL_TSTO2_1              = REG_BIT_DEFN(12, 12),
	REG1_SEL_TSTO2_2              = REG_BIT_DEFN(13, 13),
	REG1_SEL_START_FIRE           = REG_BIT_DEFN(14, 14),
	REG1_CURR32K                  = REG_BIT_DEFN(15, 15),
	REG1_HITIN1_0                 = REG_BIT_DEFN(16, 16),
	REG1_HITIN1_1                 = REG_BIT_DEFN(17, 17),
	REG1_HITIN1_2                 = REG_BIT_DEFN(18, 18),
	REG1_HITIN2_0                 = REG_BIT_DEFN(19, 19),
	REG1_HITIN2_1                 = REG_BIT_DEFN(20, 20),
	REG1_HITIN2_2                 = REG_BIT_DEFN(21, 21),
	REG1_KEEP_DEFAULT             = REG_BIT_DEFN(22, 22),
	REG1_EN_FAST_INIT             = REG_BIT_DEFN(23, 23),
	REG1_HIT1_0                   = REG_BIT_DEFN(24, 24),
	REG1_HIT1_1                   = REG_BIT_DEFN(25, 25),
	REG1_HIT1_2                   = REG_BIT_DEFN(26, 26),
	REG1_HIT1_3                   = REG_BIT_DEFN(27, 27),
	REG1_HIT2_0                   = REG_BIT_DEFN(28, 28),
	REG1_HIT2_1                   = REG_BIT_DEFN(29, 29),
	REG1_HIT2_2                   = REG_BIT_DEFN(30, 30),
	REG1_HIT2_3                   = REG_BIT_DEFN(31, 31)
};

enum REG2_BITS {
	REG2_ID2                    = REG_BIT_DEFN(0, 0),
	REG2_ID2                    = REG_BIT_DEFN(1, 1),
	REG2_ID2                    = REG_BIT_DEFN(2, 2),
	REG2_ID2                    = REG_BIT_DEFN(3, 3),
	REG2_ID2                    = REG_BIT_DEFN(4, 4),
	REG2_ID2                    = REG_BIT_DEFN(5, 5),
	REG2_ID2                    = REG_BIT_DEFN(6, 6),
	REG2_ID2                    = REG_BIT_DEFN(7, 7),
	REG2_DELVAL1_0                = REG_BIT_DEFN(8, 8),
	REG2_DELVAL1_1                = REG_BIT_DEFN(9, 9),
	REG2_DELVAL1_2                = REG_BIT_DEFN(10, 10),
	REG2_DELVAL1_3                = REG_BIT_DEFN(11, 11),
	REG2_DELVAL1_4                = REG_BIT_DEFN(12, 12),
	REG2_DELVAL1_5                = REG_BIT_DEFN(13, 13),
	REG2_DELVAL1_6                = REG_BIT_DEFN(14, 14),
	REG2_DELVAL1_7                = REG_BIT_DEFN(15, 15),
	REG2_DELVAL1_8                = REG_BIT_DEFN(16, 16),
	REG2_DELVAL1_9                = REG_BIT_DEFN(17, 17),
	REG2_DELVAL1_10               = REG_BIT_DEFN(18, 18),
	REG2_DELVAL1_11               = REG_BIT_DEFN(19, 19),
	REG2_DELVAL1_12               = REG_BIT_DEFN(20, 20),
	REG2_DELVAL1_13               = REG_BIT_DEFN(21, 21),
	REG2_DELVAL1_14               = REG_BIT_DEFN(22, 22),
	REG2_DELVAL1_15               = REG_BIT_DEFN(23, 23),
	REG2_DELVAL1_16               = REG_BIT_DEFN(24, 24),
	REG2_DELVAL1_17               = REG_BIT_DEFN(25, 25),
	REG2_DELVAL1_18               = REG_BIT_DEFN(26, 26),
	REG2_RFEDGE1                  = REG_BIT_DEFN(27, 27),
	REG2_RFEDGE2                  = REG_BIT_DEFN(28, 28),
	REG2_EN_INT_ALU           = REG_BIT_DEFN(29, 29),
	REG2_EN_INT_HITS           = REG_BIT_DEFN(30, 30),
	REG2_EN_INT_TDC_TIMEOUT           = REG_BIT_DEFN(31, 31)
};

enum REG3_BITS {
	REG3_ID3                    = REG_BIT_DEFN(0, 0),
	REG3_ID3                    = REG_BIT_DEFN(1, 1),
	REG3_ID3                    = REG_BIT_DEFN(2, 2),
	REG3_ID3                    = REG_BIT_DEFN(3, 3),
	REG3_ID3                    = REG_BIT_DEFN(4, 4),
	REG3_ID3                    = REG_BIT_DEFN(5, 5),
	REG3_ID3                    = REG_BIT_DEFN(6, 6),
	REG3_ID3                    = REG_BIT_DEFN(7, 7),
	REG3_DELVAL2_0                = REG_BIT_DEFN(8, 8),
	REG3_DELVAL2_1                = REG_BIT_DEFN(9, 9),
	REG3_DELVAL2_2                = REG_BIT_DEFN(10, 10),
	REG3_DELVAL2_3                = REG_BIT_DEFN(11, 11),
	REG3_DELVAL2_4                = REG_BIT_DEFN(12, 12),
	REG3_DELVAL2_5                = REG_BIT_DEFN(13, 13),
	REG3_DELVAL2_6                = REG_BIT_DEFN(14, 14),
	REG3_DELVAL2_7                = REG_BIT_DEFN(15, 15),
	REG3_DELVAL2_8                = REG_BIT_DEFN(16, 16),
	REG3_DELVAL2_9                = REG_BIT_DEFN(17, 17),
	REG3_DELVAL2_10               = REG_BIT_DEFN(18, 18),
	REG3_DELVAL2_11               = REG_BIT_DEFN(19, 19),
	REG3_DELVAL2_12               = REG_BIT_DEFN(20, 20),
	REG3_DELVAL2_13               = REG_BIT_DEFN(21, 21),
	REG3_DELVAL2_14               = REG_BIT_DEFN(22, 22),
	REG3_DELVAL2_15               = REG_BIT_DEFN(23, 23),
	REG3_DELVAL2_16               = REG_BIT_DEFN(24, 24),
	REG3_DELVAL2_17               = REG_BIT_DEFN(25, 25),
	REG3_DELVAL2_18               = REG_BIT_DEFN(26, 26),
	REG3_SEL_TIMO_MB2_0           = REG_BIT_DEFN(27, 27),
	REG3_SEL_TIMO_MB2_1           = REG_BIT_DEFN(28, 28),
	REG3_EN_ERR_VAL               = REG_BIT_DEFN(29, 29),
	REG3_EN_FIRST_WAVE            = REG_BIT_DEFN(30, 30),
	REG3_EN_AUTOCALC_MB2          = REG_BIT_DEFN(31, 31)
};

enum REG4_BITS {
	REG4_ID4                    = REG_BIT_DEFN(0, 0),
	REG4_ID4                    = REG_BIT_DEFN(1, 1),
	REG4_ID4                    = REG_BIT_DEFN(2, 2),
	REG4_ID4                    = REG_BIT_DEFN(3, 3),
	REG4_ID4                    = REG_BIT_DEFN(4, 4),
	REG4_ID4                    = REG_BIT_DEFN(5, 5),
	REG4_ID4                    = REG_BIT_DEFN(6, 6),
	REG4_ID4                    = REG_BIT_DEFN(7, 7),
	REG4_DELVAL3_0                = REG_BIT_DEFN(8, 8),
	REG4_DELVAL3_1                = REG_BIT_DEFN(9, 9),
	REG4_DELVAL3_2                = REG_BIT_DEFN(10, 10),
	REG4_DELVAL3_3                = REG_BIT_DEFN(11, 11),
	REG4_DELVAL3_4                = REG_BIT_DEFN(12, 12),
	REG4_DELVAL3_5                = REG_BIT_DEFN(13, 13),
	REG4_DELVAL3_6                = REG_BIT_DEFN(14, 14),
	REG4_DELVAL3_7                = REG_BIT_DEFN(15, 15),
	REG4_DELVAL3_8                = REG_BIT_DEFN(16, 16),
	REG4_DELVAL3_9                = REG_BIT_DEFN(17, 17),
	REG4_DELVAL3_10               = REG_BIT_DEFN(18, 18),
	REG4_DELVAL3_11               = REG_BIT_DEFN(19, 19),
	REG4_DELVAL3_12               = REG_BIT_DEFN(20, 20),
	REG4_DELVAL3_13               = REG_BIT_DEFN(21, 21),
	REG4_DELVAL3_14               = REG_BIT_DEFN(22, 22),
	REG4_DELVAL3_15               = REG_BIT_DEFN(23, 23),
	REG4_DELVAL3_16               = REG_BIT_DEFN(24, 24),
	REG4_DELVAL3_17               = REG_BIT_DEFN(25, 25),
	REG4_DELVAL3_18               = REG_BIT_DEFN(26, 26),
	REG4_KEEP_DEFAULT_0           = REG_BIT_DEFN(27, 27),
	REG4_KEEP_DEFAULT_1           = REG_BIT_DEFN(28, 28),
	REG4_KEEP_DEFAULT_2           = REG_BIT_DEFN(29, 29),
	REG4_KEEP_DEFAULT_3           = REG_BIT_DEFN(30, 30),
	REG4_KEEP_DEFAULT_4           = REG_BIT_DEFN(31, 31)
};


enum REG3_BITSFirstWave {
	CFG3FW_ID3_0                    = REG_BIT_DEFN(0, 0),
	CFG3FW_ID3_1                    = REG_BIT_DEFN(1, 1),
	CFG3FW_ID3_2                    = REG_BIT_DEFN(2, 2),
	CFG3FW_ID3_3                    = REG_BIT_DEFN(3, 3),
	CFG3FW_ID3_4                    = REG_BIT_DEFN(4, 4),
	CFG3FW_ID3_5                    = REG_BIT_DEFN(5, 5),
	CFG3FW_ID3_6                    = REG_BIT_DEFN(6, 6),
	CFG3FW_ID3_7                    = REG_BIT_DEFN(7, 7),
	CFG3FW_DELREL1_0                = REG_BIT_DEFN(8, 8),
	CFG3FW_DELREL1_1                = REG_BIT_DEFN(9, 9),
	CFG3FW_DELREL1_2                = REG_BIT_DEFN(10, 10),
	CFG3FW_DELREL1_3                = REG_BIT_DEFN(11, 11),
	CFG3FW_DELREL1_4                = REG_BIT_DEFN(12, 12),
	CFG3FW_DELREL1_5                = REG_BIT_DEFN(13, 13),
	CFG3FW_DELREL2_0                = REG_BIT_DEFN(14, 14),
	CFG3FW_DELREL2_1                = REG_BIT_DEFN(15, 15),
	CFG3FW_DELREL2_2                = REG_BIT_DEFN(16, 16),
	CFG3FW_DELREL2_3                = REG_BIT_DEFN(17, 17),
	CFG3FW_DELREL2_4                = REG_BIT_DEFN(18, 18),
	CFG3FW_DELREL2_5                = REG_BIT_DEFN(19, 19),
	CFG3FW_DELREL3_0                = REG_BIT_DEFN(20, 20),
	CFG3FW_DELREL3_1                = REG_BIT_DEFN(21, 21),
	CFG3FW_DELREL3_2                = REG_BIT_DEFN(22, 22),
	CFG3FW_DELREL3_3                = REG_BIT_DEFN(23, 23),
	CFG3FW_DELREL3_4                = REG_BIT_DEFN(24, 24),
	CFG3FW_DELREL3_5                = REG_BIT_DEFN(25, 25),
	CFG3FW_KEEP_DEFAULT             = REG_BIT_DEFN(26, 26),
	CFG3FW_SEL_TIMO_MB2_0           = REG_BIT_DEFN(27, 27),
	CFG3FW_SEL_TIMO_MB2_1           = REG_BIT_DEFN(28, 28),
	CFG3FW_EN_ERR_VAL               = REG_BIT_DEFN(29, 29),
	CFG3FW_EN_FIRST_WAVE            = REG_BIT_DEFN(30, 30),
	CFG3FW_EN_AUTOCALC_MB2          = REG_BIT_DEFN(31, 31)
};

enum REG4_BITSFirstWave {
	CFG4FW_ID4_0                    = REG_BIT_DEFN(0, 0),
	CFG4FW_ID4_1                    = REG_BIT_DEFN(1, 1),
	CFG4FW_ID4_2                    = REG_BIT_DEFN(2, 2),
	CFG4FW_ID4_3                    = REG_BIT_DEFN(3, 3),
	CFG4FW_ID4_4                    = REG_BIT_DEFN(4, 4),
	CFG4FW_ID4_5                    = REG_BIT_DEFN(5, 5),
	CFG4FW_ID4_6                    = REG_BIT_DEFN(6, 6),
	CFG4FW_ID4_7                    = REG_BIT_DEFN(7, 7),
	CFG4FW_OFFS_0                   = REG_BIT_DEFN(8, 8),
	CFG4FW_OFFS_1                   = REG_BIT_DEFN(9, 9),
	CFG4FW_OFFS_2                   = REG_BIT_DEFN(10, 10),
	CFG4FW_OFFS_3                   = REG_BIT_DEFN(11, 11),
	CFG4FW_OFFS_4                   = REG_BIT_DEFN(12, 12),
	CFG4FW_OFFSRNG1                 = REG_BIT_DEFN(13, 13),
	CFG4FW_OFFSRNG2                 = REG_BIT_DEFN(14, 14),
	CFG4FW_EDGE_FW                  = REG_BIT_DEFN(15, 15),
	CFG4FW_DIS_PW                   = REG_BIT_DEFN(16, 16),
	CFG4FW_KEEP_DEFAULT_0           = REG_BIT_DEFN(17, 17),
	CFG4FW_KEEP_DEFAULT_1           = REG_BIT_DEFN(18, 18),
	CFG4FW_KEEP_DEFAULT_2           = REG_BIT_DEFN(19, 19),
	CFG4FW_KEEP_DEFAULT_3           = REG_BIT_DEFN(20, 20),
	CFG4FW_KEEP_DEFAULT_4           = REG_BIT_DEFN(21, 21),
	CFG4FW_KEEP_DEFAULT_5           = REG_BIT_DEFN(22, 22),
	CFG4FW_KEEP_DEFAULT_6           = REG_BIT_DEFN(23, 23),
	CFG4FW_KEEP_DEFAULT_7           = REG_BIT_DEFN(24, 24),
	CFG4FW_KEEP_DEFAULT_8           = REG_BIT_DEFN(25, 25),
	CFG4FW_KEEP_DEFAULT_9           = REG_BIT_DEFN(26, 26),
	CFG4FW_KEEP_DEFAULT_10          = REG_BIT_DEFN(27, 27),
	CFG4FW_KEEP_DEFAULT_11          = REG_BIT_DEFN(28, 28),
	CFG4FW_KEEP_DEFAULT_12          = REG_BIT_DEFN(29, 29),
	CFG4FW_KEEP_DEFAULT_13          = REG_BIT_DEFN(30, 30),
	CFG4FW_KEEP_DEFAULT_14          = REG_BIT_DEFN(31, 31)
};

enum REG5_BITS {
	REG5_ID5                    = REG_BIT_DEFN(0, 0),
	REG5_ID5                    = REG_BIT_DEFN(1, 1),
	REG5_ID5                    = REG_BIT_DEFN(2, 2),
	REG5_ID5                    = REG_BIT_DEFN(3, 3),
	REG5_ID5                    = REG_BIT_DEFN(4, 4),
	REG5_ID5                    = REG_BIT_DEFN(5, 5),
	REG5_ID5                    = REG_BIT_DEFN(6, 6),
	REG5_ID5                    = REG_BIT_DEFN(7, 7),
	REG5_PHFIRE_0                 = REG_BIT_DEFN(8, 8),
	REG5_PHFIRE_1                 = REG_BIT_DEFN(9, 9),
	REG5_PHFIRE_2                 = REG_BIT_DEFN(10, 10),
	REG5_PHFIRE_3                 = REG_BIT_DEFN(11, 11),
	REG5_PHFIRE_4                 = REG_BIT_DEFN(12, 12),
	REG5_PHFIRE_5                 = REG_BIT_DEFN(13, 13),
	REG5_PHFIRE_6                 = REG_BIT_DEFN(14, 14),
	REG5_PHFIRE_7                 = REG_BIT_DEFN(15, 15),
	REG5_PHFIRE_8                 = REG_BIT_DEFN(16, 16),
	REG5_PHFIRE_9                 = REG_BIT_DEFN(17, 17),
	REG5_PHFIRE_10                = REG_BIT_DEFN(18, 18),
	REG5_PHFIRE_11                = REG_BIT_DEFN(19, 19),
	REG5_PHFIRE_12                = REG_BIT_DEFN(20, 20),
	REG5_PHFIRE_13                = REG_BIT_DEFN(21, 21),
	REG5_PHFIRE_14                = REG_BIT_DEFN(22, 22),
	REG5_PHFIRE_15                = REG_BIT_DEFN(23, 23),
	REG5_REPEAT_FIRE_0            = REG_BIT_DEFN(24, 24),
	REG5_REPEAT_FIRE_1            = REG_BIT_DEFN(25, 25),
	REG5_REPEAT_FIRE_2            = REG_BIT_DEFN(26, 26),
	REG5_DIS_PHASESHIFT           = REG_BIT_DEFN(27, 27),
	REG5_EN_STARTNOISE            = REG_BIT_DEFN(28, 28),
	REG5_CON_FIRE_0               = REG_BIT_DEFN(29, 29),
	REG5_CON_FIRE_1               = REG_BIT_DEFN(30, 30),
	REG5_CON_FIRE_2               = REG_BIT_DEFN(31, 31)
};

enum REG6_BITS {
	REG6_ID6                    = REG_BIT_DEFN(0, 0),
	REG6_ID6                    = REG_BIT_DEFN(1, 1),
	REG6_ID6                    = REG_BIT_DEFN(2, 2),
	REG6_ID6                    = REG_BIT_DEFN(3, 3),
	REG6_ID6                    = REG_BIT_DEFN(4, 4),
	REG6_ID6                    = REG_BIT_DEFN(5, 5),
	REG6_ID6                    = REG_BIT_DEFN(6, 6),
	REG6_ID6                    = REG_BIT_DEFN(7, 7),
	REG6_ANZ_FIRE_END_0           = REG_BIT_DEFN(8, 8),
	REG6_ANZ_FIRE_END_1           = REG_BIT_DEFN(9, 9),
	REG6_ANZ_FIRE_END_2           = REG_BIT_DEFN(10, 10),
	REG6_TEMP_PORTDIR             = REG_BIT_DEFN(11, 11),
	REG6_DOUBLE_RES               = REG_BIT_DEFN(12, 12),
	REG6_QUAD_RES                 = REG_BIT_DEFN(13, 13),
	REG6_FIREO_DEF                = REG_BIT_DEFN(14, 14),
	REG6_HZ60                     = REG_BIT_DEFN(15, 15),
	REG6_CYCLE_TOF_0              = REG_BIT_DEFN(16, 16),
	REG6_CYCLE_TOF_1              = REG_BIT_DEFN(17, 17),
	REG6_CYCLE_TEMP_0             = REG_BIT_DEFN(18, 18),
	REG6_CYCLE_TEMP_1             = REG_BIT_DEFN(19, 19),
	REG6_START_CLKHS_END          = REG_BIT_DEFN(20, 20),
	REG6_EN_INT_END               = REG_BIT_DEFN(21, 21),
	REG6_TW2_0                    = REG_BIT_DEFN(22, 22),
	REG6_TW2_1                    = REG_BIT_DEFN(23, 23),
	REG6_EMPTY_0                  = REG_BIT_DEFN(24, 24),
	REG6_DA_KORR_0                = REG_BIT_DEFN(25, 25),
	REG6_DA_KORR_1                = REG_BIT_DEFN(26, 26),
	REG6_DA_KORR_2                = REG_BIT_DEFN(27, 27),
	REG6_DA_KORR_3                = REG_BIT_DEFN(28, 28),
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