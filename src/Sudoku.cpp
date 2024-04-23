#include "Common.h"

#include "Sudoku.h"

namespace Sudoku
{
	int RunTests()
	{
		return 0;
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
	int Vals[ContainerLength] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	bool IsValid();

	GridContainer() = default;
	GridContainer(const SudokuGrid& InGrid, Type InType, int Idx);
};

bool GridContainer::IsValid()
{
	bool Result = true;
	int ValRef[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	for (int Idx = 0; Idx < ContainerLength; Idx++)
	{
		int CurrVal = Vals[Idx];
		if (0 == CurrVal)
		{
			continue;
		} else if (0 == ValRef[CurrVal - 1])
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

GridContainer::GridContainer(const SudokuGrid& InGrid, Type InType, int Idx)
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
					Vals[ValIdx++] = InGrid.Cell(RowIdx, ColIdx);
				}
			}
		} break;
		case Type::None:
		default:
		{
			Assert(false);
		} break;
	}
}

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

const int& SudokuGrid::Cell(int r, int c) const
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

bool SudokuGrid::IsValid()
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

bool SudokuGrid::IsSolved()
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

int& CoreSudokuSolver::NumAttempts()
{
	return _NumAttempts;
}

int& CoreSudokuSolver::NumSolves()
{
	return _NumSolves;
}

int& CoreSudokuSolver::NumGuesses()
{
	return _NumGuesses;
}

float& CoreSudokuSolver::TotalTime()
{
	return _TotalTime;
}

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

int GetRandomCellIdx()
{
	static std::random_device rand_dev;
	static std::mt19937 mersenne_twister_eng(rand_dev());
	static std::uniform_int_distribution<int> uni_int_dist(0, SudokuLength - 1);

	return uni_int_dist(mersenne_twister_eng);
}

int GetRandomEmptyCell(const SudokuGrid& InGrid)
{
	int EmptyCellIdx = -1;

	do
	{
		EmptyCellIdx = GetRandomCellIdx();
	}
	while (InGrid.Grid[EmptyCellIdx] != 0);

	return EmptyCellIdx;
}

}

using namespace SudokuUtils;

struct CellVal
{
	int Idx;
	int Val;
};

struct PosVals
{
	int Size = 0;
	int Vals[SudokuStride] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	void Init(const SudokuGrid& InGrid, int CellIdx)
	{
		int ValidVals[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		GridContainer CellRow(InGrid, GridContainer::Type::Row, RowOfCell(CellIdx));
		GridContainer CellCol(InGrid, GridContainer::Type::Col, ColOfCell(CellIdx));
		GridContainer CellBox(InGrid, GridContainer::Type::Box, BoxOfCell(CellIdx));

		for (int Idx = 0; Idx < SudokuStride; Idx++)
		{
			int RowVal = CellRow.Vals[Idx];
			int ColVal = CellCol.Vals[Idx];
			int BoxVal = CellBox.Vals[Idx];
			if (0 != RowVal) { ValidVals[RowVal - 1] = 0; }
			if (0 != ColVal) { ValidVals[ColVal - 1] = 0; }
			if (0 != BoxVal) { ValidVals[BoxVal - 1] = 0; }
		}

		// CKA_TODO: Shuffle
		for (int Idx = 0; Idx < SudokuStride; Idx++)
		{
			if (0 != ValidVals[Idx]) { Vals[Size++] = ValidVals[Idx]; }
		}
	}

	PosVals(const SudokuGrid& InGrid, int CellIdx)
	{
		Init(InGrid, CellIdx);
	}
	PosVals() = default;
};

struct ActiveGuess
{
	int Idx = -1;
	int CurrGuess = 0;
	PosVals Guesses;

	bool HasNextGuess()
	{
		return CurrGuess + 1 < Guesses.Size;
	}

	int NextGuess()
	{
		Assert(HasNextGuess());
		return Guesses.Vals[++CurrGuess];
	}

	int GetCurrGuess()
	{
		return Guesses.Vals[CurrGuess];
	}

	void Init(const SudokuGrid& InGrid, int InIdx)
	{
		Idx = InIdx;
		CurrGuess = 0;
		Guesses.Init(InGrid, InIdx);
	}

	ActiveGuess(const SudokuGrid& InGrid, int InIdx)
	{
		Init(InGrid, InIdx);
	}
	ActiveGuess() = default;
};

struct GuessList
{
	int LastGuess = -1;
	ActiveGuess GuessGrid[SudokuLength];
	const SudokuGrid& GridRef;

	GuessList(const SudokuGrid& InGrid)
		: GridRef(InGrid) { }

	CellVal NewGuess()
	{
		int NewGuessIdx = GetRandomEmptyCell(GridRef);
		GuessGrid[++LastGuess].Init(GridRef, NewGuessIdx);
		return CellVal{ GuessGrid[LastGuess].Idx, GuessGrid[LastGuess].GetCurrGuess() };
	}

	CellVal NextGuess()
	{
		int NextGuessVal = GuessGrid[LastGuess].NextGuess();
		return CellVal{ GuessGrid[LastGuess].Idx, NextGuessVal };
	}
};

bool CoreSudokuSolver::Solve(const SudokuGrid& InGrid)
{
	UnsolvedGrid = InGrid;

	NumAttempts()++;

	SudokuGrid InProgressGrid = UnsolvedGrid;

	GuessList Guesses(InProgressGrid);

	// Naive solution:
	//		- Keep track of guesses
	//		- Choose empty cell at random
	//		- Choose random possible value for that cell
	//		- Check if the grid is valid
	//			- If not, backtrack
	//			- If yes, repeat
	while (!InProgressGrid.IsSolved())
	{
	}

	bSolved = true; // CKA_TODO
	NumSolves() += bSolved ? 1 : 0;

	SolvedGrid = InProgressGrid;

	return bSolved;
}
