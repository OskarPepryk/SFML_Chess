#include "stdafx.h"
#include "Piece.h"
#include "Game.h"

using namespace Chess;

void Piece::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (m_isDead or !m_takenSquare)
		return;
	target.draw(m_sprite, states);
}

void Piece::setTexture(Type type, Side side, sf::Texture & texture)
{
	m_type = type;
	m_side = side;

	m_sprite.setTexture(texture);

	sf::IntRect textureRect;

	int textureColumn;
	int textureRow;

	switch (m_type)
	{
	case Type::King:
		textureColumn = 0;
		break;
	case Type::Queen:
		textureColumn = 1;
		break;
	case Type::Bishop:
		textureColumn = 2;
		break;
	case Type::Knight:
		textureColumn = 3;
		break;
	case Type::Rook:
		textureColumn = 4;
		break;
	case Type::Pawn:
		textureColumn = 5;
		break;
	default:
		textureColumn = 0;
		break;
	}

	switch (m_side)
	{
	case Side::White:
		textureRow = 0;
		break;
	case Side::Black:
		textureRow = 1;
		break;
	default:
		textureRow = 0;
		break;
	}


	m_sprite.setTextureRect(sf::IntRect((texture.getSize().x / 6) * textureColumn,
		(texture.getSize().y / 2)* textureRow, 213, 213));

}

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
	m_takenSquare = newSquare;
	if (newSquare)
		m_sprite.setPosition(newSquare->m_shape.getPosition());
	//else
	//TODO Implement hiding of unplaced piece
}

void Piece::getValidMovesInDirection(Game *board, std::list<Square*> &validSquares, Directions::DirectionSet dirSet, int maxRange, bool canJumpOver) const
{
	using boardRow = std::array<Square, 8>;
	using boardArray_t = std::array<boardRow, 8>;

	boardArray_t &squares = board->getSquares();

	int column = m_takenSquare->getColumn();
	int row = m_takenSquare->getRow();

	for (Directions::Direction dir : dirSet)
	{
		bool foundPiece = false;
		//Check in range of 1
		for (int range = 1; range <= maxRange and (!foundPiece or canJumpOver); range++)
		{
			//Unless square is not valid
			try
			{
				auto & square = squares.at(row + range * dir.up).at(column + range * dir.right);
				//If it has any piece on it
				if (square.m_piece)
				{
					//Don't look in this direction anymoreif a piece is there
					foundPiece = true;
					//If the piece is opponent
					if (square.m_piece->getSide() != m_side)
					{
						validSquares.push_back(&square);
					}

				}
				else
					validSquares.push_back(&square);
			}
			catch (std::out_of_range) {};
		}
	}
}

std::list<Square*> Piece::getValidMoves(Game * board) const
{
	if (!m_takenSquare)
		throw "Piece is not placed";

	std::list<Square*> validSquares;

	using boardRow = std::array<Square, 8>;
	using boardArray_t = std::array<boardRow, 8>;

	boardArray_t& squares = board->getSquares();

	int column = m_takenSquare->getColumn();
	int row = m_takenSquare->getRow();

	switch (m_type)
	{
	case Type::King:
	{
		getValidMovesInDirection(board, validSquares, Directions::allDirections, 1);
		break;
	}
	case Type::Bishop:
	{
		getValidMovesInDirection(board, validSquares, Directions::diagonals, 8);
		break;
	}
	case Type::Rook:
	{
		getValidMovesInDirection(board, validSquares, Directions::rightAngles, 8);
		break;
	}
	case Type::Queen:
	{
		getValidMovesInDirection(board, validSquares, Directions::allDirections, 8);
		break;
	}
	case Type::Knight:
	{
		getValidMovesInDirection(board, validSquares, Directions::knight, 1, true);
		break;
	}
	case Piece::Type::Pawn:
	default:
	{
		//TODO Implement en passant
		//White pawns and black pawns move in different directions:
		//TODO Unhardcode movement direction in case white starts on top
		int moveDirection;
		if (m_side == Piece::Side::White)
			moveDirection = 1;
		else
			moveDirection = -1;

		//Square in front
		try 
		{
			if ((squares.at(row + moveDirection).at(column)).m_piece == nullptr)
				validSquares.push_back(&(squares.at(row + moveDirection).at(column)));
		}
		catch (std::out_of_range) {}

		// or 2 square first move
		if (!m_hasMoved)
		{
			try
			{
				if ((squares.at(row + 2 * moveDirection).at(column)).m_piece == nullptr and
					(squares.at(row + moveDirection).at(column)).m_piece == nullptr)
					validSquares.push_back(&(squares.at(row + 2 * moveDirection).at(column)));
			}
			catch (std::out_of_range){}
		}


		//Diagonal left
		try
		{
			auto & diagonal1 = squares.at(row + moveDirection).at(column - 1);

			if (diagonal1.m_piece)
				if (diagonal1.m_piece->getSide() != m_side)
					validSquares.push_back(&diagonal1);

		}
		catch (std::out_of_range) {};

		//Diagonal right
		try
		{
			auto & diagonal1 = squares.at(row + moveDirection).at(column + 1);

			if (diagonal1.m_piece)
				if (diagonal1.m_piece->getSide() != m_side)
					validSquares.push_back(&diagonal1);

		}
		catch (std::out_of_range) {};

		break;
		}
	}
	return validSquares;
}

bool Piece::checkAttacked(Game * board)
{
	if (!board)
		throw "Board or piece was null in Chess::Piece::checkAttacked()\n";

	for (const Piece *piece : board->getPieces())
	{
		//Dont check for danger from itself
		if (piece == this)
			continue;

		//Get list of valid moves
		std::list<Square*> moves = piece->getValidMoves(board);
		//Check if square of checked piece is in valid moves of any piece.
		//Todo filter out friendly pieces
		auto it = std::find(moves.begin(), moves.end(), m_takenSquare);

		if (it != moves.end())
		{
			return true;
		}
	}

	return false;
}