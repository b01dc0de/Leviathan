#ifndef PSEUDOKU_H
#define PSEUDOKU_H

#include "Sudoku.h"

// Neighbors: contains all peers (row/col/box values) of a cell
struct Neighbors
{
    int Row[9];
    int Col[9];
    int Box[9];
};

// PossibleVals: contains possible values of a cell w/number of vals
struct PossibleVals
{
    int Vals[9];
    int Size;
};

struct Pseudoku : BaseSudokuSolver
{
	static int _NumAttempts;
	static int _NumSolves;
	static int _NumGuesses;
	static float _TotalTime;

	virtual int& NumAttempts() override;
	virtual int& NumSolves() override;
	virtual int& NumGuesses() override;
	virtual float& TotalTime() override;

	virtual bool Solve(const SudokuGrid& InGrid) override;

	SudokuGrid LocalGrid;

	static int RunTests();

	int* GetRowPtr(int RowIdx);
	int* GetColPtr(int ColIdx);
	int* GetBoxPtr(int BoxIdx);

    Neighbors GetNeighbors(int CellIdx);
    PossibleVals GetPosVals(int CellIdx);

    bool IsSafe(int CellIdx, int Value);
    int Fill(int StartIdx);

    void Print() const;

	void CopyFrom(const int InGrid[]);

	Pseudoku();
	Pseudoku(const int InGrid[]);
	Pseudoku(const Pseudoku& Other);
	Pseudoku& operator=(const Pseudoku& Other);
	~Pseudoku() = default;

	const char* sVersion = "1.1";
};

/*
{
    1 2 3  4 5 6  7 8 9
	4 5 6  7 8 9  1 2 3
	7 8 9  1 2 3  4 5 6

	2 3 4  5 6 7  8 9 1
	5 6 7  8 9 1  2 3 4
	8 9 1  2 3 4  5 6 7

	3 4 5  6 7 8  9 1 2
	6 7 8  9 1 2  3 4 5
	9 1 2  3 4 5  6 7 8
}

{
	 0  1  2   3  4  5   6  7  8
	 9 10 11  12 13 14  15 16 17
	18 19 20  21 22 23  24 25 26

	27 28 29  30 31 32  33 34 35
	36 37 38  39 40 41  42 43 44
	45 46 47  48 49 50  51 52 53

	54 55 56  57 58 59  60 61 62
	63 64 65  66 67 68  69 70 71
	72 73 74  75 76 77  78 79 80
}

{
	 1, 2, 3,----4, 5, 6,----7, 8, 9,
	.4, 5, 6,----7, 8, 9,----1, 2, 3,
	.7, 8, 9,----1, 2, 3,----4, 5, 6,
	|||||||||||||||||||||||||||||||||
	.2, 3, 4,----5, 6, 7,----8, 9, 1,
	.5, 6, 7,----8, 9, 1,----2, 3, 4,
	.8, 9, 1,----2, 3, 4,----5, 6, 7,
	|||||||||||||||||||||||||||||||||
	.3, 4, 5,----6, 7, 8,----9, 1, 2,
	.6, 7, 8,----9, 1, 2,----3, 4, 5,
	.9, 1, 2,----3, 4, 5,----6, 7, 8
}

{
	.0, .1, .2,---.3, .4, .5,---.6, .7, .8,
	..9, 10, 11,---12, 13, 14,---15, 16, 17,
	.18, 19, 20,---21, 22, 23,---24, 25, 26,
	||||||||||||||||||||||||||||||||||||||||
	.27, 28, 29,---30, 31, 32,---33, 34, 35,
	.36, 37, 38,---39, 40, 41,---42, 43, 44,
	.45, 46, 47,---48, 49, 50,---51, 52, 53,
	||||||||||||||||||||||||||||||||||||||||
	.54, 55, 56,---57, 58, 59,---60, 61, 62,
	.63, 64, 65,---66, 67, 68,---69, 70, 71,
	.72, 73, 74,---75, 76, 77,---78, 79, 80
}
*/

#endif // PSEUDOKU_H
