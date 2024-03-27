#include "Sudoku.h"

#include <stdio.h>

int RunPseudokuTests()
{
	Pseudoku TestGrid;

	printf("Before fill_grid:\n");
	TestGrid.Print();
	printf("After fill_grid:\n");
	TestGrid.Fill(0);
	TestGrid.Print();

	return 1;
}

int* Pseudoku::GetRow(int RowIdx)
{
	// returns ptr to beg of Row r (0-8)
	return &Grid[RowIdx * 9];
}

int* Pseudoku::GetCol(int ColIdx)
{
	// returns ptr to first cell of col c (0-8)
	return &Grid[ColIdx];
}

int* Pseudoku::GetBox(int BoxIdx)
{
	// returns ptr to first cell of Row b (0-8)
	return &Grid[(BoxIdx / 3 * 3 * 9) + (BoxIdx % 3 * 3)];
}

Neighbors Pseudoku::GetNeighbors(int CellIdx)
{
	// returns peers of cell n (0-80)
	Neighbors Peers;
	int* pRow = GetRow(CellIdx / 9);
	int* pCol = GetCol(CellIdx % 9);
	int* pBox = GetBox(CellIdx / 27 * 3 + (CellIdx % 9) / 3);
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

bool Pseudoku::IsSafe(int CellIdx, int Value)
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

PossibleVals Pseudoku::GetPosVals(int CellIdx)
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

int Pseudoku::Fill(int StartIdx)
{
	// fills cells StartIdx (0-80) to end of Sudoku
	if (0 <= StartIdx && StartIdx < 81)
	{
		if (Grid[StartIdx] != 0)
		{
			return Fill(StartIdx + 1);
		}
		else
		{
			PossibleVals PVs = GetPosVals(StartIdx);
			int i;
			for (i = 0; i < PVs.Size; i++)
			{
				Grid[StartIdx] = PVs.Vals[i];
				if (Fill(StartIdx + 1))
				{
					return (1);
				}
				else
				{
					Grid[StartIdx] = 0;
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

void Pseudoku::CopyFrom(const int InGrid[])
{
	for (int i = 0; i < 81; i++)
	{
		Grid[i] = InGrid[i];
	}
}

Pseudoku::Pseudoku()
{
	for (int i = 0; i < 81; i++)
	{
		Grid[i] = 0;
	}
}

Pseudoku::Pseudoku(const int InGrid[])
	: Grid()
{
	CopyFrom(InGrid);
}

Pseudoku::Pseudoku(const Pseudoku& Other)
	: Grid()
{
	*this = Other;
}

Pseudoku& Pseudoku::operator=(const Pseudoku& Other)
{
	CopyFrom(Other.Grid);
	return *this;
}

void Pseudoku::Print() const
{
	for (int i = 0; i < 81; i++)
	{
		if (i % 9 == 0 && i != 0)
		{
			printf("\n");
		}
		if (i % 27 == 0 && i != 0)
		{
			printf("------+-------+-------\n");
		}
		if (i % 3 == 0 && i % 9 != 0)
		{
			printf("| %d ", Grid[i]);
		}
		else
		{
			printf("%d ", Grid[i]);
		}
	}
	printf("\n");
}
