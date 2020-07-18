#include <LedControl.h>
#include "ball.h"
#include "line.h"

const int din = 2;
const int cs = 3;
const int clk = 4;
const int leftButtonPin = 5;
const int rightButtonPin = 6;
unsigned long highscore = 0;
unsigned long score =0;
unsigned long startmil;
unsigned long currentmil;
unsigned long period = 5000;

byte L[8]={B01111110,B00000010,B00000010,B00000010,B00000010};


LedControl lc = LedControl(din,clk,cs,0);
ball b(0,0);
line l(3,4,5);

void printLetter(byte letter[], int n){
  for(int i =n; i >0 ; --i){
    lc.setRow(0,i,letter[n-i]);
  }
}

void setup() {
  pinMode(leftButtonPin,INPUT);
  pinMode(rightButtonPin,INPUT);
  lc.shutdown(false,0);
  lc.setIntensity(0,1);
  lc.clearDisplay(0);
  lc.setLed(0,l.getxl(),7,true);
  lc.setLed(0,l.getx(),7,true);
  lc.setLed(0,l.getxr(),7,true);
  Serial.begin(9600);
  startmil = millis();
}

int d= 500;

void loop() {
  b.move();
  lc.setLed(0,b.gety(),b.getx(),true);
  currentmil = millis();
  if(currentmil - startmil >= period){
    if(d > 75){
      d -= 25;
    }
    startmil = currentmil;
  }
  delay(d);
  lc.setLed(0,b.gety(),b.getx(),false);
  if(b.getx()+1 == 7 && b.gety()+1   == 6){
      b.setY(5);
      b.setX(5);
     }
  if(b.getx() == 7){
    highscore = score > highscore ? score:highscore;
    d=500;
    printLetter(L,5);
    Serial.print("Your score: ");
    Serial.println(score);
    score =0;
    Serial.print("Highest score: ");
    Serial.println(highscore);
    
    b.setX(0);
    b.setY(0);
    delay(5000);
    lc.clearDisplay(0);
  }
  if (b.getx()+1 == 7 && (b.gety()+1 <=l.getxr() && b.gety()+1 >= l.getxl())) {
      ++score;
      b.setX(b.getx()-1);
      b.setXdir(-1);
      
    }
    
  
  lc.setLed(0,l.getxl(),7,false);
  lc.setLed(0,l.getx(),7,false);
  lc.setLed(0,l.getxr(),7,false);
  if(digitalRead(rightButtonPin) == LOW){
    l.shiftRight();
  }if(digitalRead(leftButtonPin) == LOW){
    l.shiftLeft();
  }
  lc.setLed(0,l.getxl(),7,true);
  lc.setLed(0,l.getx(),7,true);
  lc.setLed(0,l.getxr(),7,true);
   
  
   
}
