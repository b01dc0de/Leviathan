#include <cstdio>

#include "Sudoku.h"
#include "Pseudoku.h"

int main(int argc, const char* argv[])
{
	printf("Project Leviathan: Init\n");

	int Result = Pseudoku::RunTests();

	return Result;
}