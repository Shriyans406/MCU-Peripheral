#include <SPI.h>
#include <stdint.h>
#define SPI_SCK 13
#define SPI_MISO 12
#define SPI_MOSI 11
#define SPI_SS 10


char dataBuff[500];

void SPI_SlaveInit(void){
i=0;
dataLen=SPI_SlaveReceive();
for(i=0; i<dataLLen; i++){
  dataBuff[i]=SPI_SlaveRecieve();
}


dataBuff[i]="\0";

Serial.println("Rcvd:");
Serial.println(dataBuff);
Serial.println("Length:");
Serial.println(dataLen);
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
