#ifndef SUDOKU_H
#define SUDOKU_H

constexpr int SudokuStride = 9;
constexpr int SudokuLength = SudokuStride * SudokuStride;

constexpr int SudokuMinValue = 1;
constexpr int SudokuMaxValue = 9;
constexpr int SudokuBoxSize = 3;

constexpr int SudokuGodNumber = 17;

struct SudokuGrid
{
	int Grid[SudokuLength];

	int& Cell(int Idx);
	const int& Cell(int Idx) const;
	int& Cell(int r, int c);
	int& operator[](int Idx);
	const int& operator[](int Idx) const;

	SudokuGrid();
	SudokuGrid(const SudokuGrid& Other) = default;
	SudokuGrid& operator=(const SudokuGrid& Other) = default;
	~SudokuGrid() = default;
};

struct BaseSudokuSolver
{
	SudokuGrid UnsolvedGrid;
	SudokuGrid SolvedGrid;

	bool bSolved = false;

	virtual int& NumAttempts() = 0;
	virtual int& NumSolves() = 0;
	virtual int& NumGuesses() = 0;
	virtual float& TotalTime() = 0;

	virtual bool Solve(const SudokuGrid& InGrid) = 0;
};

#endif // SUDOKU_H