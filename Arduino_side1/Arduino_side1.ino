#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define CSN 10
#define CE 9
#define BTN_R 3
#define BTN_G 5
#define BTN_B 7

int val;

struct Sequence {
  int len;
  char* colors;  
} seqArduino, seqTeensy;

RF24 rf(CE, CSN);
void setup() {
  // put your setup code here, to run once:
  rf.begin();
  rf.setChannel(5);
  rf.setPALevel(RF24_PA_MIN);
  rf.openWritingPipe(0xc2c2c2c2c2); //what do i put here
  rf.openReadingPipe(1, 0xe7e7e7e7e7);
  rf.setCRCLength(RF24_CRC_16);
  
  pinMode(BTN_R,INPUT);
  pinMode(BTN_G,INPUT);
  pinMode(BTN_B,INPUT);

  Serial.begin(9600);
  //rf.printDetails();

  seqArduino.len = 1; //init sequence
  seqArduino.colors = malloc(sizeof(char));
  seqTeensy.len = 1; //init sequence
  seqTeensy.colors = malloc(sizeof(char));
}

int stat = 0;
// 1 - check sequence
// -1 - get new sequence
// 0 - do nothing
int correct = 0;
// 1 - correct sequence
// -1 - incorrect sequence - game over
// 0 - meh
int index = 0;

void loop() {
  //read from Teensy

  rf.stopListening();
  int val = rf.write(&correct, sizeof(int));
  Serial.println(val);
  if (stat == -1) {
    newSeq(); //either moves on or starts over
  }
  
  rf.startListening();
  if(rf.available()){
    if(stat == 0){
      rf.read(seqTeensy.colors, sizeof(char)*seqTeensy.len); //Read seq from Teensy
      stat = 1;
      Serial.println("New seq received");
      for(int i=0;i<seqArduino.len;i++)
        Serial.print(*(seqArduino.colors+i));
      Serial.println();
    } else {
      Serial.println("nothing");
      //stat = 0; //ignore everything until you get a message
    }

  }
  
  if (stat == 1) { //check over the sequence
    Serial.println("pls check")
    if(getSequence()){
      stat = checkSequence(seqTeensy.len);
      Serial.print("stat is ");
      Serial.println(stat);
    }
  }
  
  delay(100);
}

//checks sequence and sets statuses
int checkSequence(int len) {
  if (seqArduino.colors[index-1] == seqTeensy.colors[index-1]) {
    if (index == len) {//got all the button presses without error
      correct = 1;
      return -1; //setting status to get new sequence
    }
    return 1; //continue checking
  } else {
    correct = -1;
    return -1; //time to reset the game  
  }
}

  int r = LOW;
  int g = LOW;
  int b = LOW; 

bool getSequence() {
  //get inputs for the buttons
  
  int red = digitalRead(BTN_R);
  int green = digitalRead(BTN_G);
  int blue = digitalRead(BTN_B);

  //get button presses 
  if (red == HIGH && r == LOW) {
    seqArduino.colors[index] = 'R';
    index++; //increment
    Serial.println("R");
    b = blue;r = red;g = green;
    return true;
  } else if (green == HIGH && g == LOW) {
    seqArduino.colors[index] = 'G';
    index++; //increment
    Serial.println("G");
    b = blue;r = red;g = green;
    return true;
  } else if (blue == HIGH && b == LOW) {
    seqArduino.colors[index] = 'B';
    index++; //increment
    Serial.println("B");
    b = blue;r = red;g = green;
    return true;
  }

  b = blue;r = red;g = green;
  return false;
}

void newSeq() {
  Serial.println("!!>>???");
  if (val) { //write successful
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
    Serial.println(">???");
    correct = 0;
    stat = 0;
    index = 0;
  }else{
    Serial.println("write failed");
  }
}




