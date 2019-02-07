#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string> 
#include <sstream>
#include <bits/stdc++.h>
#include "ship.h"
#include "board.h"
using namespace std;

//Main recursive function to get the solutions
void recursive_find(vector<Board*>& Solutions, Board* b, vector<Ship> &ships) {
	//Base case if no ships left in ships vector
	if(ships.size() == 0){
		//Checks if the board is a solution
		if(all_lines_filled(b) &&  b->passes_constraints() && !same_board(Solutions,b)){
			//Creates a new board and pushes it back to Solutions
			Board* temp_board = new Board(b);
			Solutions.push_back(temp_board);

		}
		
	}else{
		//Assigns a new ship to the last(biggest) ship
		Ship temp_ship = ships[ships.size()-1];

		//Loops through each x,y coordinate point and see if a horizontal or vertical ship can fit at the point
		for(int x = 0; x < b->getCols();x++){
			for(int y = 0; y < b->getRows();y++){
				if(b->isFilled(y,x)){
					continue;
				}
				//Creates a new horizontal ship
				Ship horiz = temp_ship.make_horizontal(temp_ship.getType(),x,y);
				//Creates a new vertical ship
				Ship vert = temp_ship.make_vertical(temp_ship.getType(),x,y);

				//Gets the row value for the horizontal ship
				int horiz_row_val = b->getRowValue(horiz.getRow());

				//Gets the column value for the vertical ship
				int vert_col_val = b->getColValue(vert.getCol());

				//Checks if either the row value or column value at the point is 0
				if(horiz_row_val!=0 && vert_col_val!=0){

				//Bool that checks if the horizontal ship can fit
				bool ship_fits_horiz = horiz.getSize() <=horiz_row_val && (horiz.getCol() + horiz.getSize()) <= b->getCols()+1 && horiz.getRow() >= 0 && horiz.getRow() < b->getRows();

				//Bool that checks if the vertical ship can fit`
				bool ship_fits_vert = vert.getSize() <=vert_col_val && (vert.getRow() + vert.getSize()) <= b->getRows()+1 && vert.getCol() >= 0 && vert.getCol() < b->getCols();

				//If the ship is a submarine,
				if(horiz.getSize()==1){
					//If the ship fits then it adds it to the board's ships and recurses again with the board, then undos everything so the next call
					//can use the same board and ships
					if(ship_fits_horiz && ship_fits_vert && b->fits(horiz)){
						Ship temp_ship = ships[ships.size()-1];
						ships.pop_back();
						b->ships.push_back(horiz);
						b->updateValues(horiz);
						recursive_find(Solutions, b,ships);
						b->reverseValues(horiz);
						b->ships.pop_back();
						ships.push_back(temp_ship);
					}
				}else{
					//If the ship fits horizontally then it adds it to the board's ships and recurses again with the board, then undos everything so the next call
					//can use the same board and ships
					if(ship_fits_horiz && b->fits(horiz)){
						Ship temp_ship = ships[ships.size()-1];
						ships.pop_back();
						b->ships.push_back(horiz);
						b->updateValues(horiz);
						recursive_find(Solutions, b,ships);
						b->reverseValues(horiz);
						b->ships.pop_back();
						ships.push_back(temp_ship);
					}
					//If the ship fits vertically then it adds it to the board's ships and recurses again with the board, then undos everything so the next call
					//can use the same board and ships
					if(ship_fits_vert&& b->fits(vert)){
						Ship temp_ship = ships[ships.size()-1];
						ships.pop_back();
						b->ships.push_back(vert);
						b->updateValues(vert);
						recursive_find(Solutions, b,ships);
						b->reverseValues(vert);
						b->ships.pop_back();
						ships.push_back(temp_ship);
						
					}
				}
			}
		}
		}
		
	}	
}

int main(int argc, char* argv[]){
	//Checks if the number of arguments is 2 or 3
	if(argc!=2 && argc!=3){
		std::cerr << "Wrong number of argument\n";
		return 1;
	}


	ifstream inFile;
	inFile.open(argv[1]);
	vector<std::string> file_list;
	string x;

	//Reads the entire file into a vector of strings
	while(inFile >> x){
		file_list.push_back(x);
	}
	string board_name = file_list[0];

	//Reads in the number of rows in the board
	stringstream str1;
    str1 << file_list[1];
    int num_rows;
    str1 >> num_rows;

    //Reads in the number of columns in the board
    stringstream str2;
    str2 << file_list[2];
    int num_cols;
    str2 >> num_cols;

    //Reads in all of the row values for the board
    vector<int> row_vals;
    for(int i = 0;i<num_rows;i++){
    	stringstream str_row;
    	int row_val;
    	str_row << file_list[i+4];
    	str_row >> row_val;
    	row_vals.push_back(row_val);
    }

    //Reads in all of the column values for the board
    vector<int> col_vals;
    for(int i = 0;i<num_cols;i++){
    	stringstream str_col;
    	int col_val;
    	str_col << file_list[i+5+num_rows];
    	str_col >> col_val;
    	col_vals.push_back(col_val);
    }

    //Gets the current index after reading in rows and cols
    int curr_index = 5+num_rows+num_cols;
  	
  	//Creates a vector of ships
  	vector<Ship> all_ships;
  	while(curr_index<file_list.size() && file_list[curr_index]!="constraint"){
  		string type = file_list[curr_index];
  		Ship temp_ship = Ship(type);
  		all_ships.push_back(temp_ship);
  		curr_index++;
  	}

  	//Creates new board with given row and column values
  	Board* new_board = new Board(row_vals,col_vals);

  	//While there are constraints left in the file it reads them in an creates constraints
  	//and adds them to the board
  	while(curr_index<file_list.size()){
  		curr_index++;
  		stringstream constraint_row;
  		int c_row;
  		constraint_row<< file_list[curr_index];
  		constraint_row >> c_row;

  		curr_index++;
  		stringstream constraint_col;
  		int c_col;
  		constraint_col<< file_list[curr_index];
  		constraint_col >> c_col;
  		curr_index++;
  		string c_type = file_list[curr_index];
  		constraint temp_constraint(c_row,c_col,c_type);
  		new_board->constraints.push_back(temp_constraint);
  		curr_index++;
  	}

  	//Creates an empty vector of solutions
  	vector<Board*> Solutions;

  	//Sorts the ships based on size
  	sort(all_ships.begin(), all_ships.end(), biggerthan);

  	//Calls the recursive function with the empty vector of solutions, given board, and all the ships
  	recursive_find(Solutions,new_board, all_ships);

  	//After the recursion is done, it checks if the user is asking for 1 solution or all solutions
  	//and prints out accordingly
  	if(argc == 3){
  		for(int i = 0; i < Solutions.size();i++){
  			Solutions[i]->print();
  		}
  	}else{
  		if(Solutions.size()>0){
  			Solutions[0]->print();
  		}else{
  			cout << "No Solution\n";
  		}
  	}
  	return 0;
}

