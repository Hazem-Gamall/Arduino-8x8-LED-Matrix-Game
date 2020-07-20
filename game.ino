#include <LedControl.h>
#include <IRremote.h>
#include "ball.h"
#include "line.h"

//PIN for the IR reciver
const int RECV_PIN = 5;

//IRreciver object to handle the sensor
IRrecv irrecv(RECV_PIN);
decode_results results; //decode sensor input
unsigned long prevSignal;

byte digits[10][8] = {{B01111100,B01000100,B01111100},
{B00100100,B01111100,B00000100}
,{B00100100,B01001100,B00110100}
,{B01010100,B01010100,B01111100}
,{B01110000,B00010000,B01111100}
,{B01110100,B01010100,B01011100}
,{B01111100,B01010100,B01011100}
,{B01000000,B01000000,B01111100}
,{B01111100,B01010100,B01111100}
,{B01110100,B01010100,B01111100}};


//pushButton pins
int leftButtonPin;
int rightButtonPin;

//which method of control to use
enum controller{pushButtons,IRremotee};
controller controller_status;
const int din = 2;
const int cs = 3;
const int clk = 4;

//used to control the speed of the game
unsigned long startmil;
unsigned long currentmil;
const unsigned long period = 5000;  //increae the speed every 5 seconds

//keep track of your score
unsigned long highscore = 0;
unsigned long score =0;

//take the L
byte L[8]={B01111110,B00000010,B00000010,B00000010,B00000010};


LedControl lc = LedControl(din,clk,cs,0);

ball b(0,0); //initialize the ball with the (0,0) coordinates
line l(3,4,5);//initialize the line with the (3,4,5) LEDs on the 8th row


void setup() {
  //pinMode(leftButtonPin,INPUT);
  //pinMode(rightButtonPin,INPUT);
  controller_status = IRremotee;  //i want to play with the remote
  irrecv.enableIRIn(); // Start the receiver
  lc.shutdown(false,0);
  lc.setIntensity(0,1);
  lc.clearDisplay(0);
  lc.setLed(0,l.getxl(),7,true);
  lc.setLed(0,l.getx(),7,true);
  lc.setLed(0,l.getxr(),7,true);
  Serial.begin(9600);
  startmil = millis();
}
const int initial = 250;
int d= initial; //inital speed

void changeSpeed(){
  currentmil = millis();
  if(currentmil - startmil >= period){  //has 5 seconds passed?
    if(d > 75){
      d -= 20;
    }
    startmil = currentmil;  //count a new 5 seconds starting from here
  }
}

void printDigit(byte letter[], int n){
    for(int i =n; i >0 ; --i){
    lc.setRow(0,i,letter[n-i]);
  }
}

void printScore(int score){ //function that takes a byte letter array and prints the equivalent letter
  int tens, ones;
  
  if(score > 9){
    ones = score%10;
    score = score/10;
    tens = score;
    int ar[2] = {ones,tens};
    int ord = 7;
    for(int i = 1; i>= 0; --i){
      printDigit(digits[ar[i]],ord);
      ord -=4;
    }
  }else{
    printDigit(digits[score],5);
  }
  
}


void lost(){
    if(b.getx() == 7){  //you lost

    highscore = score > highscore ? score:highscore;  //check to see if your score is now highscore
    
    d=initial;
    

    printScore(score);
    score =0; //for next match
    delay(3000);
    
    lc.clearDisplay(0);
    delay(100);
    
    printScore(highscore);
    
    //reset
    b.setX(0);
    b.setY(0);
    l.set(3,4,5);
    delay(3000);
    lc.clearDisplay(0);
    }
}

void lineTouch(){
    //if the ball hits the line
  if (b.getx()+1 == 7 && (b.gety() <=l.getxr() && b.gety()+1 >= l.getxl())) {
      ++score;
      b.setX(b.getx()-1);
      b.setXdir(-1);
  }
}

void control(){
   switch(controller_status){  //which control method?
    
    case IRremotee: //if you want to play with an IR remote
      if (irrecv.decode(&results)){
        Serial.println(results.value,HEX);  //debugging
        if(results.value == 0xFFFFFFFF){
          results.value = prevSignal;
        }
        if(results.value == 0xFF10EF || results.value == 0x8C22657B){ //the codes for the button chosen as left
          prevSignal = results.value;
          l.shiftLeft();
        }else if(results.value == 0xFF5AA5 || results.value == 0x449E79F){//the codes for the button chosen as right
          l.shiftRight();
          prevSignal = results.value;
        }
        irrecv.resume(); // Receive the next value
      }
      break;
    case pushButtons: //if you want to play wiht buttons
      if(digitalRead(rightButtonPin) == LOW){
        l.shiftRight();
      }if(digitalRead(leftButtonPin) == LOW){
        l.shiftLeft();
      }
  }
}

void  setLine(bool bl){
  //line LEDs
  lc.setLed(0,l.getxl(),7,bl);
  lc.setLed(0,l.getx(),7,bl);
  lc.setLed(0,l.getxr(),7,bl);
}

void setBall(bool bl){
   lc.setLed(0,b.gety(),b.getx(),bl); 
}

void cornerCase(){
  if(b.getx()+1 == 7 && b.gety()+1   == 6){ //"corner" case
      b.setY(5);
      b.setX(5);
     }
}

void loop() {
 
  b.move(); //determine where the ball should be
  
  setBall(true);  //turn on the LED representing the ball
  
  changeSpeed();  //https://en.wikipedia.org/wiki/Hamada_Helal
  
  delay(d); //control the speed of the game
  
  setBall(false);//turn off the LED representing the ball
  
  //cornerCase(); //fix my bad logic
  
  lost(); //you !won

  lineTouch();  //did the ball touch the line?
  
  setLine(false); //old line off
  
  control();  //input from controller
  
  setLine(true);  //new line on 
   
}
