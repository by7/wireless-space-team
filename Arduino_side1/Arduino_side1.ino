#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define CSN 10
#define CE 9
#define BTN_R 3
#define BTN_G 5
#define BTN_B 7

struct Sequence {
  int len;
  char* colors;  
} seqArduino, seqTeensy;

RF24 rf(CE, CSN);
void setup() {
  // put your setup code here, to run once:
  //rf(CE, CSN, 4000000);
  rf.begin();
  rf.setChannel(5);
  rf.setPALevel(RF24_PA_MIN);
  rf.openWritingPipe(0xc2c2c2c2c2); //what do i put here
  rf.openReadingPipe(1, 0xe7e7e7e7e7);
  rf.setCRCLength(RF24_CRC_16);
  
  pinMode(BTN_R,INPUT);
  pinMode(BTN_G,INPUT);
  pinMode(BTN_B,INPUT);
  //rf.stopListening();
  Serial.begin(9600);
  //rf.printDetails();

  seqArduino.len = 1; //init sequence
  seqArduino.colors = malloc(sizeof(char));
  seqTeensy.len = 1; //init sequence
  seqTeensy.colors = malloc(sizeof(char));
}

//int readlen = 0;
int status = 0;
// 1 - check sequence
// -1 - get new sequence
// 0 - do nothing
int correct = 0;
// 1 - correct sequence
// -1 - incorrect sequence - game over
// 0 - meh
int index = 0;
int dumb = 0;

void loop() {
  //read from Teensy
  rf.startListening();
  if (rf.available()) {
    rf.read(seqTeensy.colors, sizeof(char)*seqTeensy.len); //Read seq from Teensy
    status = 1;
    Serial.println("New seq received");
  } else {
    dumb++;
    status = 0; //ignore everything until you get a message
  }
  rf.stopListening();
  
  if (status == 1) { //check over the sequence
    if(getSequence()){
      status = checkSequence(seqTeensy.len);
    }
  }

  if (status == -1) {
    newSeq(); //either moves on or starts over
  }
  
  delay(100);
}

//checks sequence and sets statuses
int checkSequence(int len) {
  if (index == len) {//got all the button presses without error
    correct = 1;
    return -1; //setting status to get new sequence
  }  
  if (seqArduino.colors[index-1] == seqTeensy.colors[index-1]) {
    return 1; //continue checking
  } else {
    correct = -1;
    return -1; //time to reset the game  
  }
}

bool getSequence() {
  //get inputs for the buttons
  int red = digitalRead(BTN_R);
  int green = digitalRead(BTN_G);
  int blue = digitalRead(BTN_B);  

  //get button presses 
  if (red == HIGH) {
    seqArduino.colors[index] = 'R';
    index++; //increment
    Serial.println("R");
    return true;
  } else if (green == HIGH) {
    seqArduino.colors[index] = 'G';
    index++; //increment
    Serial.println("G");
    return true;
  } else if (blue == HIGH) {
    seqArduino.colors[index] = 'B';
    index++; //increment
    Serial.println("B");
    return true;
  }
  return false;
}

void newSeq() {
  if (rf.write(&correct, sizeof(correct))) { //write successful
    if (correct == 1) {
      Serial.println("Moving on");
      seqArduino.len++;
      int newlen = ++(seqTeensy.len);
      realloc(seqArduino.colors,sizeof(char)*newlen);
      realloc(seqTeensy.colors,sizeof(char)*newlen);
    } else if (correct == -1) {
      Serial.println("Starting over");
      seqArduino.len = 1; //init sequence
      seqArduino.colors = realloc(seqArduino.colors,sizeof(char));
      seqTeensy.len = 1; //init sequence
      seqTeensy.colors = realloc(seqTeensy.colors,sizeof(char));  
    }
    correct = 0;
    status = 0;
    index = 0;
  }
}




