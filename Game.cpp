#include "stdafx.h"
#include "Game.h"


using namespace Chess;

//Typedefing a board array
//Assuming that the boardArray is array of rows.
// f.e. squares[row][column]
//TODO: Non hardcoded board size
using boardRow = std::array<Square, 8>;
using boardArray_t = std::array<boardRow, 8>;

Game::Game()
{
	//Popualte the board with squares
	for (int row = 0; row < 8; row++)
		for (int column = 0; column < 8; column++)
		{
			Square *square = new Square{ row, column };

			squares.set(row,column) = square;
		}
}

//Deep copy constructor, pieces with unassigned squares will not be copied

Chess::Game::Game(const Game & other)
{
	//std::cout << "Calling copy constructor of base Game class\n";
	//Deep copy squares
	for (int row = 0; row < 8; row++)
		for (int column = 0; column < 8; column++)
		{
			const Square *oldSquare = other.getSquares().at(row,column);
			//Create new drawable square from copy
			Square *newSquare = new Square(*oldSquare);
			//Add square to table (overwrite really)
			squares.set(row, column) = newSquare;

			//If there was a piece assigned to that square, deep copy it too:
			const Piece *oldPiece = oldSquare->getPiece();
			if (oldPiece)
			{
				Piece *newPiece = new Piece(*oldPiece);
				//Assign square to piece
				newSquare->setPiece(newPiece);
				//Assign piece to square
				newPiece->setTakenSquare(newSquare);
				//Add piece to table
				pieces.push_back(newPiece);
			}
		}
}

Board& Game::getSquares()
{
	return squares;
}

const Board& Game::getSquares() const
{
	return squares;
}

Piece * Game::addPiece(Piece::Type type, Side side, Square * square)
{
	Piece *newPiece = new Piece(type, side);

	if (newPiece)
	{
		pieces.push_back(newPiece);	//C++17
		placePiece(newPiece, square);
	}

	newPiece->setMoved(false);

	return newPiece;

}

//Add piece at a row and column, row and column indices start with 0
Piece * Game::addPiece(Piece::Type type, Side side, int row, int column)
{
	//Todo: Assert row and column fit the squares array
	return addPiece(type, side, squares.at(row, column));
}

void Game::populateBoard()
{
	//Add white pieces
	addPiece(Piece::Type::Rook, Side::White, 0, 0);
	addPiece(Piece::Type::Knight, Side::White, 0, 1);
	addPiece(Piece::Type::Bishop, Side::White, 0, 2);
	addPiece(Piece::Type::Queen, Side::White, 0, 3);
	addPiece(Piece::Type::King, Side::White, 0, 4);
	addPiece(Piece::Type::Bishop, Side::White, 0, 5);
	addPiece(Piece::Type::Knight, Side::White, 0, 6);
	addPiece(Piece::Type::Rook, Side::White, 0, 7);
	addPiece(Piece::Type::Pawn, Side::White, 1, 0);
	addPiece(Piece::Type::Pawn, Side::White, 1, 1);
	addPiece(Piece::Type::Pawn, Side::White, 1, 2);
	addPiece(Piece::Type::Pawn, Side::White, 1, 3);
	addPiece(Piece::Type::Pawn, Side::White, 1, 4);
	addPiece(Piece::Type::Pawn, Side::White, 1, 5);
	addPiece(Piece::Type::Pawn, Side::White, 1, 6);
	addPiece(Piece::Type::Pawn, Side::White, 1, 7);

	//Add black pieces
	addPiece(Piece::Type::Rook, Side::Black, 7, 7);
	addPiece(Piece::Type::Knight, Side::Black, 7, 6);
	addPiece(Piece::Type::Bishop, Side::Black, 7, 5);
	addPiece(Piece::Type::Queen, Side::Black, 7, 3);
	addPiece(Piece::Type::King, Side::Black, 7, 4);
	addPiece(Piece::Type::Bishop, Side::Black, 7, 2);
	addPiece(Piece::Type::Knight, Side::Black, 7, 1);
	addPiece(Piece::Type::Rook, Side::Black, 7, 0);
	addPiece(Piece::Type::Pawn, Side::Black, 6, 7);
	addPiece(Piece::Type::Pawn, Side::Black, 6, 6);
	addPiece(Piece::Type::Pawn, Side::Black, 6, 5);
	addPiece(Piece::Type::Pawn, Side::Black, 6, 4);
	addPiece(Piece::Type::Pawn, Side::Black, 6, 3);
	addPiece(Piece::Type::Pawn, Side::Black, 6, 2);
	addPiece(Piece::Type::Pawn, Side::Black, 6, 1);
	addPiece(Piece::Type::Pawn, Side::Black, 6, 0);

}

void Game::placePiece(Piece * piece, Square * square)
{
	//TODO Add exception handling
	if (!piece or !square)
		return;

	//Assign piece to square
	square->setPiece(piece);
	//Assign square to piece
	piece->setTakenSquare(square);
	//Move the sprite
	//piece.m_sprite.setPosition(square.m_shape.getPosition());  //Moved to setTakenSquare fnc
}


//Returns piece picked up
Piece* Chess::Game::pickUpPiece(Square & from)
{
	Piece* piece = from.getPiece();
	if (piece != nullptr)
	{
		//Unassign square from piece
		from.getPiece()->setTakenSquare(nullptr);
		//Unassign piece from square
		from.setPiece(nullptr);

	}
	//Return piece pointer
	return piece;
}

//Returns square picked up from
Square* Chess::Game::pickUpPiece(Piece & piece)
{
	Square* square = piece.getTakenSquare();
	//Unassign piece from square
	if (square)
	{
		piece.getTakenSquare()->setPiece(nullptr);
	}
	//Unassign square from piece
	piece.setTakenSquare(nullptr);
	//Return square ptr
	return square;
}

void Game::movePiece(Square & oldSquare, Square & newSquare)
{
	takePiece(newSquare);
	placePiece(pickUpPiece(oldSquare), &newSquare);
}

void Game::checkForMates()
{
	std::vector<Piece*> kings;

	for (Piece *piece : pieces)
	{
		if (piece->getType() == Piece::Type::King)
			kings.push_back(piece);
	}

	for (Piece *king : kings)
	{
		if (king->getSide() == Side::Black)
		{
			if (king->isAtacked(this))
				blackChecked = true;
			else
				blackChecked = false;
		}
		else
		{
			if (king->isAtacked(this))
				whiteChecked = true;
			else
				whiteChecked = false;
		}
	}
}

std::vector<Square*> Chess::Game::getAttackedSquares(Side bySide)
{
	std::vector<Square*> attacked;

	for (const Piece * piece : pieces)
	{
		if (piece->getSide() != bySide)
			continue;

		auto validMoves = piece->getLegalMoves(this);

		attacked.insert(attacked.end(), validMoves.begin(), validMoves.end());
	}
	return attacked;
}

