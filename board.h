#include <string>
#include <vector>
#include "ship.h"
using namespace std;
class constraint{
public:
	constraint(int y, int x, string con);
	int row;
	int col;
	string c;
};

class Board{

public:
	Board(vector<int> aRows, vector<int> aCols);
	Board(Board*& b);
	int getRows();
	int getCols();
	void destroy_filled_positions();
	bool isFilled(int row, int col);
	int getColValue(int col);
	int getRowValue(int row);
	vector<int> getRowValues();
	vector<int> getColValues();
	void print();
	bool fits(Ship s);
	bool check_surroundings(Ship s);
	void updateValues(const Ship& s);
	void reverseValues(const Ship& s);
	bool passes_constraints();
	void setRowValue(int row, int value);
	void setColValue(int row, int value);
	vector<Ship> ships;
	vector<constraint> constraints;
	bool** filled;

private:
	int rows;
	int cols;
	vector<int> row_values;
	vector<int> col_values;
};


bool same_board(vector<Board*> Solutions, Board* b);
bool all_lines_filled(Board* b);