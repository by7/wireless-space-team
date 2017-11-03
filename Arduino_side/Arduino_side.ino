#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define CSN 10
#define CE 9
#define BTN_R 3
#define BTN_G 4
#define BTN_B 5

RF24 rf(CE, CSN);
void setup() {
  // put your setup code here, to run once:
  //rf(CE, CSN, 4000000);
  rf.begin();
  rf.setChannel(5);
  rf.setPALevel(RF24_PA_MIN);
  rf.openWritingPipe(0xe7e7e7e7e7); //what do i put here
  rf.setCRCLength(RF24_CRC_16);
  pinMode(BTN_R,INPUT);
  pinMode(BTN_G,INPUT);
  pinMode(BTN_B,INPUT);
  rf.stopListening();
  Serial.begin(9600);
  rf.printDetails();
}

void loop() {
  //get inputs for the buttons
  int red = digitalRead(BTN_R);
  int green = digitalRead(BTN_G);
  int blue = digitalRead(BTN_B);
  
  //what to send when buttons are pressed
  char r = 'r';
  char g = 'g';
  char b = 'b';
  
  if (red == HIGH) { //if red send r
    rf.write(&r, 1);
  } else if (green == HIGH) { //if green send g
    rf.write(&g, 1);
  } else if (blue == HIGH) { //if blue send b
    rf.write(&b, 1);
  }
  
//  if (rf.write(&buf, 1)) {
//    Serial.println("We in business, bitches");  
//  } else {
//    Serial.println("We ain't doing so hot, sweetharht");
//  }
  
  Serial.println("FUCK.");
  
  delay(100);
}
