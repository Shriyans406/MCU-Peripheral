#include <Wire.h>

int LED=13;

uint8_t rcv_buf[32];
int data_len=0;

#define SLAVE ADDR 0x68

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Arduino Master");
  Serial.println("Send character \"s\" to begin");
  Serial.println("------------------------------");

}
