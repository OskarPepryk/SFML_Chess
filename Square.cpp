#include "stdafx.h"

#include "Square.h"
#include "Piece.h"
#include "Game.h"

using namespace Chess;


std::string Square::identify() const
{
	std::string name;
	//lowercase letter for column
	name += static_cast<char>(m_column + 97);
	//number for row
	name += static_cast<char>(m_row + 49);
	return name;
}

bool Square::isAtacked(Game * board, Side bySide)
{
	if (!board)
		throw "Board<Square*> or Piece was null in Chess::Square::isAtacked()\n";

	for (const Piece *Piece : board->getPieces())
	{
		if (Piece->getSide() != bySide)
			continue;

		//Get list of valid moves
		std::vector<Square*> moves = Piece->getPseudoLegalMoves(board);
		//Check if this square is in valid moves of this Piece.
		auto it = std::find(moves.begin(), moves.end(), this);

		if (it != moves.end())
		{
			return true;
		}
	}

	return false;
}
