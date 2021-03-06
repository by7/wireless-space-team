#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define CSN 10
#define CE 9
#define BTN_R 3
#define BTN_G 4
#define BTN_B 5

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

<<<<<<< HEAD
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

=======
int readlen = 0;
bool correct = true;
int indexArduino = 0;
char r = 'R', g = 'G', b = 'B';
>>>>>>> 755728baae7112d5d262b69a979ce9b891b4a1f6

void loop() {
  //read from Teensy
  rf.startListening();
  if (rf.available()) {
    rf.read(&seqTeensy.colors, sizeof(char)*seqTeensy.len); //Read seq from Teensy
    status = 1;
  } else {
    status = 0; //ignore everything until you get a message
  }
  rf. stopListening();
  
  if (status == 1) { //check over the sequence
    getSequence();
    checkSequence(seqTeensy.len);
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
  if (seqArduino.colors[index] == seqTeensy.colors[index]) {
    return 1; //continue checking
  } else {
    correct = -1;
    return -1; //time to reset the game  
  }
}

void getSequence() {
  //get inputs for the buttons
  int red = digitalRead(BTN_R);
  int green = digitalRead(BTN_G);
<<<<<<< HEAD
  int blue = digitalRead(BTN_B);  

  char r = 'R', g = 'G', b = 'B';
=======
  int blue = digitalRead(BTN_B);
  
  //reads sequence if new cycle and shit
  //  seqArduino.len = seq.len;
  //  realloc(seqArduino.colors, sizeof(char)*seq.len);
  
  // Increase size of array as player presses more buttons
  seqArduino.len++;
  realloc(seqArduino.colors(seqArduino.len));

>>>>>>> 755728baae7112d5d262b69a979ce9b891b4a1f6
  //get button presses 
  if (red == HIGH) {
    seqArduino.colors[index] = 'R';
  } else if (green == HIGH) {
<<<<<<< HEAD
    seqArduino.colors[index] = 'G';
  } else if (blue == HIGH) {
    seqArduino.colors[index] = 'B';
  }

  index++; //increment
}

void newSeq() {
  if (rf.write(&correct, sizeof(correct))) { //write successful
    if (correct == 1) {
      seqArduino.len++;
      int newlen = ++(seqTeensy.len);
      realloc(seqArduino.colors,sizeof(char)*newlen);
      realloc(seqTeensy.colors,sizeof(char)*newlen);
    } else if (correct == -1) {
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
=======
    seqArduino.colors[indexArduino] = 'G';
  } else if (blue == HIGH) {
    seqArduiono.colors[indexArduino] = 'B';
  }

  // Read in Teensy data

  rf.startListening();
>>>>>>> 755728baae7112d5d262b69a979ce9b891b4a1f6

  if(rf.available()){
	// read in colors and size of array into seq
  }
 
  rf.stopListening();

  // if length is same and button presses are correct then send true and deallocate
  // otherwise false and deallocate

  if(seqArduino.len == seq.len){
     for(int i = 0; i < seq.len){
     	if(seqArduino.colors[i] == seq.colors[i])
	   // write success (1)
	else
	   // write fail (-1)
     }
     seqArduino.len = 0;
     free(seqArduino.colors);
     seqArduino.colors = malloc(0);
     indexArduino = -1;
  }

  indexArduino++;
  delay(100);
}