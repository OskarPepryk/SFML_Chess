#include "stdafx.h"

#include "BoardMatrix.h"
#include "Game.h"

namespace Chess
{
	//std::shared_ptr<Square> Board::at(int row, int column)
	//{
	//	if ((row >= 0 and row < static_cast<int>(array.size()))
	//		and (column >= 0 and column < static_cast<int>(array[row].size())))
	//		return *array[row][column];
	//	else
	//		return nullSquare;
	//}
	const std::shared_ptr<Square> Board::at(int row, int column) const
	{
		if ((row >= 0 and row < 8)
			and (column >= 0 and column < 8))
			return array[row * 8 + column];
		else
			return std::make_shared<Square>(nullSquare);
	}
	//std::shared_ptr<Square> Board::at(const Position && pos)
	//{
	//	if (pos.valid())
	//		return *array[pos.row][pos.column];
	//	else
	//		return nullSquare;
	//}
	const std::shared_ptr<Square> Board::at(const Position && pos) const
	{
		if (pos.valid())
			return array[pos.row * 8 + pos.column];
		else
			return std::make_shared<Square>(nullSquare);
	}
	//std::shared_ptr<Square> Board::at(const Position & pos)
	//{
	//	if (pos.valid())
	//		return *array[pos.row][pos.column];
	//	else
	//		return nullSquare;
	//}
	const std::shared_ptr<Square> Board::at(const Position & pos) const
	{
		if (pos.valid())
			return array[pos.row * 8 + pos.column];
		else
			return std::make_shared<Square>(nullSquare);
	}
	//std::shared_ptr<Square> Board::at(const PieceID & id)
	//{
	//	return at(parentGame.getPieces().at(id)->getPos());
	//}
	const std::shared_ptr<Square> Board::at(const PieceID & id) const
	{
		return at(parentGame.getPieces().at(id)->getPos());
	}
	//std::shared_ptr<Square> Board::at(const PieceID && id)
	//{
	//	return at(parentGame.getPieces().at(id)->getPos());
	//}
	const std::shared_ptr<Square> Board::at(const PieceID && id) const
	{
		return at(parentGame.getPieces().at(id)->getPos());
	}
	std::shared_ptr<Square>& Board::set(int row, int column)
	{
		if ((row >= 0 and row < 8)
			and (column >= 0 and column < 8))
		{
			return array[row * 8 + column];
		}
		else
		{
			throw (std::out_of_range("Tried to assign square at out of range index."));
		}
	}
}