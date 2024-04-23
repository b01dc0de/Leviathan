#include <cstdio>

#include "Sudoku.h"
#include "Pseudoku.h"

int main(int argc, const char* argv[])
{
	printf("Project Leviathan: Init\n");

	using Pseudoku::PseudokuNew;
	int Result = PseudokuNew::RunTests();

	Result += Sudoku::RunTests();

	return Result;
}