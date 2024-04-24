#ifndef SUDOKU_H
#define SUDOKU_H

constexpr int SudokuStride = 9;
constexpr int SudokuLength = SudokuStride * SudokuStride;

constexpr int SudokuMinValue = 1;
constexpr int SudokuMaxValue = 9;
constexpr int SudokuBoxSize = 3;

constexpr int SudokuGodNumber = 17;

namespace SudokuTests
{
	int RunTests();
};

struct SudokuGrid
{
	int Grid[SudokuLength];

	int& Cell(int Idx);
	const int& Cell(int Idx) const;
	int& Cell(int r, int c);
	const int& Cell(int r, int c) const;
	int& operator[](int Idx);
	const int& operator[](int Idx) const;

	void Print() const;

	SudokuGrid();
	SudokuGrid(const int InGrid[]);
	SudokuGrid(const SudokuGrid& Other) = default;
	SudokuGrid& operator=(const SudokuGrid& Other) = default;
	~SudokuGrid() = default;

	bool IsValid() const;
	bool IsSolved() const;
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

struct CoreSudokuSolver : BaseSudokuSolver
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
};

#endif // SUDOKU_H