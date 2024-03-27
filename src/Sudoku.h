#ifndef SUDOKU_H
#define SUDOKU_H

#define ROW_OF(r) {r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8]}
#define COL_OF(c) {c[0], c[9], c[18], c[27], c[36], c[45], c[54], c[63], c[72]}
#define BOX_OF(b) {b[0], b[1], b[2], b[9], b[10], b[11], b[18], b[19], b[20]}

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

typedef struct Pseudoku
{
    int Grid[81];

	int* GetRow(int RowIdx);
	int* GetCol(int ColIdx);
	int* GetBox(int BoxIdx);

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
} Pseudoku_Ver1_1;

int RunPseudokuTests();

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

#endif // SUDOKU_H