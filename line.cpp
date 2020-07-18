#include "line.h"

line::line(){}

line::line(int xl,int x, int xr) :xl(xl),x(x),xr(xr) {}


void line::shiftRight() {
  if(xl > 0){
  --xl;
  --x;
  --xr;}
}

void line::shiftLeft() {
  if(xr <7){
  ++xl;
  ++x;
  ++xr;
  }
}

int line::getxl() {
  return xl%8;
}

int line::getx(){
  return x%8;
}

int line::getxr() {
  return xr%8;
}
