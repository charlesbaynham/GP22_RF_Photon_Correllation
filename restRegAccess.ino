#include "testReg.h"

void printReg(uint32_t reg[2]) {
  
  Serial.print(F("testRegistersData contains: 0b"));
  Serial.print(reg[0], BIN);
  Serial.print(F(", 0b"));
  Serial.println(reg[1], BIN);
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(250000);

  Serial.println(F("FIRST, SECOND_TWO, FOURTH_TO_SEVENTH"));
  Serial.println(testReg::FIRST, HEX);
  Serial.println(testReg::SECOND_TWO, HEX);
  Serial.println(testReg::FOURTH_TO_SEVENTH, BIN);

  printReg(testRegistersData);

  bitWrite_s(testReg::REG1, testReg::FIRST, 1);

  printReg(testRegistersData);

  bitWrite_s(testReg::REG1, testReg::FOURTH_TO_SEVENTH, 0b1010);

  printReg(testRegistersData);

}

void loop() {
  // put your main code here, to run repeatedly:

}
