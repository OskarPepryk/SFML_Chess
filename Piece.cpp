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

void Chess::Piece::setTakenSquare(Position &position)
{
	m_hasMoved = true;
	m_pos = position;
}

void Chess::Piece::setTakenSquare(Position && position)
{
	m_hasMoved = true;
	m_pos = position;
}

void Piece::getPseudoLegalMovesInDirection(std::vector<Position> &validSquares, Directions::DirectionSet dirSet, int maxRange, bool canJumpOver) const
{
	Board &squares = m_parentGame.get().getSquares();
	const auto &pieces = m_parentGame.get().getPieces();

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
std::vector<Position> Piece::getPseudoLegalMoves(bool includeNonTakingMoves) const
{
	Board &squares = m_parentGame.get().getSquares();
	const auto &pieces = m_parentGame.get().getPieces();

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
			getPseudoLegalMovesInDirection(validSquares, Directions::allDirections, 1);
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
					!squares.at(row, 5).isAttacked(m_parentGame, enemySide) and
					!squares.at(row, 6).isAttacked(m_parentGame, enemySide)
					)
					validSquares.push_back(Position{ row,6 });

				//Queenside castling
				if (!squares.at(row, 2).getPieceID().valid() and
					!squares.at(row, 3).getPieceID().valid() and
					squares.at(row, 0).getPieceID().valid() and
					pieces.at(squares.at(row, 0).getPieceID())->getType() == Type::Rook and
					!pieces.at(squares.at(row, 0).getPieceID())->getMoved() and 
					!squares.at(row, 2).isAttacked(m_parentGame, enemySide) and
					!squares.at(row, 3).isAttacked(m_parentGame, enemySide)
					)
					validSquares.push_back(Position{ row,2 });
			}
			break;
		}
	}
	case Type::Bishop:
	{
		getPseudoLegalMovesInDirection(validSquares, Directions::diagonals, 8);
		break;
	}
	case Type::Rook:
	{
		getPseudoLegalMovesInDirection(validSquares, Directions::rightAngles, 8);
		break;
	}
	case Type::Queen:
	{
		getPseudoLegalMovesInDirection(validSquares, Directions::allDirections, 8);
		break;
	}
	case Type::Knight:
	{
		getPseudoLegalMovesInDirection(validSquares, Directions::knight, 1, true);
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

const std::vector<Position>& Chess::Piece::getLegalMoves() const
{
	return m_legalMoves;
}

//Bool pseudolegal	- to break infinite loop of checking for checks that checks for legalMoves that checks for checks...
//					- intended for use in copied simulated game
void Chess::Piece::refreshLegalMoves(bool pseudoLegal)
{
	m_legalMoves.clear();
	m_legalMoves = getPseudoLegalMoves();

	if (pseudoLegal)
		return;


	//Filter out the moves that would leave king in check
	for (auto it = m_legalMoves.begin(); it != m_legalMoves.end(); )
	{
		Position & move = *it;
		//Copy game to check future move:
		Game gameCopy(m_parentGame);

		//Find our piece in copied board and target square
		PieceID thisCopy = m_id;
		Position originPos = m_pos;
		Position targetPos = move;

		bool stillChecked = false;

		if (m_id.valid() and originPos.valid() and targetPos.valid())
		{
			//Make a simulated move, so when refreshLegalMoves is called it skips check for checkmate, which would cause an infinite loop.
			gameCopy.movePiece(m_pos, move, true);

			if (gameCopy.getChecked(m_side))
				stillChecked = true;
		}

		if (stillChecked)
		{
			it = m_legalMoves.erase(it);
		}
		else
			it++;
	}
}

bool Piece::isAttacked()
{
	//TODO Don't check for moves of friendly pieces

	for (const Piece *Piece : m_parentGame.get().getPieces())
	{
		//Dont check for danger from itself
		if (Piece == this)
			continue;

		//Filter out friendly pieces
		if (m_side == Piece->m_side)
			continue;

		//Get list of valid moves
		//Dont check for moves that cant take a piece anyway (castling).
		const std::vector<Position> & moves = Piece->getLegalMoves();

		//Check if square of checked Piece is in valid moves of any Piece.
		auto it = std::find(moves.begin(), moves.end(), m_pos);

		if (it != moves.end())
		{
			return true;
		}
	}

	return false;
}

bool Chess::Piece::canPromote() const
{
	if (!m_pos.valid())
		return false;

	if (m_side == Side::White)
		return m_pos.row == 7;
	else
	if (m_side == Side::Black)
		return m_pos.row == 0;
	else 
		return false;
}
