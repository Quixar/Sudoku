#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define UNASSIGNED 0

class Sudoku {
private:
	int grid[9][9];
	int solnGrid[9][9];
	int guessNum[9];
	int gridPos[81];
	int difficultyLevel;
	bool grid_status;

public:
	Sudoku();
	Sudoku(string, bool row_major = true);
	void fillEmptyDiagonalBox(int);
	void createSeed();
	void printGrid();
	bool solveGrid();
	string getGrid();
	void countSoln(int& number);
	void genPuzzle();
	bool verifyGridStatus();
	void printSVG(string);
	void calculateDifficulty();
	int  branchDifficultyScore();
};