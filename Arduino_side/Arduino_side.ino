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
} seqArduino;

RF24 rf(CE, CSN);
void setup() {
  // put your setup code here, to run once:
  //rf(CE, CSN, 4000000);
  rf.begin();
  rf.setChannel(5);
  rf.setPALevel(RF24_PA_MIN);
  rf.openWritingPipe(0xc2c2c2c2c2); //what do i put here
  rf.openReadingPipe(1s, 0xe7e7e7e7e7);
  rf.setCRCLength(RF24_CRC_16);
  
  pinMode(BTN_R,INPUT);
  pinMode(BTN_G,INPUT);
  pinMode(BTN_B,INPUT);
  //rf.stopListening();
  Serial.begin(9600);
  //rf.printDetails();

  seqArduino.len = 0; //init sequence
  seqArduino.colors = malloc(0);
}

int readlen = 0;
bool correct = true;
int indexArduino = 0;
char r = 'R', g = 'G', b = 'B';

void loop() {
  //get inputs for the buttons
  int red = digitalRead(BTN_R);
  int green = digitalRead(BTN_G);
  int blue = digitalRead(BTN_B);
  
  //reads sequence if new cycle and shit
  //  seqArduino.len = seq.len;
  //  realloc(seqArduino.colors, sizeof(char)*seq.len);
  
  // Increase size of array as player presses more buttons
  seqArduino.len++;
  realloc(seqArduino.colors(seqArduino.len));

  //get button presses 
  if (red == HIGH) {
    seqArduino.colors[indexArduino] = 'R';
  } else if (green == HIGH) {
    seqArduino.colors[indexArduino] = 'G';
  } else if (blue == HIGH) {
    seqArduiono.colors[indexArduino] = 'B';
  }

  // Read in Teensy data

  rf.startListening();

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