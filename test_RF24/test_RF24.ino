#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#define CSN 10
#define CE 9

RF24 rf(CE, CSN);
void setup() {
  // put your setup code here, to run once:
  rf.begin();
  rf.setChannel(5);
  rf.setPALevel(RF24_PA_MIN);
  rf.openReadingPipe(0,0xe7e7e7e7e7);
  rf.setCRCLength(RF24_CRC_16);
  rf.startListening();
  rf.printDetails();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char buf = '0';
  if(rf.available()){
    rf.read(&buf,sizeof(buf));
    Serial.println(buf);
  }else{
    Serial.println("not available");
  }
  delay(100);
}

