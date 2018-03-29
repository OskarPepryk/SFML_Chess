#include "stdafx.h"

#include <algorithm>

#include "Piece.h"
#include "Game.h"

using namespace Chess;

Square * Chess::Piece::getTakenSquare()
{
	return m_takenSquare;
}

const Square * Chess::Piece::getTakenSquare() const
{
	return m_takenSquare;
}

void Chess::Piece::setTakenSquare(Square * newSquare)
{
	m_hasMoved = true;
	m_takenSquare = newSquare;
}

void Piece::getPseudoLegalMovesInDirection(Game *board, std::vector<Square*> &validSquares, Directions::DirectionSet dirSet, int maxRange, bool canJumpOver) const
{
	Board &squares = board->getSquares();

	int column = m_takenSquare->getColumn();
	int row = m_takenSquare->getRow();

	for (Directions::Direction dir : dirSet)
	{
		bool foundPiece_nondrawable = false;
		//Check in range of 1
		for (int range = 1; range <= maxRange and (!foundPiece_nondrawable or canJumpOver); range++)
		{
			//Unless square is not valid
		
			const auto square = squares.at(row + range * dir.up,column + range * dir.right);
			if (square->isValid())
				//If it has any Piece on it
			{
				if (square->getPiece())
				{
					//Don't look in this direction anymoreif a Piece is there
					foundPiece_nondrawable = true;
					//If the Piece is opponent
					if (square->getPiece()->getSide() != m_side)
					{
						validSquares.push_back(square);
					}
				}
				else
					validSquares.push_back(square);
			}
		}
	}
}

std::vector<Square*> Piece::getPseudoLegalMoves(Game * board) const
{
	std::vector<Square*> validSquares;

	if (!m_takenSquare)
		return validSquares;

	using boardRow = std::array<Square, 8>;
	using boardArray_t = std::array<boardRow, 8>;

	auto & squares = board->getSquares();

	int column = m_takenSquare->getColumn();
	int row = m_takenSquare->getRow();

	switch (m_type)
	{
	case Type::King:
	{
		getPseudoLegalMovesInDirection(board, validSquares, Directions::allDirections, 1);
		break;
	}
	case Type::Bishop:
	{
		getPseudoLegalMovesInDirection(board, validSquares, Directions::diagonals, 8);
		break;
	}
	case Type::Rook:
	{
		getPseudoLegalMovesInDirection(board, validSquares, Directions::rightAngles, 8);
		break;
	}
	case Type::Queen:
	{
		getPseudoLegalMovesInDirection(board, validSquares, Directions::allDirections, 8);
		break;
	}
	case Type::Knight:
	{
		getPseudoLegalMovesInDirection(board, validSquares, Directions::knight, 1, true);
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
		if (squares.at(row + moveDirection, column)->isValid() and 
			squares.at(row + moveDirection, column)->getPiece() == nullptr)
			validSquares.push_back(squares.at(row + moveDirection, column));

		// or 2 square first move
		if (!m_hasMoved)
		{
			if (squares.at(row + 2 * moveDirection, column)->isValid() and
				squares.at(row + 1 * moveDirection, column)->isValid() and
				squares.at(row + 2 * moveDirection, column)->getPiece() == nullptr and
				squares.at(row + 1 * moveDirection, column)->getPiece() == nullptr)
				validSquares.push_back(squares.at(row + 2 * moveDirection,column));
		}


		//Diagonal left
		Square* diagonal1 = squares.at(row + moveDirection, column - 1);

		if (diagonal1->isValid() and
			diagonal1->getPiece())
			if (diagonal1->getPiece()->getSide() != m_side)
				validSquares.push_back(diagonal1);


		//Diagonal right
		Square* diagonal2 = squares.at(row + moveDirection, column + 1);

		if (diagonal2->isValid() and
			diagonal2->getPiece())
			if (diagonal2->getPiece()->getSide() != m_side)
				validSquares.push_back(diagonal2);


		break;
	}
	}
	return validSquares;
}

std::vector<Square*> Chess::Piece::getLegalMoves(Game * board) const
{
	std::vector<Square*> PseudoLegalMoves = getPseudoLegalMoves(board);

	//Find king

	//const Piece * king = nullptr;///King must exist

	//for (const Piece * piece : board->getPieces())
	//{
	//	if (piece->getType() == Type::King and piece->getSide() == getSide())
	//		king = piece;
	//}

	//if (!king)
	//	return PseudoLegalMoves;

	//Filter out the moves that would leave king in check

	for (auto it = PseudoLegalMoves.begin(); it!=PseudoLegalMoves.end(); )
	{
		Square * move = *it;
		//Copy game to check future move:
		Game gameCopy(*board);

		//Find our piece in copied board and target square
		Piece* thisCopy = nullptr;
		Square* squareCopy = nullptr;
		Square* targetCopy = nullptr;

		bool stillChecked = false;

		for (auto & row : gameCopy.getSquares())
		{
			for (auto square : row)
			{
				if (square->getRow() == m_takenSquare->getRow() and
					square->getColumn() == m_takenSquare->getColumn())
				{
					thisCopy = square->getPiece();
					squareCopy = square;
				}
				else if (square->getRow() == move->getRow() and
					square->getColumn() == move->getColumn())
				{
					targetCopy = square;
				}
				//else
					//std::cout << "Didn't found a piece in copied game.";
			}
		}

		if (thisCopy and squareCopy and targetCopy)
		{
			gameCopy.movePiece(*squareCopy, *targetCopy);

			if (gameCopy.getChecked(thisCopy->getSide()))
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

bool Piece::isAtacked(Game * board)
{
	if (!board)
		throw "Board<Square*> or Piece was null in Chess::Piece::isAtacked()\n";

	for (const Piece *Piece : board->getPieces())
	{
		//Dont check for danger from itself
		if (Piece == this)
			continue;

		//Get list of valid moves
		std::vector<Square*> moves = Piece->getPseudoLegalMoves(board);
		//Check if square of checked Piece is in valid moves of any Piece.
		//Todo filter out friendly Piece_nondrawables
		auto it = std::find(moves.begin(), moves.end(), m_takenSquare);

		if (it != moves.end())
		{
			return true;
		}
	}

	return false;
}