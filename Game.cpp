#include "stdafx.h"
#include "Game.h"
#include "Setups.h"

using namespace Chess;

Game::Game() : pieces{ *this }
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

Chess::Game::Game(const Game & other) : pieces { *this }
{
	//static int times = 0;
	//std::cout << "Calling copy constructor of base Game class " << ++times <<" \n";
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
		Piece *newPiece = new Piece{ *oldPiece, *this };
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
		piece->setTakenSquare(pos);
	}
}

Piece * Game::addPiece(Piece::Type type, Side side, Position square)
{
	Piece *newPiece = new Piece(type, side, piece_count, *this);

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
	//Chess::Setups::standardGame(*this);
	Chess::Setups::standardGame(*this);

	refreshAllLegalMoves();
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
	Position square = piece->getPos();
	//Unassign piece from square
	if (piece->getPos().valid())
	{
		squares.at(piece->getPos()).setPieceID(PieceID{});
		//Unassign square from piece
		piece->setTakenSquare(Position{});
	}
	//Return square pos
	return square;
}

//bool simulated - true when move is a simulation for illegal move check
void Game::movePiece(Position oldSquare, Position newSquare, bool simulated)
{
	takePiece(newSquare);

	auto & pieceID = squares.at(oldSquare).getPieceID();

	if (pieceID.valid())
	{
		Piece & piece = *pieces.at(pieceID);
		//Check is this move is castling
		//If it is, move the rook too.
		if (piece.getType() == Piece::Type::King and
			!piece.getMoved())
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
		if (piece.getType() == Piece::Type::Pawn and
			!piece.getMoved())
		{
			//White side
			if (newSquare.row == 3 and oldSquare.row == 1)
			{
				squares.at(2, oldSquare.column).setEnPassantPieceID(pieceID);
			}
			else //Black side
			if (newSquare.row == 4 and oldSquare.row == 6)
			{
				squares.at(5, oldSquare.column).setEnPassantPieceID(pieceID);
			}
		}
			placePiece(pickUpPiece(oldSquare), newSquare);

			//Check if pieceID can be promoted:
			if (piece.canPromote())
				piece.promote(Piece::Type::Queen);

			refreshAllLegalMoves(simulated);
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
			if (king->isAttacked())
				blackChecked = true;
			else
				blackChecked = false;
		}
		else
		{
			if (king->isAttacked())
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

		auto validMoves = piece->getLegalMoves();

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

void Chess::Game::refreshAllLegalMoves(bool pseudoLegal)
{
	for (auto * piece : pieces)
	{
		piece->refreshLegalMoves(pseudoLegal);
	}
}



