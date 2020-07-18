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

void printLetter(byte letter[], int n){ //function that takes a byte letter array and prints the equivalent letter
  for(int i =n; i >0 ; --i){
    lc.setRow(0,i,letter[n-i]);
  }
}

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

int d= 250; //inital speed

void loop() {
 
  b.move(); //determine where the ball should be
  
  lc.setLed(0,b.gety(),b.getx(),true);  //turn on the LED representing the ball
  
  currentmil = millis();
  if(currentmil - startmil >= period){  //has 5 seconds passed?
    if(d > 50){
      d -= 10;
    }
    startmil = currentmil;  //count a new 5 seconds starting from here
  }
  delay(d); //control the speed of the game
  
  lc.setLed(0,b.gety(),b.getx(),false);//turn off the LED representing the ball
  
  if(b.getx()+1 == 7 && b.gety()+1   == 6){ //"corner" case
      b.setY(5);
      b.setX(5);
     }
     
  if(b.getx() == 7){  //you lost
    highscore = score > highscore ? score:highscore;  //check to see if your score is now highscore
    
    d=500;
    
    printLetter(L,5); //tAkE tHe L

    //time to be proud
    Serial.print("Your score: ");
    Serial.println(score);
    score =0; //for next match

    //time to not be proud
    Serial.print("Highest score: ");
    Serial.println(highscore);
    
    //reset
    b.setX(0);
    b.setY(0);
    l.set(3,4,5);
    delay(5000);
    lc.clearDisplay(0);
  }

  //if the ball hits the line
  if (b.getx()+1 == 7 && (b.gety()+1 <=l.getxr() && b.gety()+1 >= l.getxl())) {
      ++score;
      b.setX(b.getx()-1);
      b.setXdir(-1);
      
    }
    
  //line LEDs
  lc.setLed(0,l.getxl(),7,false);
  lc.setLed(0,l.getx(),7,false);
  lc.setLed(0,l.getxr(),7,false);

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
  
  lc.setLed(0,l.getxl(),7,true);
  lc.setLed(0,l.getx(),7,true);
  lc.setLed(0,l.getxr(),7,true);
   
  
   
}
