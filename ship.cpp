#include "ship.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string> 

//Constructor for class XYpoint
XYPoint::XYPoint(int a_x, int a_y){
	x = a_x;
	y = a_y;
}

//Creates a horizontal ship and adds points to the ship to make it horizontal at a given x,y point
Ship Ship::make_horizontal(string type, int x, int y){
	Ship temp_ship(type);
	temp_ship.setHoriz(true);
	temp_ship.points.clear();
	int curr_col = x;
	for(int i = 0; i < size; i++, curr_col++){
		XYPoint temp_p(curr_col,y);
		temp_ship.points.push_back(temp_p);
	}
	return temp_ship;
}

//Creates a vertical ship and adds points to the ship to make it vertical at an x,y point
Ship Ship::make_vertical(string type, int x, int y){
	Ship temp_ship(type);
	temp_ship.setHoriz(false);
	temp_ship.points.clear();
	int curr_row = y;
	for(int i = 0; i < size; i++, curr_row++){
		XYPoint temp_p(x,curr_row);
		temp_ship.points.push_back(temp_p);
	}
	return temp_ship;
}

//Ship constructor that takes in the type of ship it is
Ship::Ship(string type){
	isHoriz = true;
	row = 0;
	col = 0;
	if(type == "submarine"){
		size = 1;
		name = "submarine";
	}else if(type == "destroyer"){
		size = 2;
		name = "destroyer";
	}else if(type == "cruiser"){
		size = 3;
		name = "cruiser";
	}else if(type == "battleship"){
		size = 4;
		name = "battleship";
	}else if(type == "carrier"){
		size = 5;
		name = "carrier";
	}else if(type == "cargo"){
		size = 6;
		name = "cargo";
	}else if(type == "tanker"){
		size = 7;
		name = "tanker";
	}else{
		size = -1;
	}
}

//Returns true if the ship is horizontal
bool Ship::getHoriz(){
	return isHoriz;
}

//Sets the ship to hirzontal or vertical
void Ship::setHoriz(bool tf){
	isHoriz = tf;
}

//Returns the size of the ship
int Ship::getSize(){
	return size;
}

//Returns the name of the ship
string Ship::getType(){
	return name;
}

//Returns the row of the upper left point
int Ship::getRow(){
	return points[0].y;
}

//Returns the column of the upper left point
int Ship::getCol(){
	return points[0].x;
}

//Compares the size of two ships
bool biggerthan(Ship& s1, Ship& s2){
	if(s1.getSize() < s2.getSize()){
		return true;
	}else{
		return false;
	}
}