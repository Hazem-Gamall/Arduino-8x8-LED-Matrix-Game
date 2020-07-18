class ball {
	int x;
	int y;
	int xdir=1;
	int ydir=1;
public:
  ball(int , int);
  void setYdir(int);
  void setXdir(int);
  void setY(int);
  void setX(int);
	void move();
	void up();
	void down();
	void right();
	void left();
	void upLeft();
	void upRight();
  int getx();
  int gety();
};
