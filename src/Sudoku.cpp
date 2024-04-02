#include "Common.h"

#include "Sudoku.h"

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

bool CoreSudokuSolver::Solve(const SudokuGrid& InGrid)
{
	UnsolvedGrid = InGrid;

	NumAttempts()++;

	SudokuGrid InProgressGrid = UnsolvedGrid;

	while (!InProgressGrid.IsSolved())
	{
		// CKA_TODO
		break;
	}

	bSolved = true; // CKA_TODO
	NumSolves() += bSolved ? 1 : 0;

	SolvedGrid = InProgressGrid;

	return bSolved;
}
