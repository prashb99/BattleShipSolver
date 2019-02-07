#include "board.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string> 

//Constraints constructor
constraint::constraint(int aRow, int aCol, string a_C){
	row = aRow;
	col = aCol;
	c = a_C;
}

//Board constructor
Board::Board(vector<int> aRows, vector<int> aCols){
	rows = aRows.size();
	cols = aCols.size();
  	row_values = aRows;
  	col_values = aCols;
  	filled = new bool*[rows*2];
  	for(int r = 0; r < rows*2; r++){
  		filled[r] = new bool[cols*2];
  		for(int c = 0; c < cols*2; c++){
  			filled[r][c] = false;
  		}
  	}
}

//Board copy constructor
Board::Board(Board* &b){
	rows = b->getRows();
	cols = b->getCols();
  	row_values = b->getRowValues();
  	col_values = b->getColValues();
  	ships = b->ships;
  	constraints = b->constraints;
  	filled = b->filled;
  }


//Returns number of rows
int Board::getRows(){
	return rows;
}

//Returns number of columns
int Board::getCols(){
	return cols;
}

//Checks if the point passed in is already occupied by another ship
bool Board::isFilled(int row, int col){
	if(row < 0 || row >= rows || col >= cols || col <0){
		return false;
	}
	if(filled[row][col]){
		return true;	
	}
return false;
}

//Returns the column value for a given column
int Board::getColValue(int col){
	return col_values[col];
}

//Returns the row value for a given row
int Board::getRowValue(int row){
	return row_values[row];
}

//Sets the row value to a number at a given row
void Board::setRowValue(int row, int val){
	row_values[row] = val;
}

//Sets the column value to a number at a given column
void Board::setColValue(int col, int val){
	col_values[col] = val;
}

//Returns the vector of all the row values
vector<int> Board::getRowValues(){
	return row_values;
}

//Returns the vector of all the column values
vector<int> Board::getColValues(){
	return col_values;
}

//Prints out the board in correct format
void Board::print(){
	cout << "Solution:\n";
	for(Ship& s: ships){
		int row = s.getRow();
		int col = s.getCol();
		string t = s.getType();

		cout << t << " " << row << " " <<col << " ";
		if(s.getSize() > 1){
			if(s.isHoriz){
				cout << "horizontal";
			}else{
				cout << "vertical";
			}
		}
		cout << "\n";
	}

	cout << "+ ";
	for(int i = 0; i < getCols(); i++){
		cout << i << " ";
	}
	cout << "\n";
	for(int y = 0; y < rows; y++){
		cout <<  y << " ";
		for(int x = 0; x < cols; x++){
			if(isFilled(y,x)){
				cout << "X ";
			}else{
				cout << ". ";
			}
		}
		cout << "| " ;
		cout<< "\n";
	}
	cout << " ";
	for(int i = 0; i < getCols()*2; i++){
		cout << "_" ; 
	}
	cout << "\n\n";

}

//Adjusts the boards values after a ship is put in
void Board::updateValues(const Ship& s){
	for(const XYPoint& p: s.points){
		row_values[p.y] -=1;
		col_values[p.x] -=1;
		filled[p.y][p.x] = true;
	}
}

//Undos the boards values after recursion is called
void Board::reverseValues(const Ship& s){
	for(const XYPoint& p: s.points){
		row_values[p.y] +=1;
		col_values[p.x] +=1;
		filled[p.y][p.x] = false;
	}
}

//Checks all of the points around each point of a ship returns true if surroundings are clear
bool Board::check_surroundings(Ship curr_ship){
	bool fits = true;
	for(const XYPoint& p: curr_ship.points){
		int row = p.y;
		int col = p.x;
		if(isFilled(row,col)){
			fits = false;
		}else if(isFilled(row-1,col)){
			fits = false;
		}else if(isFilled(row,col-1)){
			fits = false;
		}else if(isFilled(row-1,col-1)){
			fits = false;
		}else if(isFilled(row-1, col+1)){
			fits =false;
		}else if(isFilled(row,col+1)){
			fits= false;
		}else if(isFilled(row+1,col+1)){
			fits = false;
		}else if(isFilled(row+1,col)){
			fits =false;
		}else if(isFilled(row+1,col-1)){
			fits =false;
		}
	}
	return fits;
}


//Checks if the a horizontal or vertical ship can fit in the board
bool Board::fits(Ship curr_ship){
	bool fits = false;
	bool fits_in_surroundings = check_surroundings(curr_ship);
	if(curr_ship.isHoriz && fits_in_surroundings){
		fits = true;
		for(const XYPoint& p: curr_ship.points){
			if(getColValue(p.x) == 0){
				fits = false;	
			}
		}
		if(fits){
			return true;
		}
	}
	if(!curr_ship.isHoriz && fits_in_surroundings){
		fits = true;
		for(const XYPoint& p: curr_ship.points){
			if(getRowValue(p.y) == 0){
				fits = false;
			}
		}
		if(fits){
			return true;
		}
	}


	return fits;
}


//Checks if the solution passes the constraints
bool Board::passes_constraints(){
	int constraints_passed = 0;
	for(const constraint& con: constraints){
		if(con.c == "o"){
			for(Ship& s: ships){
				if(s.points[0].y == con.row && s.points[0].x == con.col && s.getSize() == 1){
					constraints_passed++;
				}
			}
		}else if(con.c == "<"){
			for(Ship& s: ships){
				if(s.points[0].y == con.row && s.points[0].x == con.col && s.getHoriz()){
					constraints_passed++;
					break;
				}
			}
		}else if(con.c == ">"){
			for(Ship& s: ships){
				if(s.points[s.points.size()-1].y == con.row && s.points[s.points.size()-1].x == con.col && s.getHoriz()){
					constraints_passed++;
					break;
				}
			}
		}else if(con.c == "^"){
			for(Ship& s: ships){
				if(s.points[0].y == con.row && s.points[0].x == con.col && !s.getHoriz()){
					constraints_passed++;
					break;
				}
			}
		}else if(con.c == "v"){
			for(Ship& s: ships){
				if(s.points[s.points.size()-1].y == con.row && s.points[s.points.size()-1].x == con.col && !s.getHoriz()){
					constraints_passed++;
					break;
				}
			}
		}else if(con.c == "X"){
			for(Ship& s: ships){
				for(int x = 1; x < s.points.size()-1;x++){
					if(s.points[x].y == con.row && s.points[x].x == con.col && !s.getHoriz() && s.getSize() > 2){
						constraints_passed++;
						break;
					}
				}
			}
		}else if(con.c == "_"){
			if(!isFilled(con.row,con.col)){
				constraints_passed++;
			}
		}

	}
	return constraints_passed == constraints.size();
}


//Loops through each board in solutions and checks if the board passed in is the same as one of the solutions
bool same_board(vector<Board*> Solutions, Board* b){
	int count = 0;
	for(int i = 0; i < Solutions.size(); i++){
		bool add;
		int count_ships = 0;
		for(Ship& s1: b->ships){
			bool ship_there = false;
			for(Ship& s2: Solutions[i]->ships){
				if(s1.getSize() == s2.getSize() && s1.isHoriz == s2.isHoriz && s1.getCol() == s2.getCol() && s1.getRow() == s2.getRow()){
					ship_there = true;
				}
			}
			if(!ship_there){
				count_ships++;
			}
		}
		add = count_ships>0;
		if(add){
			count++;
		}
	}
	return count!=Solutions.size();
}

//Checks if all the lines are filled in the board
bool all_lines_filled(Board* b){
	bool filled = true;
	for(int i = 0; i < b->getRows();i++){
		if(b->getRowValue(i) != 0 && b->getRowValue(i) < b->getRows() ){
			filled = false;
		}
	}
	for(int i = 0; i <  b->getCols();i++){
		if(b->getColValue(i)!= 0 && b->getColValue(i) < b->getCols()){
			filled = false;
		}
	}
	return filled;
}

 