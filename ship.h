#include <string>
#include <vector>
#pragma once
using namespace std;

class XYPoint{
public:
	XYPoint(int a_x, int a_y);
	int x;
	int y;
};

class Ship{

public:
	int getRow();
	int getCol();
	int getSize();
	string getType();
	Ship(string type);
	Ship make_horizontal(string type, int x, int y);
	Ship make_vertical(string type, int x, int y);
	int row;
	int col;
	bool isHoriz;
	bool getHoriz();
	void setHoriz(bool tf);
	vector<XYPoint> points;
private:
	int size;
	string name;


};

bool biggerthan(Ship& s1, Ship& s2);


