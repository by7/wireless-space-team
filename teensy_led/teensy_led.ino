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
  char colors[64];
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

  randomSeed(analogRead(0));
  seq.len = 0; //init sequence
  //seq.colors = (char*) malloc(0);
}

int readlen = 0;
int stat = 1;
// 1 - correct and continue
// 2 - incorrect and restart
// 0 - do nothing

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Stat: ");
  Serial.println(stat);
  if(stat == 1){

//    Serial.println("Correct!");
    flashLED(GRN_LED);
    delay(1000);
    nextSeq();
    flashSequence();
    stat = 0;
    // Write sequence to Arduino
    rf.stopListening();
    //delay(100);
    //rf.reUseTX();
    int chicken = rf.write(seq.colors, seq.len);
    Serial.print("Chicken: ");
    Serial.println(chicken);
//    while(! rf.write(seq.colors, seq.len)) {
//      //stat = 0;
//      Serial.println("Attempting to write");
//    }


  }

  if (stat == 0) {
  // Read from Arduino
  rf.startListening();
  delay(100);
  //Serial.println("WHy no avail?");
  while (! rf.available()) {
    Serial.println("WHy no available?");
    delay(1000);
  }
  if(rf.available()){
    //char buf;
    rf.read(&stat, sizeof(int));  // Read result from Arduino
    //print stuff for debug
    Serial.print("read stat: ");
    //stat = (int) buf;
    Serial.println(stat);
    //stat = 0;
  }
  }
  
  // If incorrect, restart game
  if(stat == 2){
    Serial.println("Incorrect!");
    newGame();
  }

/* WORKING EXAMPLE */  
//  rf.stopListening();
//  
//  int sending = 6;
//  int chicken = rf.write(&sending, sizeof(int));
//  Serial.println(chicken);
//
//  rf.startListening();
//  if (rf.available()) {
//    rf.read(&stat, sizeof(int));  
//    Serial.println(stat);
//  }
  
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



  //realloc(seq.colors,newlen); // do i need to save the old colors?
  (seq.colors)[newlen-1] = newchar;
}

void flashLED(int led){
  digitalWrite(led, HIGH);
  delay(300);
  digitalWrite(led, LOW);
  delay(300);
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
  //free(seq.colors);
  seq.len = 0; //init sequence
  //seq.colors = (char*) malloc(0);
  readlen = 0;
  flashLED(RED_LED);
  stat = 1;
}

