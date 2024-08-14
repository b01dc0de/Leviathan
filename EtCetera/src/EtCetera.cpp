#include "Common.h"
#include "PuzzleSolver.h"
#include "BitmapFile.h"

enum struct EtCeteraProject : int
{
	PuzzleSolver,
	BitmapFile,
	Project_Num
};

#define BUILD_PROJECT() (EtCeteraProject::PuzzleSolver)

int main(int argc, const char* argv[])
{
	int Result = 0;
	switch (BUILD_PROJECT())
	{
		case EtCeteraProject::PuzzleSolver:
		{
			Result = PuzzleSolver_Main(argc, argv);
		} break;
		case EtCeteraProject::BitmapFile:
		{
			Result = BitmapFile_Main(argc, argv);
		} break;
		default:
		{
			Result = 1;
			Assert(false);
		} break;
	}
}