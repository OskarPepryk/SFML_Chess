#include "stdafx.h"

#include "BoardMatrix.h"
#include "Game.h"

namespace Chess
{
	Square & Board::at(int row, int column)
	{
		if ((row >= 0 and row < static_cast<int>(array.size()))
			and (column >= 0 and column < static_cast<int>(array[row].size())))
			return *array[row][column];
		else
			return nullSquare;
	}
	const Square & Board::at(int row, int column) const
	{
		if ((row >= 0 and row < static_cast<int>(array.size()))
			and (column >= 0 and column < static_cast<int>(array[row].size())))
			return *array[row][column];
		else
			return nullSquare;
	}
	Square & Board::at(const Position && pos)
	{
		if (pos.valid())
			return *array[pos.row][pos.column];
		else
			return nullSquare;
	}
	const Square & Board::at(const Position && pos) const
	{
		if (pos.valid())
			return *array[pos.row][pos.column];
		else
			return nullSquare;
	}
	Square & Board::at(const Position & pos)
	{
		if (pos.valid())
			return *array[pos.row][pos.column];
		else
			return nullSquare;
	}
	const Square & Board::at(const Position & pos) const
	{
		if (pos.valid())
			return *array[pos.row][pos.column];
		else
			return nullSquare;
	}
	Square & Board::at(const PieceID & id)
	{
		return at(parentGame.getPieces().at(id)->getPos());
	}
	const Square & Board::at(const PieceID & id) const
	{
		return at(parentGame.getPieces().at(id)->getPos());
	}
	Square & Board::at(const PieceID && id)
	{
		return at(parentGame.getPieces().at(id)->getPos());
	}
	const Square & Board::at(const PieceID && id) const
	{
		return at(parentGame.getPieces().at(id)->getPos());
	}
	Square *& Board::set(int row, int column)
	{
		if ((row >= 0 and row < static_cast<int>(array.size()))
			and (column >= 0 and column < static_cast<int>(array[row].size())))
		{
			return array[row][column];
		}
		else
		{
			throw (std::out_of_range("Tried to assign square at out of range index."));
		}
	}
}