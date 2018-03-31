#include "stdafx.h"
#include "Game.h"


using namespace Chess;

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
	{
		for (int column = 0; column < 8; column++)
		{
			const Square & oldSquare = other.getSquares().at(row, column);
			//Create new drawable square from copy
			Square *newSquare = new Square(oldSquare);
			//Add square ptr to table (overwrite really)
			squares.set(row, column) = newSquare;
		}
	}
	//Deep copy pieces
	for (const Piece * oldPiece : other.getPieces())
	{
		//Create copy of old piece
		Piece *newPiece = new Piece{ *oldPiece };
		pieces.push_back(newPiece);
		++piece_count;
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

void Game::placePiece(PieceID pieceID, Position pos)
{
	if (pieceID.valid() and pos.valid())
	{
		Piece * piece = pieces.at(pieceID);
		//Assign piece to square
		squares.at(pos).setPieceID(piece->getID());
		//Assign square to piece
		piece->setTakenSquare(pos, &squares);
		//Move the sprite
		//piece.m_sprite.setPosition(square.m_shape.getPosition());  //Moved to setTakenSquare fnc
	}
}

Piece * Game::addPiece(Piece::Type type, Side side, Position square)
{
	Piece *newPiece = new Piece(type, side, piece_count);

	if (newPiece)
	{
		pieces.push_back(newPiece);	//C++17
		placePiece(piece_count, square);
		++piece_count;

		newPiece->setMoved(false);
	}
	return newPiece;

}

//Add piece at a row and column, row and column indices start with 0
Piece * Game::addPiece(Piece::Type type, Side side, int row, int column)
{
	//Todo: Assert row and column fit the squares array
	return addPiece(type, side, Position(row, column));
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


//Returns piece picked up
PieceID Chess::Game::pickUpPiece(Position from)
{
	PieceID piece = squares.at(from).getPieceID();
	if (piece.valid())
	{
		//Unassign square from piece
		pieces.at(piece)->setTakenSquare(Position{});
		//Unassign piece from square
		squares.at(from).setPieceID(PieceID{});

	}
	//Return piece pointer
	return piece;
}

//Returns square picked up from
Position Chess::Game::pickUpPiece(PieceID pieceID)
{
	if (!pieceID.valid())
		return Position{};

	Piece * piece = pieces.at(pieceID);
	Position square = piece->getTakenSquare();
	//Unassign piece from square
	if (piece->getTakenSquare().valid())
	{
		squares.at(piece->getTakenSquare()).setPieceID(PieceID{});
		//Unassign square from piece
		piece->setTakenSquare(Position{});
	}
	//Return square pos
	return square;
}

void Game::movePiece(Position oldSquare, Position newSquare)
{
	takePiece(newSquare);

	auto & piece = squares.at(oldSquare).getPieceID();

	if (piece.valid())
	{
		//Check is this move is castling
		//If it is, move the rook too.
		if (pieces.at(piece)->getType() == Piece::Type::King and
			!pieces.at(piece)->getMoved())
		{
			//Kingside or queenside?
			if ((newSquare.column - oldSquare.column) == -2)		//Queenside
			{
				movePiece(Position(oldSquare.row, 0), Position(oldSquare.row, 3));
			}
			else if ((newSquare.column - oldSquare.column) == 2)	//Kingside
			{
				movePiece(Position(oldSquare.row, 7), Position(oldSquare.row, 5));
			}
		}

		//Check if this move is pawn double push.
		if (pieces.at(piece)->getType() == Piece::Type::Pawn and
			!pieces.at(piece)->getMoved())
		{
			//White side
			if (newSquare.row == 3 and oldSquare.row == 1)
			{
				squares.at(2, oldSquare.column).setEnPassantPieceID(piece);
			}
			else //Black side
			if (newSquare.row == 4 and oldSquare.row == 6)
			{
				squares.at(5, oldSquare.column).setEnPassantPieceID(piece);
			}
		}
			placePiece(pickUpPiece(oldSquare), newSquare);
	}
}


void Chess::Game::takePiece(Position pos)
{
	auto & square = squares.at(pos);
	//If the square had a piece on it, take it
	if (squares.at(pos).getPieceID().valid())
	{
		Piece * piece = pieces.at(squares.at(pos).getPieceID());
		piece->setTakenSquare(Position{});
		piece->setDead(true);
		square.setPieceID(PieceID{});
	} else
	if (squares.at(pos).getEnPassantPieceID().valid())
	{
		Piece * piece = pieces.at(squares.at(pos).getEnPassantPieceID());
		piece->setTakenSquare(Position{});
		piece->setDead(true);
		square.setEnPassantPieceID(PieceID{});
	}
}

void Game::checkForMates()
{
	PieceSet kings;

	for (Piece *piece : pieces)
	{
		if (piece->getType() == Piece::Type::King)
			kings.push_back(piece);
	}

	for (Piece *king : kings)
	{
		if (king->getSide() == Side::Black)
		{
			if (king->isAttacked(*this))
				blackChecked = true;
			else
				blackChecked = false;
		}
		else
		{
			if (king->isAttacked(*this))
				whiteChecked = true;
			else
				whiteChecked = false;
		}
	}
}

std::vector<Position> Chess::Game::getAttackedSquares(Side bySide)
{
	std::vector<Position> attacked;

	for (const Piece * piece : pieces)
	{
		if (piece->getSide() != bySide)
			continue;

		auto validMoves = piece->getLegalMoves(*this);

		attacked.insert(attacked.end(), validMoves.begin(), validMoves.end());
	}
	return attacked;
}

void Chess::Game::switchActiveSide()
{
	++activeSide;
	//Clear enPassant status of new side - en passant capture can only happen immediately
	if (activeSide == Side::White)
	{
		for (int column = 0; column < 8; column++)
		{
			squares.at(2, column).setEnPassantPieceID(-1);
		}
	} else
	if (activeSide == Side::Black)
	{
		for (int column = 0; column < 8; column++)
		{
			squares.at(5, column).setEnPassantPieceID(-1);
		}
	}
}

