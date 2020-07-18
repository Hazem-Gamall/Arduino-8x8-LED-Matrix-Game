#include"ball.h"

ball::ball(int x, int y):x(x), y(y) {}

void ball::setYdir(int yd){
  ydir = yd;
}

void ball::setXdir(int xd){
  xdir = xd;
}

void ball::setY(int ny){
  y = ny;
}

void ball::setX(int nx){
  x= nx;
}

void ball::move() {

	if (ydir > 0) {
		++y;
		
		if (y > 7) {
			y = 6;
			down();
		}
	}
	else if (ydir < 0) {
		--y;
		if (y < 0) {
			y = 1;
			up();
		}
	}
	if (xdir > 0) {
		++x;
		if (x > 7) {
			x = 6;
			
     left();
		}
	}
	else if (xdir < 0) {
		--x;
		if (x < 0){
		x = 1;
    right();
		}
	}
}
void ball::upLeft() {
  xdir = -1;
  ydir = 1;
  x++;
}

void ball::upRight() {
  xdir = 1;
  ydir = 1;
  x--;
}

void ball::down() {
	ydir = -1;
}

void ball::up() {
	ydir = 1;
}

void ball::left() {
	xdir = -1;
}

void ball::right() {
	xdir = 1;
}

int ball::getx(){
  return x%8;
}

int ball::gety(){
  return y%8;
}
