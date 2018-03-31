#include "stdafx.h"

#include <algorithm>

#include "Piece.h"
#include "Game.h"

using namespace Chess;

Position& Chess::Piece::getTakenSquare()
{
	return m_pos;
}

const Position& Chess::Piece::getTakenSquare() const
{
	return m_pos;
}

void Chess::Piece::setTakenSquare(Position &position, Board * board)
{
	m_hasMoved = true;
	m_pos = position;
}

void Chess::Piece::setTakenSquare(Position && position)
{
	m_hasMoved = true;
	m_pos = position;
}

void Piece::getPseudoLegalMovesInDirection(Game & game, std::vector<Position> &validSquares, Directions::DirectionSet dirSet, int maxRange, bool canJumpOver) const
{
	Board &squares = game.getSquares();
	const auto &pieces = game.getPieces();

	int column = m_pos.column;
	int row = m_pos.row;

	for (Directions::Direction dir : dirSet)
	{
		bool foundPiece = false;
		//Check in range of 1
		for (int range = 1; range <= maxRange and (!foundPiece or canJumpOver); range++)
		{
			//Unless square is not valid
		
			const Square & square = squares.at(row + range * dir.up,column + range * dir.right);
			if (square.isValid())
				//If it has any Piece on it
			{
				if (square.getPieceID().valid())
				{
					//Don't look in this direction anymoreif a Piece is there
					foundPiece = true;
					//If the Piece is opponent
					if (pieces.at(square.getPieceID())->getSide() != m_side)
					{
						validSquares.push_back(square.getPos());
					}
				}
				else
					validSquares.push_back(square.getPos());
			}
		}
	}
}
//DONE:
//Add parameter to not check for moves that could not take a piece (castling). It should fix infinite loop with functions getPseudoLegalMoves <-> isAttacked
//This parameter is to be used in isAttacked function.
std::vector<Position> Piece::getPseudoLegalMoves(Game & game, bool includeNonTakingMoves) const
{
	Board &squares = game.getSquares();
	const auto &pieces = game.getPieces();

	std::vector<Position> validSquares;

	if (!m_pos.valid())
		return validSquares;


	int column = m_pos.column;
	int row = m_pos.row;

	switch (m_type)
	{
	case Type::King:
	{
		{
			getPseudoLegalMovesInDirection(game, validSquares, Directions::allDirections, 1);
			//Castling check
			if (!m_hasMoved and column == 4 and includeNonTakingMoves)
			{	
				//Get enemy side
				Side enemySide = m_side;
				++enemySide;

				//Kingside castling
				if (!squares.at(row, 5).getPieceID().valid() and
					!squares.at(row, 6).getPieceID().valid() and
					squares.at(row, 7).getPieceID().valid() and
					pieces.at(squares.at(row, 7).getPieceID())->getType() == Type::Rook and
					!pieces.at(squares.at(row, 7).getPieceID())->getMoved() and
					!squares.at(row, 5).isAttacked(game, enemySide) and
					!squares.at(row, 6).isAttacked(game, enemySide)
					)
					validSquares.push_back(Position{ row,6 });

				//Queenside castling
				if (!squares.at(row, 2).getPieceID().valid() and
					!squares.at(row, 3).getPieceID().valid() and
					squares.at(row, 0).getPieceID().valid() and
					pieces.at(squares.at(row, 0).getPieceID())->getType() == Type::Rook and
					!pieces.at(squares.at(row, 0).getPieceID())->getMoved() and 
					!squares.at(row, 2).isAttacked(game, enemySide) and
					!squares.at(row, 3).isAttacked(game, enemySide)
					)
					validSquares.push_back(Position{ row,2 });
			}
			break;
		}
	}
	case Type::Bishop:
	{
		getPseudoLegalMovesInDirection(game, validSquares, Directions::diagonals, 8);
		break;
	}
	case Type::Rook:
	{
		getPseudoLegalMovesInDirection(game, validSquares, Directions::rightAngles, 8);
		break;
	}
	case Type::Queen:
	{
		getPseudoLegalMovesInDirection(game, validSquares, Directions::allDirections, 8);
		break;
	}
	case Type::Knight:
	{
		getPseudoLegalMovesInDirection(game, validSquares, Directions::knight, 1, true);
		break;
	}
	case Piece::Type::Pawn:
	default:
	{
		//TODO Implement en passant
		//White pawns and black pawns move in different directions:
		//TODO Unhardcode movement direction in case white starts on top
		int moveDirection;
		if (m_side == Side::White)
			moveDirection = 1;
		else
			moveDirection = -1;

		//Square in front
		if (squares.at(row + moveDirection, column).isValid() and 
			!squares.at(row + moveDirection, column).getPieceID().valid())
			validSquares.push_back(Position(row + moveDirection, column));

		// or 2 square first move
		if (!m_hasMoved)
		{
			if (squares.at(row + 2 * moveDirection, column).isValid() and
				squares.at(row + 1 * moveDirection, column).isValid() and
				!squares.at(row + 2 * moveDirection, column).getPieceID().valid() and
				!squares.at(row + 1 * moveDirection, column).getPieceID().valid())
				validSquares.push_back(Position(row + 2 * moveDirection,column));
		}


		//Diagonal left
		Position diag1(row + moveDirection, column - 1);
		Square & diagonal1 = squares.at(diag1);

		if (diagonal1.getPieceID().valid())
		{
			if (pieces.at(diagonal1.getPieceID())->getSide() != m_side)
				validSquares.push_back(diag1);
		} 
		else
		if (diagonal1.getEnPassantPieceID().valid())
		{
			if (pieces.at(diagonal1.getEnPassantPieceID())->getSide() != m_side)
				validSquares.push_back(diag1);
		}

		//Diagonal right
		Position diag2(row + moveDirection, column + 1);
		Square & diagonal2 = squares.at(diag2);

		if (diagonal2.getPieceID().valid())
		{
			if (pieces.at(diagonal2.getPieceID())->getSide() != m_side)
				validSquares.push_back(diag2);
		}
		else
		if (diagonal2.getEnPassantPieceID().valid())
		{
			if (pieces.at(diagonal2.getEnPassantPieceID())->getSide() != m_side)
				validSquares.push_back(diag2);
		}

		break;
	}
	}
	return validSquares;
}

std::vector<Position> Chess::Piece::getLegalMoves(Game & game) const
{
	std::vector<Position> PseudoLegalMoves = getPseudoLegalMoves(game);

	//Filter out the moves that would leave king in check
	
	for (auto it = PseudoLegalMoves.begin(); it!=PseudoLegalMoves.end(); )
	{
		Position & move = *it;
		//Copy game to check future move:
		Game gameCopy(game);

		//Find our piece in copied board and target square
		PieceID thisCopy = m_id;
		Position originPos = m_pos;
		Position targetPos = move;

		bool stillChecked = false;

		if (m_id.valid() and originPos.valid() and targetPos.valid())
		{
			gameCopy.movePiece(m_pos, move);

			if (gameCopy.getChecked(m_side))
				stillChecked = true;
		}

		if (stillChecked)
		{
			it = PseudoLegalMoves.erase(it);
		}
		else
			it++;
	}
	
	return PseudoLegalMoves;
}

bool Piece::isAttacked(Game & game)
{
	//TODO Don't check for moves of friendly pieces

	for (const Piece *Piece : game.getPieces())
	{
		//Dont check for danger from itself
		if (Piece == this)
			continue;

		//Get list of valid moves
		//Dont check for moves that cant take a piece anyway (castling).
		std::vector<Position> moves = Piece->getPseudoLegalMoves(game, false);
		//Check if square of checked Piece is in valid moves of any Piece.
		//Todo filter out friendly Piece_nondrawables
		auto it = std::find(moves.begin(), moves.end(), m_pos);

		if (it != moves.end())
		{
			return true;
		}
	}

	return false;
}