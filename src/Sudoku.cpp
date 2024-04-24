#include "Common.h"

#include "Sudoku.h"

namespace SudokuUtils
{
	int RowOfCell(int CellIdx)
	{
		return CellIdx / SudokuStride;
	}

	int ColOfCell(int CellIdx)
	{
		return CellIdx % SudokuStride;
	}

	int BoxOfCell(int CellIdx)
	{
		return (RowOfCell(CellIdx) / 3 * 3) + (ColOfCell(CellIdx) / 3);
	}

	int GetRandomEmptyCell(const SudokuGrid &InGrid)
	{
		int EmptyCellIdx = -1;

		do
		{
			EmptyCellIdx = GenRandInRange(0, SudokuLength - 1);
		} while (InGrid[EmptyCellIdx] != 0);

		return EmptyCellIdx;
	}

	int GetNumCellsOccupied(const SudokuGrid &InGrid)
	{
		int CellsOccupied = 0;
		for (int Idx = 0; Idx < SudokuLength; Idx++)
		{
			if (InGrid.Grid[Idx] != 0)
			{
				CellsOccupied++;
			}
		}
		return CellsOccupied;
	}

	// CKA_NOTE: Definitions below -- require PosVal
	// CKA_TODO: Figure out better organization
	int GetRandomPosVal(const SudokuGrid &InGrid, int CellIdx);
	void GenRandomValues(SudokuGrid &InGrid, int NumValues);
}
using namespace SudokuUtils;

namespace SudokuTests
{
	constexpr int TestFullGridA[SudokuLength] =
	{
		1, 2, 3, 4, 5, 6, 7, 8, 9,
		4, 5, 6, 7, 8, 9, 1, 2, 3,
		7, 8, 9, 1, 2, 3, 4, 5, 6,
		5, 6, 7, 8, 9, 1, 2, 3, 4,
		8, 9, 1, 2, 3, 4, 5, 6, 7,
		2, 3, 4, 5, 6, 7, 8, 9, 1,
		9, 1, 2, 3, 4, 5, 6, 7, 8,
		6, 7, 8, 9, 1, 2, 3, 4, 5,
		3, 4, 5, 6, 7, 8, 9, 1, 2,
	};

	constexpr int TestFullGridB[SudokuLength] =
	{
		1, 4, 7, 5, 8, 2, 9, 6, 3,
		2, 5, 8, 6, 9, 3, 1, 7, 4,
		3, 6, 9, 7, 1, 4, 2, 8, 5,
		4, 7, 1, 8, 2, 5, 3, 9, 6,
		5, 8, 2, 9, 3, 6, 4, 1, 7,
		6, 9, 3, 1, 4, 7, 5, 2, 8,
		7, 1, 4, 2, 5, 8, 6, 3, 9,
		8, 2, 5, 3, 6, 9, 7, 4, 1,
		9, 3, 6, 4, 7, 1, 8, 5, 2
	};

	constexpr int TestFullGridC[SudokuLength] =
	{
		1, 2, 3, 4, 5, 6, 7, 8, 9,
		4, 5, 6, 7, 8, 9, 1, 2, 3,
		7, 8, 9, 1, 2, 3, 4, 5, 6,
		5, 6, 7, 8, 9, 1, 2, 3, 4,
		8, 9, 1, 2, 3, 4, 5, 6, 7,
		2, 3, 4, 5, 6, 7, 8, 9, 1,
		9, 1, 2, 6, 7, 8, 3, 4, 5,
		3, 4, 5, 9, 1, 2, 6, 7, 8,
		6, 7, 8, 3, 4, 5, 9, 1, 2
	};

	constexpr int GridCellNumbers[SudokuLength] =
	{
		0, 1, 2, /**/ 3, 4, 5, /**/ 6, 7, 8,
		9, 10, 11, /**/ 12, 13, 14, /**/ 15, 16, 17,
		18, 19, 20, /**/ 21, 22, 23, /**/ 24, 25, 26,
		////////////////////////////////////////////
		27, 28, 29, /**/ 30, 31, 32, /**/ 33, 34, 35,
		36, 37, 38, /**/ 39, 40, 41, /**/ 42, 43, 44,
		45, 46, 47, /**/ 48, 49, 50, /**/ 51, 52, 53,
		////////////////////////////////////////////
		54, 55, 56, /**/ 57, 58, 59, /**/ 60, 61, 62,
		63, 64, 65, /**/ 66, 67, 68, /**/ 69, 70, 71,
		72, 73, 74, /**/ 75, 76, 77, /**/ 78, 79, 80
	};

	int RunTests()
	{
		SudokuGrid TestFullA{TestFullGridA};
		SudokuGrid TestFullB{TestFullGridB};
		SudokuGrid TestFullC{TestFullGridC};

		int Result = 0;

		printf("\nBEGIN SudokuTests::RunTests:\n");

		{
			constexpr int NearFullGridA[SudokuLength] =
			{
				1, 2, 3, 4, 5, 6, 7, 0, 9,
				0, 5, 6, 7, 0, 9, 1, 2, 3,
				7, 0, 9, 1, 2, 3, 4, 5, 6,
				5, 6, 7, 8, 0, 1, 0, 3, 4,
				0, 9, 1, 2, 3, 4, 5, 0, 7,
				2, 3, 4, 0, 6, 7, 8, 9, 1,
				0, 0, 2, 0, 4, 5, 6, 7, 8,
				6, 7, 8, 9, 1, 2, 3, 0, 0,
				3, 4, 5, 0, 7, 8, 9, 1, 0,
			};
			SudokuGrid NearFullGrid(NearFullGridA);
			CoreSudokuSolver NearFullSolver;
			NearFullSolver.Solve(NearFullGrid);
			printf("UnsolvedGrid:\n");
			NearFullSolver.UnsolvedGrid.Print();
			printf("\nSolvedGrid:\n");
			NearFullSolver.SolvedGrid.Print();

			Result += NearFullSolver.bSolved ? 1 : 0;

			constexpr int PartialGridA[SudokuLength] =
			{
				1, 0, 3, 4, 0, 6, 7, 0, 9,
				0, 5, 0, 7, 0, 9, 0, 2, 3,
				7, 0, 9, 1, 2, 3, 4, 0, 6,
				5, 0, 7, 8, 0, 1, 0, 0, 4,
				0, 0, 0, 2, 3, 4, 5, 0, 7,
				0, 3, 4, 0, 6, 0, 0, 9, 0,
				0, 0, 2, 0, 4, 5, 6, 0, 8,
				6, 7, 0, 9, 1, 2, 3, 0, 0,
				0, 4, 5, 0, 7, 0, 9, 1, 0,
			};
			SudokuGrid PartialGrid(NearFullGridA);
			CoreSudokuSolver PartialSolver;
			PartialSolver.Solve(PartialGrid);
			printf("UnsolvedGrid:\n");
			PartialSolver.UnsolvedGrid.Print();
			printf("\nSolvedGrid:\n");
			PartialSolver.SolvedGrid.Print();

			Result += NearFullSolver.bSolved ? 1 : 0;


			constexpr int TestNearEmptyGridA[SudokuLength] =
			{
				1, 0, 0, 4, 0, 0, 0, 0, 9,
				0, 5, 0, 7, 0, 9, 0, 2, 0,
				7, 0, 0, 0, 2, 0, 4, 0, 6,
				5, 0, 0, 0, 0, 1, 0, 0, 4,
				0, 0, 0, 2, 0, 0, 5, 0, 0,
				0, 0, 4, 0, 0, 0, 0, 9, 0,
				0, 0, 0, 0, 4, 0, 6, 0, 8,
				6, 0, 0, 0, 0, 0, 3, 0, 0,
				0, 0, 5, 0, 7, 0, 0, 1, 0,
			};
			SudokuGrid NearEmptyGrid(TestNearEmptyGridA);
			CoreSudokuSolver NearEmptySolver;
			NearEmptySolver.Solve(NearEmptyGrid);
			printf("UnsolvedGrid:\n");
			NearEmptySolver.UnsolvedGrid.Print();
			printf("\nSolvedGrid:\n");
			NearEmptySolver.SolvedGrid.Print();

			Result += NearEmptySolver.bSolved ? 1 : 0;

			printf("\n\t\tSTATS:\n");
			printf("\t\t\tAttempts: %d\n", NearEmptySolver.NumAttempts());
			printf("\t\t\tSolves: %d\n", NearEmptySolver.NumSolves());
			printf("\t\t\tGuesses: %d\n", NearEmptySolver.NumGuesses());
			printf("\t\t\tTotalTime: %.2f\n", NearEmptySolver.TotalTime());
		}

/*
		SudokuGrid StartingGrid;
		//GenRandomValues(StartingGrid, SudokuGodNumber);
		GenRandomValues(StartingGrid, 60);
		Assert(StartingGrid.IsValid());

		CoreSudokuSolver CoreSolver;
		CoreSolver.Solve(StartingGrid);

		printf("UnsolvedGrid:\n");
		CoreSolver.UnsolvedGrid.Print();

		printf("\nSolvedGrid:\n");
		CoreSolver.SolvedGrid.Print();

		printf("\n\t\tSTATS:\n");
		printf("\t\t\tAttempts: %d\n", CoreSolver.NumAttempts());
		printf("\t\t\tSolves: %d\n", CoreSolver.NumSolves());
		printf("\t\t\tGuesses: %d\n", CoreSolver.NumGuesses());
		printf("\t\t\tTotalTime: %.2f\n", CoreSolver.TotalTime());

		Result += CoreSolver.bSolved ? 1 : 0;
*/

		printf("END SudokuTests::RunTests\n\n");

		return Result;
	}
};

struct GridContainer
{
	const static int ContainerLength = SudokuStride;
	enum class Type
	{
		Row,
		Col,
		Box,
		None
	};
	int Vals[ContainerLength] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	bool IsValid();

	GridContainer() = default;
	GridContainer(const SudokuGrid &InGrid, Type InType, int Idx);
};

bool GridContainer::IsValid()
{
	bool Result = true;
	int ValRef[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	for (int Idx = 0; Idx < ContainerLength; Idx++)
	{
		int CurrVal = Vals[Idx];
		if (0 == CurrVal)
		{
			continue;
		}
		else if (0 == ValRef[CurrVal - 1])
		{
			Result = false;
			break;
		}
		else
		{
			ValRef[CurrVal - 1] = 0;
		}
	}
	return Result;
}

GridContainer::GridContainer(const SudokuGrid &InGrid, Type InType, int Idx)
{
	switch (InType)
	{
		case Type::Row:
		{
			for (int ValIdx = 0; ValIdx < ContainerLength; ValIdx++)
			{
				Vals[ValIdx] = InGrid.Cell(Idx, ValIdx);
			}
		} break;
		case Type::Col:
		{
			for (int ValIdx = 0; ValIdx < ContainerLength; ValIdx++)
			{
				Vals[ValIdx] = InGrid.Cell(ValIdx, Idx);
			}
		} break;
		case Type::Box:
		{
			int ValIdx = 0;
			int BeginRow = Idx / 3 * 3;
			int BeginCol = Idx % 3 * 3;
			for (int RowIdx = BeginRow; RowIdx < 3; RowIdx++)
			{
				for (int ColIdx = BeginCol; ColIdx < 3; ColIdx++)
				{
					Assert((RowIdx * 9 + ColIdx) < 81);
					Vals[ValIdx++] = InGrid.Cell(RowIdx, ColIdx);
				}
			}
		} break;
		case Type::None:
		default:
		{
			Assert(false);
		}
		break;
	}
}

int &SudokuGrid::Cell(int Idx)
{
	Assert(0 <= Idx && Idx < SudokuLength);
	return Grid[Idx];
}

const int &SudokuGrid::Cell(int Idx) const
{
	Assert(0 <= Idx && Idx < SudokuLength);
	return Grid[Idx];
}

int &SudokuGrid::Cell(int r, int c)
{
	return Cell(r * SudokuStride + c);
}

const int &SudokuGrid::Cell(int r, int c) const
{
	return Cell(r * SudokuStride + c);
}

int &SudokuGrid::operator[](int Idx)
{
	return Cell(Idx);
}

const int &SudokuGrid::operator[](int Idx) const
{
	return Cell(Idx);
}

void SudokuGrid::Print() const
{
	for (int r = 0; r < SudokuStride; r++)
	{
		int StartIdx = r * SudokuStride;
		printf("%d %d %d | %d %d %d | %d %d %d\n",
			   Grid[StartIdx + 0],
			   Grid[StartIdx + 1],
			   Grid[StartIdx + 2],
			   Grid[StartIdx + 3],
			   Grid[StartIdx + 4],
			   Grid[StartIdx + 5],
			   Grid[StartIdx + 6],
			   Grid[StartIdx + 7],
			   Grid[StartIdx + 8]);

		constexpr int FirstBoxEndRow = 2;
		constexpr int SecondBoxEndRow = 5;
		constexpr int ThirdBoxEndRow = 8;
		if (r == FirstBoxEndRow || r == SecondBoxEndRow)
		{
			printf("- - - | - - - | - - -\n");
		}
		else if (r == ThirdBoxEndRow)
		{
			printf("\n\n");
		}
	}
	/*
	printf("%d %d %d | %d %d %d | %d %d %d\n",   grid[0],  grid[1],  grid[2],  grid[3],  grid[4],  grid[5],  grid[6],  grid[7],  grid[8]);
	printf("%d %d %d | %d %d %d | %d %d %d\n",   grid[9],  grid[10], grid[11], grid[12], grid[13], grid[14], grid[15], grid[16], grid[17]);
	printf("%d %d %d | %d %d %d | %d %d %d\n",   grid[18], grid[19], grid[20], grid[21], grid[22], grid[23], grid[24], grid[25], grid[26]);
	printf("- - - | - - - | - - -\n");
	printf("%d %d %d | %d %d %d | %d %d %d\n",   grid[27], grid[28], grid[29], grid[30], grid[31], grid[32], grid[33], grid[34], grid[35]);
	printf("%d %d %d | %d %d %d | %d %d %d\n",   grid[36], grid[37], grid[38], grid[39], grid[40], grid[41], grid[42], grid[43], grid[44]);
	printf("%d %d %d | %d %d %d | %d %d %d\n",   grid[45], grid[46], grid[47], grid[48], grid[49], grid[50], grid[51], grid[52], grid[53]);
	printf("- - - | - - - | - - -\n");
	printf("%d %d %d | %d %d %d | %d %d %d\n",   grid[54], grid[55], grid[56], grid[57], grid[58], grid[59], grid[60], grid[61], grid[62]);
	printf("%d %d %d | %d %d %d | %d %d %d\n",   grid[63], grid[64], grid[65], grid[66], grid[67], grid[68], grid[69], grid[70], grid[71]);
	printf("%d %d %d | %d %d %d | %d %d %d\n\n", grid[72], grid[73], grid[74], grid[75], grid[76], grid[77], grid[78], grid[79], grid[80]);
	*/
}

SudokuGrid::SudokuGrid()
{
	for (int Idx = 0; Idx < SudokuLength; Idx++)
	{
		Grid[Idx] = 0;
	}
}

SudokuGrid::SudokuGrid(const int InGrid[])
{
	// CKA_TODO: Make safe
	for (int Idx = 0; Idx < SudokuLength; Idx++)
	{
		Grid[Idx] = InGrid[Idx];
	}
}

bool SudokuGrid::IsValid() const
{
	bool Result = true;
	for (int Idx = 0; Idx < SudokuStride; Idx++)
	{
		GridContainer RowCon(*this, GridContainer::Type::Row, Idx);
		GridContainer ColCon(*this, GridContainer::Type::Col, Idx);
		GridContainer BoxCon(*this, GridContainer::Type::Box, Idx);

		if (!RowCon.IsValid() || !ColCon.IsValid() || !BoxCon.IsValid())
		{
			Result = false;
			break;
		}
	}
	return Result;
}

bool SudokuGrid::IsSolved() const
{
	bool Result = true;
	for (int Idx = 0; Idx < SudokuLength; Idx++)
	{
		if (0 == Grid[Idx])
		{
			Result = false;
			break;
		}
	}
	return Result;
}

int CoreSudokuSolver::_NumAttempts = 0;
int CoreSudokuSolver::_NumSolves = 0;
int CoreSudokuSolver::_NumGuesses = 0;
float CoreSudokuSolver::_TotalTime = 0.0f;

int &CoreSudokuSolver::NumAttempts()
{
	return _NumAttempts;
}

int &CoreSudokuSolver::NumSolves()
{
	return _NumSolves;
}

int &CoreSudokuSolver::NumGuesses()
{
	return _NumGuesses;
}

float &CoreSudokuSolver::TotalTime()
{
	return _TotalTime;
}

struct PosVals
{
	int Size = 0;
	int Vals[SudokuStride] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	bool IsValid()
	{
		return Size > 0;
	}

	void Init(const SudokuGrid &InGrid, int CellIdx)
	{
		int ValidVals[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

		GridContainer CellRow(InGrid, GridContainer::Type::Row, RowOfCell(CellIdx));
		GridContainer CellCol(InGrid, GridContainer::Type::Col, ColOfCell(CellIdx));
		GridContainer CellBox(InGrid, GridContainer::Type::Box, BoxOfCell(CellIdx));

		for (int Idx = 0; Idx < SudokuStride; Idx++)
		{
			int RowVal = CellRow.Vals[Idx];
			int ColVal = CellCol.Vals[Idx];
			int BoxVal = CellBox.Vals[Idx];
			if (0 != RowVal)
			{
				ValidVals[RowVal - 1] = 0;
			}
			if (0 != ColVal)
			{
				ValidVals[ColVal - 1] = 0;
			}
			if (0 != BoxVal)
			{
				ValidVals[BoxVal - 1] = 0;
			}
		}

		Size = 0;
		for (int Idx = 0; Idx < SudokuStride; Idx++)
		{
			if (0 != ValidVals[Idx])
			{
				Vals[Size++] = ValidVals[Idx];
			}
		}

		SimpleShuffle(Vals, Size);
	}

	PosVals(const SudokuGrid &InGrid, int CellIdx)
	{
		Init(InGrid, CellIdx);
	}
	PosVals() = default;
};

struct ActiveGuess
{
	int Idx = -1;
	int CurrGuess = -1;
	PosVals Guesses;

	bool HasNextGuess() const
	{
		return CurrGuess + 1 < Guesses.Size;
	}

	int NextGuess()
	{
		Assert(HasNextGuess());
		return Guesses.Vals[++CurrGuess];
	}

	int GetVal() const
	{
		Assert(CurrGuess < Guesses.Size);
		return Guesses.Vals[CurrGuess];
	}

	void Init(const SudokuGrid &InGrid, int InIdx)
	{
		Idx = InIdx;
		CurrGuess = 0;
		Guesses.Init(InGrid, InIdx);
	}

	ActiveGuess(const SudokuGrid &InGrid, int InIdx)
	{
		Init(InGrid, InIdx);
	}
	ActiveGuess() = default;
};

struct GuessList
{
	int LastGuess = -1;
	ActiveGuess Guesses[SudokuLength];

	int MaxGuess = -1;
	int GuessPerm[SudokuLength];

	const SudokuGrid &GridRef;

	GuessList(const SudokuGrid& InGrid)
		: LastGuess(-1)
		, GridRef(InGrid),
		MaxGuess(SudokuLength - GetNumCellsOccupied(InGrid))
	{
		int NumEmpty = 0;
		int NumFilled = 0;
		for (int Idx = 0; Idx < SudokuLength; Idx++)
		{
			if (GridRef[Idx] == 0)
			{
				GuessPerm[NumEmpty++] = Idx;
			}
			else
			{
				GuessPerm[(SudokuLength - 1) - NumFilled++] = 0;
			}
		}
		Assert(NumEmpty == MaxGuess);

		SimpleShuffle(GuessPerm, MaxGuess);
	}

	const ActiveGuess& CurrGuess() const
	{
		Assert(LastGuess >= 0);
		return Guesses[LastGuess];
	}

	const ActiveGuess& NewGuess()
	{
		//int NewGuessIdx = GetRandomEmptyCell(GridRef);
		Assert(LastGuess + 1 < MaxGuess);
		int NewGuessIdx = GuessPerm[++LastGuess];
		Guesses[LastGuess].Init(GridRef, NewGuessIdx);
		return Guesses[LastGuess];
	}

	const ActiveGuess& NextGuess()
	{
		int NextGuessVal = Guesses[LastGuess].NextGuess();
		return Guesses[LastGuess];
	}

	int Backtrack()
	{
		Assert(LastGuess >= 0);
		int IdxToEmpty = Guesses[LastGuess--].Idx;
		return IdxToEmpty;
	}
};

#define ENABLE_SUDO_PRINT() 1
#if ENABLE_SUDO_PRINT()
#define SUDO_PRINT(...) printf(__VA_ARGS__)
#else
#define SUDO_PRINT(...) (void)0
#endif // ENABLE_SUDO_PRINT()

bool CoreSudokuSolver::Solve(const SudokuGrid &InGrid)
{
	UnsolvedGrid = InGrid;

	NumAttempts()++;

	Assert(InGrid.IsValid());

	int NumCellsOccupied = GetNumCellsOccupied(InGrid);
	int NumEmptyCells = SudokuLength - NumCellsOccupied;

	SudokuGrid InProgressGrid = UnsolvedGrid;

	GuessList Guesses(InProgressGrid);

	// Does the current guess have another possible value?
	// 		If yes, try to proceed with that next value
	//		If no, backtrack one guess
	auto FindNextGuess = [&]() -> void
	{
		DBG_PRINT("\tBegin Backtracking\n");
		int NumBacktracked = 0;
		int GuessStartIdx = Guesses.LastGuess;
		while (!Guesses.CurrGuess().HasNextGuess())
		{
			int IdxToRemove = Guesses.Backtrack();
			InProgressGrid[IdxToRemove] = 0;
			DBG_PRINT("\t\tEmptying cell %d\n", IdxToRemove);
			NumBacktracked++;
		}
		DBG_PRINT("\t\tBacktracked to guess #%d\n", Guesses.LastGuess);
		const ActiveGuess& NextGuess = Guesses.NextGuess();
		InProgressGrid[NextGuess.Idx] = NextGuess.GetVal();

		int GuessEndIdx = Guesses.LastGuess;
		if (NumBacktracked > 0)
		{
			SUDO_PRINT("Backtracked %d guesses from [%d] to [%d]\n", NumBacktracked, GuessStartIdx, GuessEndIdx);
			SUDO_PRINT("\t{  ");
			for (int i = 0; i < NextGuess.Guesses.Size; i++)
			{
				if (i == NextGuess.CurrGuess)
				{
					SUDO_PRINT(">%d<  ", NextGuess.Guesses.Vals[i]);
				}
				else
				{
					SUDO_PRINT("%d  ", NextGuess.Guesses.Vals[i]);
				}
			}
			SUDO_PRINT("}\n");
		}

		DBG_PRINT("\t\tChose next guess: %d for cell [%d]\t(", NextGuess.GetVal(), NextGuess.Idx);
		DBG_PRINT("Idx %d in { ", NextGuess.CurrGuess);
		for (int i = 0; i < NextGuess.Guesses.Size; i++)
		{
			DBG_PRINT("%d ", NextGuess.Guesses.Vals[i]);
		}
		DBG_PRINT("}\n\tEnd Backtracking\n");
	};

	// Naive solution:
	//		- Keep track of guesses
	//		- Choose empty cell at random
	//		- Choose random possible value for that cell
	//		- Check if the grid is valid
	//			- If not, backtrack
	//			- If yes, repeat
	int LoopCount = 0;
	while (!InProgressGrid.IsSolved())
	{
		LoopCount++;
		Assert(Guesses.LastGuess < NumEmptyCells);
		Assert(Guesses.LastGuess < Guesses.MaxGuess);
		if (!InProgressGrid.IsValid())
		{
			DBG_PRINT("InProgressGrid NOT valid! Backtracking...\n");
			Assert(Guesses.LastGuess >= 0);
			FindNextGuess();
		}
		else
		{
			const ActiveGuess &NewGuess = Guesses.NewGuess();
			DBG_PRINT("Chose new guess (%d) at cell [%d]\n", Guesses.LastGuess, NewGuess.Idx);
			if (NewGuess.Guesses.Size > 0)
			{
				DBG_PRINT("\tTrying %d in cell [%d]...\n", NewGuess.GetVal(), NewGuess.Idx);
				DBG_PRINT("\t%d PosVals for cell [%d]: { ", NewGuess.Guesses.Size, NewGuess.Idx);
				for (int i = 0; i < NewGuess.Guesses.Size; i++)
				{
					DBG_PRINT("%d ", NewGuess.Guesses.Vals[i]);
				}
				DBG_PRINT("}\n");
				InProgressGrid[NewGuess.Idx] = NewGuess.GetVal();
			}
			else
			{
				DBG_PRINT("\tNo possible values for cell %d! Backtracking...\n", NewGuess.Idx);
				FindNextGuess();
			}
		}

/*
		if (LoopCount == 100)
		{
			Assert(false);
		}
*/
	}

	bSolved = InProgressGrid.IsSolved();
	NumSolves() += bSolved ? 1 : 0;

	SolvedGrid = InProgressGrid;

	return bSolved;
}

namespace SudokuUtils
{
	int GetRandomPosVal(const SudokuGrid &InGrid, int CellIdx)
	{
		PosVals PosValsForCell(InGrid, CellIdx);
		Assert(PosValsForCell.Size > 0);
		return PosValsForCell.Vals[0];
	}

	void GenRandomValues(SudokuGrid &InGrid, int NumValues)
	{
		int BeginNumOccupiedCells = GetNumCellsOccupied(InGrid);

		Assert(InGrid.IsValid());
		for (int i = 0; i < NumValues; i++)
		{
			int Idx = GetRandomEmptyCell(InGrid);
			InGrid.Grid[Idx] = GetRandomPosVal(InGrid, Idx);
		}

		int EndNumOccupiedCells = GetNumCellsOccupied(InGrid);

		Assert(EndNumOccupiedCells - BeginNumOccupiedCells == NumValues);
	}
}
