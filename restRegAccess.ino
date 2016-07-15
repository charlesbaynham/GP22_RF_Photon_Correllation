#include "GP22_reg.h"

uint32_t GP22_registers_data[7];

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

  printReg(GP22_registers_data);

  regWrite(GP22_reg::REG0, 0x22019800);

  printReg(GP22_registers_data);

  bitWrite_s(GP22_reg::REG0, GP22_reg::REG0_ID0, 0xab);
  bitWrite_s(GP22_reg::REG1, GP22_reg::REG1_ID1, 0xcd);
  bitWrite_s(GP22_reg::REG2, GP22_reg::REG2_ID2, 0xef);

  printReg(GP22_registers_data);

  bitWrite_s(GP22_reg::REG0, GP22_reg::REG0_DIV_FIRE, 15);
  bitWrite_s(GP22_reg::REG0, GP22_reg::REG0_NEG_STOP1, true);

  printReg(GP22_registers_data);

}

void loop() {
  // put your main code here, to run repeatedly:

}
