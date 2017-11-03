#include <RF24.h>
#include <RF24.h>
#include <RF24_config.h>

#define RED_LED 7
#define GRN_LED 5
#define BLU_LED 3
#define CSN 10
#define CE 9

struct Sequence{
  int len;
  char* colors;
} seq;

RF24 rf(CE, CSN);

void setup() {
  // put your setup code here, to run once:
  rf.begin();
  rf.setChannel(5);
  rf.setPALevel(RF24_PA_MIN);
  rf.openWritingPipe(0xe7e7e7e7e7);
  rf.openReadingPipe(1, 0xc2c2c2c2c2);
  rf.setCRCLength(RF24_CRC_16);

  pinMode(RED_LED,OUTPUT);
  pinMode(GRN_LED,OUTPUT);
  pinMode(BLU_LED,OUTPUT);
  Serial.begin(9600);

  seq.len = 0; //init sequence
  seq.colors = (char*) malloc(0);
}

int readlen = 0;
int status = 1;
// 1 - correct and continue
// -1 - incorrect and restart
// 0 - do nothing

void loop() {
  // put your main code here, to run repeatedly:

  if(status == 1){

    Serial.println("Correct!");
    flashLED(GRN_LED);
    
    nextSeq();
    flashSequence();
    status = 0;
    // Write sequence to Arduino
    rf.stopListening();
    while(! rf.write(seq.colors, seq.len)) {
      //status = 0;
      Serial.println("Attempting to write");
    }


  }

  // Read from Arduino
  rf.startListening();

  if(rf.available()){
    rf.read(&status, sizeof(status)); // Read result from Arduino
    //print stuff for debug
    Serial.print("read status: ");
    Serial.println(status);
  }

  // If incorrect, restart game
  if(status == -1){
    Serial.println("Incorrect!");
    newGame();
  }

  //nextSeq();
  //flashSequence();
  
  delay(100); //arbitrary - change later
}

void nextSeq(){
  int newlen = ++(seq.len); // increase length by 1
  //Serial.println(seq.len);
  int randnum = random(3);
  
  char newchar;
  if(randnum == 0){
    //Serial.println(randnum);  
    newchar = 'R'; //add R
  }else if(randnum == 1){
    newchar = 'G'; //add G
  }else{
    newchar = 'B'; //add B
  }



  realloc(seq.colors,newlen); // do i need to save the old colors?
  (seq.colors)[newlen-1] = newchar;
}

void flashLED(int led){
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
}

void flashSequence(){
  int len = seq.len;
  int i = 0;
  char* iterchar = seq.colors;
  int numled;
  char currchar;

  Serial.print("Write seq: ");

  for(; i<len; iterchar++,i++){
    currchar = *iterchar;
    if(currchar == 'R'){
      numled = RED_LED;
    }else if(currchar == 'G'){
      numled = GRN_LED;
    }else if(currchar == 'B'){
      numled = BLU_LED;
    }else{
      Serial.println("currchar is not RGB!");
      return;
    }
    flashLED(numled);
    Serial.print(currchar);
  }
  Serial.println("");
}

void newGame(){
  free(seq.colors);
  seq.len = 0; //init sequence
  seq.colors = (char*) malloc(0);
  readlen = 0;
  flashLED(RED_LED);
  status = 1;
}

