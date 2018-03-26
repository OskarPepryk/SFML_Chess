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

			squares[row][column] = square;
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

void Game::addPiece(Piece::Type type, Piece::Side side, Square * square)
{
	Piece *newPiece = new Piece(type, side);

	if (!newPiece)
	{
		//add exception handling
		return;
	}

	pieces.push_back(newPiece);	//C++17

	placePiece(newPiece, square);
}

//Add piece at a row and column, row and column indices start with 0
void Game::addPiece(Piece::Type type, Piece::Side side, int row, int column)
{
	//Todo: Assert row and column fit the squares array
	addPiece(type, side, squares[row][column]);
}

void Game::populateBoard()
{
	//Add white pieces
	addPiece(Piece::Type::Rook, Piece::Side::White, 0, 0);
	addPiece(Piece::Type::Knight, Piece::Side::White, 0, 1);
	addPiece(Piece::Type::Bishop, Piece::Side::White, 0, 2);
	addPiece(Piece::Type::Queen, Piece::Side::White, 0, 3);
	addPiece(Piece::Type::King, Piece::Side::White, 0, 4);
	addPiece(Piece::Type::Bishop, Piece::Side::White, 0, 5);
	addPiece(Piece::Type::Knight, Piece::Side::White, 0, 6);
	addPiece(Piece::Type::Rook, Piece::Side::White, 0, 7);
	addPiece(Piece::Type::Pawn, Piece::Side::White, 1, 0);
	addPiece(Piece::Type::Pawn, Piece::Side::White, 1, 1);
	addPiece(Piece::Type::Pawn, Piece::Side::White, 1, 2);
	addPiece(Piece::Type::Pawn, Piece::Side::White, 1, 3);
	addPiece(Piece::Type::Pawn, Piece::Side::White, 1, 4);
	addPiece(Piece::Type::Pawn, Piece::Side::White, 1, 5);
	addPiece(Piece::Type::Pawn, Piece::Side::White, 1, 6);
	addPiece(Piece::Type::Pawn, Piece::Side::White, 1, 7);

	//Add black pieces
	addPiece(Piece::Type::Rook, Piece::Side::Black, 7, 7);
	addPiece(Piece::Type::Knight, Piece::Side::Black, 7, 6);
	addPiece(Piece::Type::Bishop, Piece::Side::Black, 7, 5);
	addPiece(Piece::Type::Queen, Piece::Side::Black, 7, 3);
	addPiece(Piece::Type::King, Piece::Side::Black, 7, 4);
	addPiece(Piece::Type::Bishop, Piece::Side::Black, 7, 2);
	addPiece(Piece::Type::Knight, Piece::Side::Black, 7, 1);
	addPiece(Piece::Type::Rook, Piece::Side::Black, 7, 0);
	addPiece(Piece::Type::Pawn, Piece::Side::Black, 6, 7);
	addPiece(Piece::Type::Pawn, Piece::Side::Black, 6, 6);
	addPiece(Piece::Type::Pawn, Piece::Side::Black, 6, 5);
	addPiece(Piece::Type::Pawn, Piece::Side::Black, 6, 4);
	addPiece(Piece::Type::Pawn, Piece::Side::Black, 6, 3);
	addPiece(Piece::Type::Pawn, Piece::Side::Black, 6, 2);
	addPiece(Piece::Type::Pawn, Piece::Side::Black, 6, 1);
	addPiece(Piece::Type::Pawn, Piece::Side::Black, 6, 0);

}

void Game::placePiece(Piece * piece, Square * square)
{
	//TODO Add exception handling
	if (!piece or !square)
		return;

	//Assign piece to square
	square->m_piece = piece;
	//Assign square to piece
	piece->setTakenSquare(square);
	//Move the sprite
	//piece.m_sprite.setPosition(square.m_shape.getPosition());  //Moved to setTakenSquare fnc
}


//Returns piece picked up
Piece* Chess::Game::pickUpPiece(Square & from)
{
	Piece* piece = from.m_piece;
	if (piece != nullptr)
	{
		//Unassign square from piece
		from.m_piece->setTakenSquare(nullptr);
		//Unassign piece from square
		from.m_piece = nullptr;

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
	//if (oldSquare.m_piece != nullptr)
	//{
	//	newSquare.m_piece = oldSquare.m_piece;
	//	oldSquare.m_piece = nullptr;

	//	newSquare.m_piece->m_sprite.setPosition(newSquare.m_shape.getPosition());
	//	newSquare.m_piece->m_hasMoved = true;
	//}

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
		if (king->getSide() == Piece::Side::Black)
		{
			if (king->checkAttacked(this))
				blackChecked = true;
			else
				blackChecked = false;
		}
		else
		{
			if (king->checkAttacked(this))
				whiteChecked = true;
			else
				whiteChecked = false;
		}
	}
}

