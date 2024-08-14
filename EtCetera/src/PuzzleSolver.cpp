#include "PuzzleSolver.h"
#include "Common.h"
#include "Sudoku.h"
#include "Pseudoku.h"

int PuzzleSolver_Main(int argc, const char* argv[])
{
	printf("PuzzleSolver: Init\n");

	using Pseudoku::PseudokuNew;
	int PzSolveResult = 0;
	PzSolveResult += PseudokuNew::RunTests();
	PzSolveResult += SudokuTests::RunTests();

	constexpr int NumSolverTests = 2;
	int Result = !(PzSolveResult >= NumSolverTests);
	return Result;
}