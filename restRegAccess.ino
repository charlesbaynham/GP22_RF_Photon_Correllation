// This is an example sketch showing how to access and modify a set of device registers
//
// The registers are defined in "GP22_reg.h" and are those for a GP22 time-to-digital convertor
// as an example.

// To adapt this code for another device, copy the GP22_reg.h file and follow the insutuctions in it
// to modifiy it for your needs. 

#include "GP22_reg.h"

uint32_t GP22::registers_data[7];

void printReg(uint32_t reg[7]) {
  
  Serial.println(F("reg contains: "));

  for (int i = 0; i<7; i++) {
    Serial.print(F("0x"));
    Serial.println(reg[i], HEX);
  }

  Serial.println(F("***"));
}

void setup() {

  Serial.begin(250000);

  printReg(GP22::registers_data);

  regWrite(GP22::REG0, 0x22019800);

  printReg(GP22::registers_data);

  bitmaskWrite(GP22::REG0, GP22::REG0_ID0, 0xab);
  bitmaskWrite(GP22::REG1, GP22::REG1_ID1, 0xcd);
  bitmaskWrite(GP22::REG2, GP22::REG2_ID2, 0xef);

  printReg(GP22::registers_data);

  bitmaskWrite(GP22::REG0, GP22::REG0_DIV_FIRE, 15);
  bitmaskWrite(GP22::REG0, GP22::REG0_NEG_STOP1, true);

  printReg(GP22::registers_data);

}

void loop() {
  // put your main code here, to run repeatedly:

}
