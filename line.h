#pragma once

class line {
  unsigned int x;
  unsigned int xl;
  unsigned int xr;
public:
  line();
  line(int,int,int);
  void shiftRight();
  void shiftLeft();
  int getxl();
  int getx();
  int getxr();
};
