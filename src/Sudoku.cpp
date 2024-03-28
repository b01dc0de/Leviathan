#include "Common.h"

#include "Sudoku.h"

int& SudokuGrid::Cell(int Idx)
{
	Assert(Idx >= 0 && Idx < SudokuLength);
	return Grid[Idx];
}

const int& SudokuGrid::Cell(int Idx) const
{
	Assert(Idx >= 0 && Idx < SudokuLength);
	return Grid[Idx];
}

int& SudokuGrid::Cell(int r, int c)
{
	return Cell(r * SudokuStride + c);
}

int& SudokuGrid::operator[](int Idx)
{
	return Cell(Idx);
}

const int& SudokuGrid::operator[](int Idx) const
{
	return Cell(Idx);
}

SudokuGrid::SudokuGrid()
{
	for (int Idx = 0; Idx < SudokuLength; Idx++)
	{
		Grid[Idx] = 0;
	}
}
