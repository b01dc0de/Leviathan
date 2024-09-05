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
		return (RowOfCell(CellIdx) / SudokuBoxSize * SudokuBoxSize) + (ColOfCell(CellIdx) / SudokuBoxSize);
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

	struct DbgPrintTabScope
	{
		static int ScopeLevel;

		static int GetScopeLevel()
		{
			return ScopeLevel;
		}
		DbgPrintTabScope()
		{
			ScopeLevel++;
		}
		~DbgPrintTabScope()
		{
			ScopeLevel--;
		}
	};

	// CKA_NOTE: Only support up to 6 indent levels for now
	int DbgPrintTabScope::ScopeLevel = 0;
	constexpr int SudoDbgPrintVerboseLevel = 1;
	// -1: Completely off, no test print
	//  0: Debug printing off, test results only
	//  1: Minimal debug
	//  2: Full debug

	// CKA_NOTE: Definitions at bottom -- require PosVal
	// CKA_TODO: Figure out better organization
	int GetRandomPosVal(const SudokuGrid &InGrid, int CellIdx);
	void GenRandomValues(SudokuGrid &InGrid, int NumValues);
}
using namespace SudokuUtils;

#define ENABLE_SUDO_PRINT() 1
#if ENABLE_SUDO_PRINT()
	#pragma warning( push )
	#pragma warning(disable : 4984)
	#define SUDO_DBG_PRINT(VerboseLevel, ...) \
	{ \
		if constexpr (SudoDbgPrintVerboseLevel >= VerboseLevel) \
		{ \
			switch (DbgPrintTabScope::GetScopeLevel()) \
			{ \
				case 0: { DBG_PRINT(__VA_ARGS__); } break; \
				case 1: { DBG_PRINT("\t" __VA_ARGS__); } break; \
				case 2: { DBG_PRINT("\t\t" __VA_ARGS__); } break; \
				case 3: { DBG_PRINT("\t\t\t" __VA_ARGS__); } break; \
				case 4: { DBG_PRINT("\t\t\t\t" __VA_ARGS__); } break; \
				case 5: { DBG_PRINT("\t\t\t\t\t" __VA_ARGS__); } break; \
				case 6: { DBG_PRINT("\t\t\t\t\t\t" __VA_ARGS__); } break; \
				default: { DBG_PRINT(__VA_ARGS__); } break; \
			} \
		} \
	}
	#define SUDO_TEST_PRINT(...) SUDO_DBG_PRINT(0, __VA_ARGS__)

	#define SUDO_DBG_PRINT_INLINE(VerboseLevel, ...) \
		if constexpr (SudoDbgPrintVerboseLevel >= VerboseLevel) { DBG_PRINT(__VA_ARGS__); }
	#define SUDO_TEST_PRINT_INLINE(...) SUDO_DBG_PRINT_INLINE(0, __VA_ARGS__)
	#pragma warning( pop )
#else
	#define SUDO_DBG_PRINT(...) (void)0
	#define SUDO_TEST_PRINT(...) (void)0
	#define SUDO_DBG_PRINT(1, ...) (void)0
	#define SUDO_DBG_PRINT(2, ...) (void)0
#endif // ENABLE_SUDO_PRINT()

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

	constexpr int GridCellNumbers[SudokuLength] =
	{
		 0,  1,  2, /**/  3,  4,  5, /**/  6,  7,  8,
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
		int Result = 0;

		SUDO_TEST_PRINT("\nBEGIN SudokuTests::RunTests:\n");
		
		BruteForceSudokuSolver BruteForceSolver;

		auto SolveTest = [&](const SudokuGrid& SudoGrid)
		{
			DbgPrintTabScope();
			BruteForceSolver.Solve(SudoGrid);
			SUDO_TEST_PRINT("UnsolvedGrid:\n");
			BruteForceSolver.UnsolvedGrid.Print();
			SUDO_TEST_PRINT("SolvedGrid:\n");
			BruteForceSolver.SolvedGrid.Print();
		};

		constexpr bool bEnablePartialTest = true;
		if constexpr (bEnablePartialTest)
		{
			SudokuGrid NearFullGrid(NearFullGridA);

			SolveTest(NearFullGrid);
			Result += BruteForceSolver.bSolved ? 1 : 0;

			SudokuGrid PartialGrid(NearFullGridA);
			SolveTest(PartialGrid);

			Result += BruteForceSolver.bSolved ? 1 : 0;

			SudokuGrid NearEmptyGrid(TestNearEmptyGridA);
			SolveTest(NearEmptyGrid);

			Result += BruteForceSolver.bSolved ? 1 : 0;

			DbgPrintTabScope();
			SUDO_TEST_PRINT("\nSTATS:\n");
			DbgPrintTabScope();
			SUDO_TEST_PRINT("Attempts: %d\n", BruteForceSolver.NumAttempts());
			SUDO_TEST_PRINT("Solves: %d\n", BruteForceSolver.NumSolves());
			SUDO_TEST_PRINT("Guesses: %d\n", BruteForceSolver.NumGuesses());
			SUDO_TEST_PRINT("TotalTime: %.2f\n", BruteForceSolver.TotalTime());
		}

		constexpr bool bEnableScratchTest = false;
		if constexpr (bEnableScratchTest)
		{
			SudokuGrid StartingGrid;
			GenRandomValues(StartingGrid, SudokuGodNumber);
			Assert(StartingGrid.IsValid());

			BruteForceSudokuSolver CoreSolver;
			CoreSolver.Solve(StartingGrid);

			SUDO_TEST_PRINT("UnsolvedGrid:\n");
			CoreSolver.UnsolvedGrid.Print();

			SUDO_TEST_PRINT("\nSolvedGrid:\n");
			CoreSolver.SolvedGrid.Print();

			DbgPrintTabScope();
			SUDO_TEST_PRINT("\nSTATS:\n");
			DbgPrintTabScope();
			SUDO_TEST_PRINT("Attempts: %d\n", CoreSolver.NumAttempts());
			SUDO_TEST_PRINT("Solves: %d\n", CoreSolver.NumSolves());
			SUDO_TEST_PRINT("Guesses: %d\n", CoreSolver.NumGuesses());
			SUDO_TEST_PRINT("TotalTime: %.2f\n", CoreSolver.TotalTime());

			Result += CoreSolver.bSolved ? 1 : 0;
		}

		SUDO_TEST_PRINT("END SudokuTests::RunTests\n\n");

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
	DbgPrintTabScope();
	for (int r = 0; r < SudokuStride; r++)
	{
		int StartIdx = r * SudokuStride;
		SUDO_TEST_PRINT("%d %d %d | %d %d %d | %d %d %d\n",
			   Grid[StartIdx + 0],
			   Grid[StartIdx + 1],
			   Grid[StartIdx + 2],
			   Grid[StartIdx + 3],
			   Grid[StartIdx + 4],
			   Grid[StartIdx + 5],
			   Grid[StartIdx + 6],
			   Grid[StartIdx + 7],
			   Grid[StartIdx + 8]);

		constexpr int FirstBoxEndRow = SudokuBoxSize - 1;
		constexpr int SecondBoxEndRow = (SudokuBoxSize * 2) - 1;
		constexpr int ThirdBoxEndRow = (SudokuBoxSize * 3) - 1;
		if (r == FirstBoxEndRow || r == SecondBoxEndRow)
		{
			SUDO_TEST_PRINT("- - - | - - - | - - -\n");
		}
		else if (r == ThirdBoxEndRow)
		{
			SUDO_TEST_PRINT("\n\n");
		}
	}
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

int BruteForceSudokuSolver::_NumAttempts = 0;
int BruteForceSudokuSolver::_NumSolves = 0;
int BruteForceSudokuSolver::_NumGuesses = 0;
float BruteForceSudokuSolver::_TotalTime = 0.0f;

int &BruteForceSudokuSolver::NumAttempts()
{
	return _NumAttempts;
}

int &BruteForceSudokuSolver::NumSolves()
{
	return _NumSolves;
}

int &BruteForceSudokuSolver::NumGuesses()
{
	return _NumGuesses;
}

float &BruteForceSudokuSolver::TotalTime()
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
		, GridRef(InGrid)
		, MaxGuess(SudokuLength - GetNumCellsOccupied(InGrid))
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
		Assert(LastGuess + 1 < MaxGuess);
		int NewGuessIdx = GuessPerm[++LastGuess];
		Guesses[LastGuess].Init(GridRef, NewGuessIdx);
		return Guesses[LastGuess];
	}

	const ActiveGuess& NextGuess()
	{
		int NextGuessVal = Guesses[LastGuess].NextGuess();
		return Guesses[NextGuessVal];
	}

	int Backtrack()
	{
		Assert(LastGuess >= 0);
		int IdxToEmpty = Guesses[LastGuess--].Idx;
		return IdxToEmpty;
	}
};

bool BruteForceSudokuSolver::Solve(const SudokuGrid &InGrid)
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
		DbgPrintTabScope TS_DBG_0;
		SUDO_DBG_PRINT(2, "Begin Backtracking\n");
		DbgPrintTabScope TS_DBG_1;
		static int TotalBacktracked = 0;
		int NumBacktracked = 0;
		int GuessStartIdx = Guesses.LastGuess;
		while (!Guesses.CurrGuess().HasNextGuess())
		{
			int IdxToRemove = Guesses.Backtrack();
			InProgressGrid[IdxToRemove] = 0;
			SUDO_DBG_PRINT(2, "Emptying cell %d\n", IdxToRemove);
			NumBacktracked++;
		}
		SUDO_DBG_PRINT(2, "Backtracked to guess #%d\n", Guesses.LastGuess);
		TotalBacktracked += NumBacktracked;
		const ActiveGuess& NextGuess = Guesses.NextGuess();
		InProgressGrid[NextGuess.Idx] = NextGuess.GetVal();
		NumGuesses()++;

		int GuessEndIdx = Guesses.LastGuess;
		if (NumBacktracked > 0)
		{
			SUDO_DBG_PRINT(1, "Backtracked %d guesses from [%d] to [%d]: { ", NumBacktracked, GuessStartIdx, GuessEndIdx);
			for (int Idx = 0; Idx < NextGuess.Guesses.Size; Idx++)
			{
				SUDO_DBG_PRINT_INLINE(1, Idx == NextGuess.CurrGuess ? "[%d]  " : "%d  ", NextGuess.Guesses.Vals[Idx]);
			}
			SUDO_DBG_PRINT_INLINE(1, "}\n");
		}
		else
		{
			DbgPrintTabScope TS_DBG_2;
			SUDO_DBG_PRINT(2, "Chose next guess: %d for cell [%d]    (", NextGuess.GetVal(), NextGuess.Idx);
			SUDO_DBG_PRINT_INLINE(2, "Idx %d in { ", NextGuess.CurrGuess);
			for (int i = 0; i < NextGuess.Guesses.Size; i++)
			{
				SUDO_DBG_PRINT_INLINE(2, "%d ", NextGuess.Guesses.Vals[i]);
			}
			SUDO_DBG_PRINT_INLINE(2, "} )\n");
		}
		SUDO_DBG_PRINT(2, "End Backtracking\n");
	};

	// Naive solution:
	//		- Keep track of guesses
	//		- Choose empty cell at random
	//		- Choose random possible value for that cell
	//		- Check if the grid is valid
	//			- If not, backtrack
	//			- If yes, repeat
	int LoopCount = 0;
	DbgPrintTabScope TS_DBG_0;
	while (!InProgressGrid.IsSolved())
	{
		LoopCount++;
		Assert(Guesses.LastGuess < NumEmptyCells);
		Assert(Guesses.LastGuess < Guesses.MaxGuess);
		DbgPrintTabScope TS_DBG_1;
		if (!InProgressGrid.IsValid())
		{
			SUDO_DBG_PRINT(2, "InProgressGrid NOT valid! Backtracking...\n");
			Assert(Guesses.LastGuess >= 0);
			FindNextGuess();
		}
		else
		{
			const ActiveGuess &NewGuess = Guesses.NewGuess();
			SUDO_DBG_PRINT(2, "Chose new guess (%d) at cell [%d]\n", Guesses.LastGuess, NewGuess.Idx);
			if (NewGuess.Guesses.Size > 0)
			{
				DbgPrintTabScope TS_DBG_2;
				SUDO_DBG_PRINT(2, "Trying %d in cell [%d]...\n", NewGuess.GetVal(), NewGuess.Idx);
				SUDO_DBG_PRINT(2, "%d PosVals for cell [%d]: { ", NewGuess.Guesses.Size, NewGuess.Idx);
				for (int i = 0; i < NewGuess.Guesses.Size; i++)
				{
					SUDO_DBG_PRINT_INLINE(2, "%d ", NewGuess.Guesses.Vals[i]);
				}
				SUDO_DBG_PRINT_INLINE(2, "}\n");
				InProgressGrid[NewGuess.Idx] = NewGuess.GetVal();
				NumGuesses()++;
			}
			else
			{
				DbgPrintTabScope TS_DBG_2;
				SUDO_DBG_PRINT(2, "No possible values for cell %d! Backtracking...\n", NewGuess.Idx);
				FindNextGuess();
			}
		}
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
