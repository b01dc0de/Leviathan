#include "Common.h"
#include "PuzzleSolver.h"
#include "BitmapTest.h"

enum struct EtCeteraProject : int
{
	PuzzleSolver,
	BitmapTest,
	Project_Num
};

#define BUILD_PROJECT() (EtCeteraProject::BitmapTest)

int main(int argc, const char* argv[])
{
	int Result = 0;
	switch (BUILD_PROJECT())
	{
		case EtCeteraProject::PuzzleSolver:
		{
			Result = PuzzleSolver_Main(argc, argv);
		} break;
		case EtCeteraProject::BitmapTest:
		{
			Result = BitmapTest_Main(argc, argv);
		} break;
		default:
		{
			Result = 1;
			Assert(false);
		} break;
	}
	return Result;
}