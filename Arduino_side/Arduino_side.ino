#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define CSN 10
#define CE 9
#define BTN 3

RF24 rf(CE, CSN);
void setup() {
  // put your setup code here, to run once:
  //rf(CE, CSN, 4000000);
  rf.begin();
  rf.setChannel(5);
  rf.setPALevel(RF24_PA_MIN);
  rf.openWritingPipe(0xe7e7e7e7e7); //what do i put here
  rf.setCRCLength(RF24_CRC_16);
  pinMode(BTN,INPUT);
  rf.stopListening();
  Serial.begin(9600);
  rf.printDetails();
}

void loop() {
  // put your main code here, to run repeatedly:
  int in = digitalRead(BTN);
  char buf = '!';
//  if(in == HIGH){ //input to button
//    Serial.println(buf);
//    if(!rf.write(&buf,1)){
//      Serial.println("No ack");
//    } else {
//      Serial.println("ack");
//      }
//  } else { //no input to button
//    Serial.println("No button presso");
//  }
  if (rf.write(&buf, 1)) {
    Serial.println("We in business, bitches");  
  } else {
    Serial.println("We ain't doing so hot, sweetharht");
  }
  
  Serial.println("FUCK.");
  
  delay(100);
}
