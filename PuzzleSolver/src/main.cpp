#include <cstdio>

#include "Sudoku.h"
#include "Pseudoku.h"

int main(int argc, const char* argv[])
{
	printf("PuzzleSolver: Init\n");

	using Pseudoku::PseudokuNew;
	int Result = PseudokuNew::RunTests();

	Result += SudokuTests::RunTests();

	return Result;
}