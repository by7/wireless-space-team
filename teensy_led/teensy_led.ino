#define RED_LED 7
#define GRN_LED 5
#define BLU_LED 3

struct Sequence{
  int len;
  char* colors;
} seq;

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_LED,OUTPUT);
  pinMode(GRN_LED,OUTPUT);
  pinMode(BLU_LED,OUTPUT);
  Serial.begin(9600);

  seq.len = 0; //init sequence
  seq.colors = malloc(0);
}

int readlen = 0;
bool correct = true;

void loop() {
  // put your main code here, to run repeatedly:

  if(correct){
    flashLED(GRN_LED);
    
    nextSeq();
    flashSequence();
    correct = false;
  }

  //read character from spi
  //if char does not match flash red and restart game

  correct = (readlen == s.len);
  delay(100); //arbitrary - change later
}

void nextSeq(){
  int newlen = ++(seq.len); // increase length by 1

  int randnum = random(3);
  char newchar;
  if(randnum == 0){
    newchar = 'R'; //add R
  }else if(randnum == 1){
    newchar = 'G'; //add G
  }else{
    newchar = 'B'; //add B
  }

  realloc(seq.colors,newlen); // do i need to save the old colors?
  (seq.colors)[newlen] = newchar;
}

void flashLED(int led){
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
}

void flashSequence(){
  int len = seq.len;
  char* iterchar = seq.colors;
  int numled;
  char currchar;
  for(; i<len; iterchar++){
    currchar = *iterchar;
    if(currchar == R){
      numled = RED_LED;
    }else if(currchar == G){
      numled = GRD_LED;
    }else if(currchar == B){
      numled = BLU_LED;
    }else{
      Serial.println("currchar is not RGB!");
      return;
    }
    flashLED(numled);
  }
}

void newGame(){
  free(seq.colors);
  seq.len = 0; //init sequence
  seq.colors = malloc(0);
  readlen = 0;
  correct = true;
}

