#include "stdafx.h"

#include "PieceSet.h"
#include "Game.h"

namespace Chess
{
	std::shared_ptr<Piece> PieceSet::at(const PieceID & id)
	{
		if (id.valid())
			return std::vector<std::shared_ptr<Piece>>::operator[](id);
		else
			return std::make_shared<Piece>(nullPiece);
	}
	const std::shared_ptr<Piece> PieceSet::at(const PieceID & id) const
	{
		if (id.valid())
			return std::vector<std::shared_ptr<Piece>>::operator[](id);
		else
			return std::make_shared<Piece>(nullPiece);
	}
	std::shared_ptr<Piece> PieceSet::at(const Position & pos)
	{
		return at(parentGame.getSquares().at(pos)->getPieceID());
	}
	const std::shared_ptr<Piece> PieceSet::at(const Position & pos) const
	{
		return at(parentGame.getSquares().at(pos)->getPieceID());
	}
}