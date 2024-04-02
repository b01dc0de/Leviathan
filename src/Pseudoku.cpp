#include "Common.h"

#include "Pseudoku.h"

#define ROW_OF(r) {r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8]}
#define COL_OF(c) {c[0], c[9], c[18], c[27], c[36], c[45], c[54], c[63], c[72]}
#define BOX_OF(b) {b[0], b[1], b[2], b[9], b[10], b[11], b[18], b[19], b[20]}

using namespace Pseudoku;

int PseudokuNew::_NumAttempts = 0;
int PseudokuNew::_NumSolves = 0;
int PseudokuNew::_NumGuesses = 0;
float PseudokuNew::_TotalTime = 0.f;

int& PseudokuNew::NumAttempts()
{
	return _NumAttempts;
}

int& PseudokuNew::NumSolves()
{
	return _NumSolves;
}

int& PseudokuNew::NumGuesses()
{
	return _NumGuesses;
}

float& PseudokuNew::TotalTime()
{
	return _TotalTime;
}

bool PseudokuNew::Solve(const SudokuGrid& InGrid)
{
	UnsolvedGrid = InGrid;

	NumAttempts()++;
	float BeginTime = 0.0f;

	LocalGrid = UnsolvedGrid;
	bSolved = Fill(0);
	SolvedGrid = LocalGrid;
	float EndTime = 0.0f;

	NumSolves() += (int)bSolved;
	TotalTime() += EndTime - BeginTime;

	return bSolved;
}

int PseudokuNew::RunTests()
{
	int Result = 0;

	printf("\nBEGIN Pseudoku::RunTests:\n");

	SudokuGrid EmptyGrid;
	PseudokuNew Test;

	printf("\tBefore:\n");
	Test.Print();

	Test.Solve(EmptyGrid);

	printf("\tAfter:\n");
	Test.Print();

	printf("\n\tSTATS:\n");
	printf("\t\tAttempts: %d\n", _NumAttempts);
	printf("\t\tSolves: %d\n", _NumSolves);
	printf("\t\tGuesses: %d\n", _NumGuesses);
	printf("\t\tTotalTime: %.2f\n", _TotalTime);

	printf("END Pseudoku::RunTests\n\n");

	return Result;
}

int* PseudokuNew::GetRowPtr(int RowIdx)
{
	// returns ptr to beg of Row r (0-8)
	return &LocalGrid[RowIdx * 9];
}

int* PseudokuNew::GetColPtr(int ColIdx)
{
	// returns ptr to first cell of col c (0-8)
	return &LocalGrid[ColIdx];
}

int* PseudokuNew::GetBoxPtr(int BoxIdx)
{
	// returns ptr to first cell of Row b (0-8)
	return &LocalGrid[(BoxIdx / 3 * 3 * 9) + (BoxIdx % 3 * 3)];
}

Neighbors PseudokuNew::GetNeighbors(int CellIdx)
{
	// returns peers of cell n (0-80)
	Neighbors Peers;
	int* pRow = GetRowPtr(CellIdx / 9);
	int* pCol = GetColPtr(CellIdx % 9);
	int* pBox = GetBoxPtr(CellIdx / 27 * 3 + (CellIdx % 9) / 3);
	int Row[9] = ROW_OF(pRow);
	int Col[9] = COL_OF(pCol);
	int Box[9] = BOX_OF(pBox);
	for (int i = 0; i < 9; i++)
	{
		Peers.Row[i] = Row[i];
		Peers.Col[i] = Col[i];
		Peers.Box[i] = Box[i];
	}
	return Peers;
}

bool PseudokuNew::IsSafe(int CellIdx, int Value)
{// returns whether or not value v (1-9) in cell n (0-80) is safe
	Neighbors Peers = GetNeighbors(CellIdx);
	for (int i = 0; i < 9; i++)
	{
		if (Peers.Row[i] == Value ||
			Peers.Col[i] == Value ||
			Peers.Box[i] == Value)
		{
			return false;
		}
	}
	return true;
}

PossibleVals PseudokuNew::GetPosVals(int CellIdx)
{// returns pos struct containing possible values in cell n (0-80)
	PossibleVals PVs;
	int i, s;
	for (i = 0, s = 0; i < 9; i++)
	{
		if (IsSafe(CellIdx, i + 1))
		{
			PVs.Vals[s] = i + 1;
			s++;
		}

	}
	PVs.Size = s;
	return PVs;
}

int PseudokuNew::Fill(int StartIdx)
{
	// fills cells StartIdx (0-80) to end of Sudoku
	if (0 <= StartIdx && StartIdx < 81)
	{
		if (LocalGrid[StartIdx] != 0)
		{
			return Fill(StartIdx + 1);
		}
		else
		{
			PossibleVals PVs = GetPosVals(StartIdx);
			int i;
			for (i = 0; i < PVs.Size; i++)
			{
				LocalGrid[StartIdx] = PVs.Vals[i];
				NumGuesses()++;
				if (Fill(StartIdx + 1))
				{
					return (1);
				}
				else
				{
					LocalGrid[StartIdx] = 0;
				}
			}
			return false;
		}
	}
	else
	{
		return true;
	}

}

void PseudokuNew::CopyFrom(const int InGrid[])
{
	for (int i = 0; i < 81; i++)
	{
		LocalGrid[i] = InGrid[i];
	}
}

PseudokuNew::PseudokuNew()
{
	for (int i = 0; i < 81; i++)
	{
		LocalGrid[i] = 0;
	}
}

PseudokuNew::PseudokuNew(const int InGrid[])
	: LocalGrid()
{
	CopyFrom(InGrid);
}

PseudokuNew::PseudokuNew(const PseudokuNew& Other)
	: LocalGrid()
{
	*this = Other;
}

PseudokuNew& PseudokuNew::operator=(const PseudokuNew& Other)
{
	CopyFrom(Other.LocalGrid.Grid);
	return *this;
}

void PseudokuNew::Print() const
{
	for (int i = 0; i < 81; i++)
	{
		if (i == 0)
		{
			printf("\t\t");
		}
		if (i % 9 == 0 && i != 0)
		{
			printf("\n\t\t");
		}
		if (i % 27 == 0 && i != 0)
		{
			printf("------+-------+-------\n\t\t");
		}
		if (i % 3 == 0 && i % 9 != 0)
		{
			printf("| %d ", LocalGrid[i]);
		}
		else
		{
			printf("%d ", LocalGrid[i]);
		}
	}
	printf("\n");
}

