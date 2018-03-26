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
	//Load piece texture
	if (!pieceTexture.loadFromFile("chess_sprites.png"))
		throw "Error loading piece texture";

	pieceTexture.setSmooth(true);
	//Popualte the board with squares
	for (int row = 0; row < 8; row++)
		for (int column = 0; column < 8; column++)
		{
			Square square{ row, column, bounds };
			squares[row][column] = square;
		}
}

boardArray_t& Game::getSquares()
{
	return squares;
}

const boardArray_t& Game::getSquares() const
{
	return squares;
}

void Game::addPiece(Piece::Type type, Piece::Side side, Square & square)
{
	Piece *newPiece = new Piece(type, side, pieceTexture);

	if (!newPiece)
	{
		//add exception handling
		return;
	}


	pieces.push_back(newPiece);	//C++17
	
	placePiece(newPiece, &square);

	//newPiece.m_hasMoved = false;
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
	addPiece(Piece::Type::Rook,		Piece::Side::White, 0, 0);
	addPiece(Piece::Type::Knight,	Piece::Side::White, 0, 1);
	addPiece(Piece::Type::Bishop,	Piece::Side::White, 0, 2);
	addPiece(Piece::Type::Queen,	Piece::Side::White, 0, 3);
	addPiece(Piece::Type::King,		Piece::Side::White, 0, 4);
	addPiece(Piece::Type::Bishop,	Piece::Side::White, 0, 5);
	addPiece(Piece::Type::Knight,	Piece::Side::White, 0, 6);
	addPiece(Piece::Type::Rook,		Piece::Side::White, 0, 7);
	addPiece(Piece::Type::Pawn,		Piece::Side::White, 1, 0);
	addPiece(Piece::Type::Pawn,		Piece::Side::White, 1, 1);
	addPiece(Piece::Type::Pawn,		Piece::Side::White, 1, 2);
	addPiece(Piece::Type::Pawn,		Piece::Side::White, 1, 3);
	addPiece(Piece::Type::Pawn,		Piece::Side::White, 1, 4);
	addPiece(Piece::Type::Pawn,		Piece::Side::White, 1, 5);
	addPiece(Piece::Type::Pawn,		Piece::Side::White, 1, 6);
	addPiece(Piece::Type::Pawn,		Piece::Side::White, 1, 7);

	//Add black pieces
	addPiece(Piece::Type::Rook,		Piece::Side::Black, 7, 7);
	addPiece(Piece::Type::Knight,	Piece::Side::Black, 7, 6);
	addPiece(Piece::Type::Bishop,	Piece::Side::Black, 7, 5);
	addPiece(Piece::Type::Queen,	Piece::Side::Black, 7, 3);
	addPiece(Piece::Type::King,		Piece::Side::Black, 7, 4);
	addPiece(Piece::Type::Bishop,	Piece::Side::Black, 7, 2);
	addPiece(Piece::Type::Knight,	Piece::Side::Black, 7, 1);
	addPiece(Piece::Type::Rook,		Piece::Side::Black, 7, 0);
	addPiece(Piece::Type::Pawn,		Piece::Side::Black, 6, 7);
	addPiece(Piece::Type::Pawn,		Piece::Side::Black, 6, 6);
	addPiece(Piece::Type::Pawn,		Piece::Side::Black, 6, 5);
	addPiece(Piece::Type::Pawn,		Piece::Side::Black, 6, 4);
	addPiece(Piece::Type::Pawn,		Piece::Side::Black, 6, 3);
	addPiece(Piece::Type::Pawn,		Piece::Side::Black, 6, 2);
	addPiece(Piece::Type::Pawn,		Piece::Side::Black, 6, 1);
	addPiece(Piece::Type::Pawn,		Piece::Side::Black, 6, 0);

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
		piece.getTakenSquare()->m_piece = nullptr;
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

void Game::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const auto &row : squares)
	{
		for (const auto &square : row)
		{
			square.draw(target, states);
		}
	}

	for (Piece* piece : pieces)
	{
		piece->draw(target, states);
	}
}

void Game::unhighlightAll()
{
	for (auto & row : squares)
	{
		for (auto & square : row)
		{
			square.highlight(false);
		}
	}
}

Square * Game::selectSquare(const sf::Vector2f & worldCoords)
{
	for (auto & row : squares)
	{
		for (auto & square : row)
		{
			if (square.checkInBounds(worldCoords))
			{
				return &square;
			}
		}
	}
	return nullptr;
}

Piece * Game::selectPiece(const sf::Vector2f & worldCoords)
{
	unhighlightAll();

	for (auto & row : squares)
	{
		for (auto & square : row)
		{
			if (square.checkInBounds(worldCoords))
			{
				return square.m_piece;
			}
		}
	}
	return nullptr;
}

void Game::onMouseClick(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window)
{

	std::string print;

	if (whiteSideActive)
		print += "White ";
	else
		print += "Black ";

	switch (gameState)
	{
	case Game::GameState::SelectingPiece:
		print += "Selecting piece\n";
		break;
	case Game::GameState::MovingPiece:
		print += "Moving piece\n";
		break;
	default:
		break;
	}

	std::cout << print;

	playGame(event, window);

	std::cout << selectSquare(window.mapPixelToCoords(sf::Vector2i(event.x, event.y)))->identify() << "\n";
}

void Game::playGame(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window)
{
	static Piece *selectedPiece = nullptr;
	static Square *selectedSquare = nullptr;

	switch (gameState)
	{
		//White player selects a piece to move.
	case GameState::SelectingPiece:
	{
		sf::Vector2f worldCoords = window.mapPixelToCoords(sf::Vector2i(event.x, event.y));
		selectedPiece = selectPiece(worldCoords);
		//If no selected piece, don't advance the game !!!!!!!
		if (!selectedPiece)
		{
			std::cout << "Failed with selecting a piece.\n";
			break;
		}
		//Check if selected piece belongs to player
		if (selectedPiece->getSide() == Piece::Side::White && whiteSideActive
			|| selectedPiece->getSide() == Piece::Side::Black && !whiteSideActive)
		{
			std::cout << "Succeeded with selecting a piece.\n";
			//Highlight square with selected piece
			//TODO: This could be actually another square, implement function finding piece on correct square
			selectedSquare = selectSquare(worldCoords);
			selectedSquare->highlight(true);
			//Check if the piece can move.
			auto validMoves = selectedPiece->getValidMoves(this);

			for (auto & validMove : validMoves)
			{
				validMove->highlight(true);
			}
			//Allow reselection of piece if there are no valid moves.
			if (validMoves.size() > 0)
				gameState = GameState::MovingPiece;
		}
		else
		{
			std::cout << "Selected piece of opponent.\n";
		}
		break;
	}
	case GameState::MovingPiece:
	{
		sf::Vector2f worldCoords = window.mapPixelToCoords(sf::Vector2i(event.x, event.y));
		Square *newSelectedSquare = selectSquare(worldCoords);
		auto validMoves = selectedPiece->getValidMoves(this);
		//Check if selectedPiece is not a nullptr, and if selected move square is valid
		if (newSelectedSquare and std::find(validMoves.begin(),validMoves.end(), newSelectedSquare) != validMoves.end())
		{
			//Take a piece
			Piece *takenPiece = newSelectedSquare->m_piece;
			if (takenPiece)
				takenPiece->setDead(true);
			selectedSquare->m_piece = selectedPiece;
			//Move the piece
			movePiece(*selectedSquare, *newSelectedSquare);
			//Reset command state
			selectedPiece = nullptr;
			selectedSquare = nullptr;
			gameState = GameState::SelectingPiece;
			//Switch active side
			whiteSideActive = !whiteSideActive;
			//Unhighlight everything
			unhighlightAll();
		}
	}
	}


}


