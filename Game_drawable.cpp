#include "stdafx.h"
#include "Game_drawable.h"
#include "Timer.h"

using namespace Chess;

Game_drawable::Game_drawable()
{
	//Load piece texture
	if (!pieceTexture.loadFromFile("chess_sprites.png"))
		throw "Error loading piece texture";

	pieceTexture.setSmooth(true);
	//Popualte the board with squares
	for (int row = 0; row < 8; row++)
		for (int column = 0; column < 8; column++)
		{
			Square_draw *square = new Square_draw{ row, column, bounds };
			squares.set(row, column) = square;
		}
}

//Deep copy constructor from non-drawable (or drawable) game. pieces with unassigned squares will not be copied
Game_drawable::Game_drawable(const Game & game)
{
	//std::cout << "Calling copy constructor of drawable Game class\n";
	//Deep copy squares
	for (int row = 0; row < 8; row++)
		for (int column = 0; column < 8; column++)
		{
			const Square *oldSquare = game.getSquares().at(row, column);
			//Create new drawable square from copy
			Square_draw *newSquare = new Square_draw(*oldSquare, bounds);
			//Add square to table (overwrite really)
			squares.set(row, column) = newSquare;
			
			//If there was a piece assigned to that square, deep copy it too:
			const Piece *oldPiece = oldSquare->getPiece();
			if (oldPiece)
			{
				Piece *newPiece = new Piece_draw(*oldPiece, pieceTexture);
				//Assign square to piece
				newSquare->setPiece(newPiece);
				//Assign piece to square
				newPiece->setTakenSquare(newSquare);
				//Add piece to table
				pieces.push_back(newPiece);
			}
		}
}

Piece_draw * Game_drawable::addPiece(Piece::Type type, Side side, Square * square)
{
	Piece_draw *newPiece = new Piece_draw(type, side, pieceTexture);

	if (newPiece)
	{
		pieces.push_back(newPiece);	//C++17
		placePiece(newPiece, square);
	}

	newPiece->setMoved(false);

	return newPiece;
}

//Add piece at a row and column, row and column indices start with 0
Piece_draw * Game_drawable::addPiece(Piece::Type type, Side side, int row, int column)
{
	if (row > 7 or column > 7)
		return nullptr;

	return addPiece(type, side, squares.at(row, column));
}

void Game_drawable::populateBoard()
{
	//Add white pieces
	addPiece(Piece_draw::Type::Rook,		Side::White, 0, 0);
	addPiece(Piece_draw::Type::Knight,	Side::White, 0, 1);
	addPiece(Piece_draw::Type::Bishop,	Side::White, 0, 2);
	addPiece(Piece_draw::Type::Queen,	Side::White, 0, 3);
	addPiece(Piece_draw::Type::King,		Side::White, 0, 4);
	addPiece(Piece_draw::Type::Bishop,	Side::White, 0, 5);
	addPiece(Piece_draw::Type::Knight,	Side::White, 0, 6);
	addPiece(Piece_draw::Type::Rook,		Side::White, 0, 7);
	addPiece(Piece_draw::Type::Pawn,		Side::White, 1, 0);
	addPiece(Piece_draw::Type::Pawn,		Side::White, 1, 1);
	addPiece(Piece_draw::Type::Pawn,		Side::White, 1, 2);
	addPiece(Piece_draw::Type::Pawn,		Side::White, 1, 3);
	addPiece(Piece_draw::Type::Pawn,		Side::White, 1, 4);
	addPiece(Piece_draw::Type::Pawn,		Side::White, 1, 5);
	addPiece(Piece_draw::Type::Pawn,		Side::White, 1, 6);
	addPiece(Piece_draw::Type::Pawn,		Side::White, 1, 7);

	//Add black pieces
	addPiece(Piece_draw::Type::Rook,		Side::Black, 7, 7);
	addPiece(Piece_draw::Type::Knight,	Side::Black, 7, 6);
	addPiece(Piece_draw::Type::Bishop,	Side::Black, 7, 5);
	addPiece(Piece_draw::Type::Queen,	Side::Black, 7, 3);
	addPiece(Piece_draw::Type::King,		Side::Black, 7, 4);
	addPiece(Piece_draw::Type::Bishop,	Side::Black, 7, 2);
	addPiece(Piece_draw::Type::Knight,	Side::Black, 7, 1);
	addPiece(Piece_draw::Type::Rook,		Side::Black, 7, 0);
	addPiece(Piece_draw::Type::Pawn,		Side::Black, 6, 7);
	addPiece(Piece_draw::Type::Pawn,		Side::Black, 6, 6);
	addPiece(Piece_draw::Type::Pawn,		Side::Black, 6, 5);
	addPiece(Piece_draw::Type::Pawn,		Side::Black, 6, 4);
	addPiece(Piece_draw::Type::Pawn,		Side::Black, 6, 3);
	addPiece(Piece_draw::Type::Pawn,		Side::Black, 6, 2);
	addPiece(Piece_draw::Type::Pawn,		Side::Black, 6, 1);
	addPiece(Piece_draw::Type::Pawn,		Side::Black, 6, 0);

}

////Returns piece picked up
//Piece_draw* Chess::Game_drawable::pickUpPiece(Square_draw & from)
//{
//	Piece_draw* piece = from.getPiece();
//	if (piece != nullptr)
//	{
//		//Unassign square from piece
//		from.getPiece()->setTakenSquare(nullptr);
//		//Unassign piece from square
//		from.setPiece(nullptr);
//
//	}
//	//Return piece pointer
//	return piece;
//}


//void Game_drawable::movePiece(Square_draw & oldSquare, Square_draw & newSquare)
//{
//	takePiece(newSquare);
//	placePiece(pickUpPiece(oldSquare), &newSquare);
//}

void Game_drawable::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const auto row : squares)
	{
		for (const auto square : row)
		{
			const Square_draw *drawableSquare = dynamic_cast<const Square_draw*>(square);

			if (drawableSquare)
				drawableSquare->draw(target, states);
		}
	}

	for (Piece* piece : pieces)
	{
		Piece_draw *drawablePiece = dynamic_cast<Piece_draw*>(piece);

		if (drawablePiece)
			drawablePiece->draw(target, states);
	}
}

void Game_drawable::unhighlightAll()
{
	for (auto & row : squares)
	{
		for (auto & square : row)
		{
			Square_draw *drawableSquare = dynamic_cast<Square_draw*>(square);

			if (drawableSquare)
				drawableSquare->highlight(sf::Color::Transparent);
		}
	}
}

Square_draw * Game_drawable::selectSquare(const sf::Vector2f & worldCoords)
{
	for (auto & row : squares)
	{
		for (auto & square : row)
		{
			Square_draw *drawableSquare = dynamic_cast<Square_draw*>(square);

			if (drawableSquare)
			{
				if (drawableSquare->checkInBounds(worldCoords))
				{
					return drawableSquare;
				}
			}
		}
	}
	return nullptr;
}

Piece * Game_drawable::selectPiece(const sf::Vector2f & worldCoords)
{
	unhighlightAll();

	for (auto & row : squares)
	{
		for (auto square : row)
		{
			Square_draw *drawableSquare = dynamic_cast<Square_draw*>(square);
			if (drawableSquare)
			{
				if (drawableSquare->checkInBounds(worldCoords))
				{
					return drawableSquare->getPiece();
				}
			}
		}
	}
	return nullptr;
}

void Game_drawable::onMouseClick(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window)
{

	std::string print;

	//if (whiteSideActive)
	//	print += "White ";
	//else
	//	print += "Black ";

	switch (gameState)
	{
	case Game_drawable::GameState::SelectingPiece:
		//print += "Selecting piece\n";
		break;
	case Game_drawable::GameState::MovingPiece:
		//print += "Moving piece\n";
		break;
	default:
		break;
	}

	std::cout << print;

	playGame(event, window);

	//std::cout << selectSquare(window.mapPixelToCoords(sf::Vector2i(event.x, event.y)))->identify() << "\n";
}

void Game_drawable::highlight(const std::vector<Square*> & list, sf::Color color)
{
	for (Square* validMove : list)
	{
		Square_draw* drawableSquare = static_cast<Square_draw*>(validMove);
		drawableSquare->highlight(color);
	}
}

void Game_drawable::playGame(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window)
{
	static Piece *selectedPiece = nullptr;
	static Square_draw *selectedSquare = nullptr;

	if (event.button == sf::Mouse::Button::Right)
	{
		selectedPiece = nullptr;
		selectedSquare = nullptr;
		unhighlightAll();
		gameState = GameState::SelectingPiece;
		return;
	}

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
			//std::cout << "Failed with selecting a piece.\n";
			break;
		}
		//Check if selected piece belongs to player
		if (selectedPiece->getSide() == activeSide)
		{
			//std::cout << "Succeeded with selecting a piece.\n";
			//Highlight square with selected piece
			//TODO: This could be actually another square, implement function finding piece on correct square
			selectedSquare = selectSquare(worldCoords);
			selectedSquare->highlight(sf::Color::Green);
			//Check if the piece can move.
			auto validMoves = selectedPiece->getLegalMoves(this);
			highlight(validMoves, sf::Color::Green);
			//Allow reselection of piece if there are no valid moves.
			if (validMoves.size() > 0)
				gameState = GameState::MovingPiece;
		}
		else
		{
			//std::cout << "Selected piece of opponent.\n";
		}
		break;
	}
	case GameState::MovingPiece:
	{
		Timer timer;
		sf::Vector2f worldCoords = window.mapPixelToCoords(sf::Vector2i(event.x, event.y));
		Square_draw *newSelectedSquare = selectSquare(worldCoords);
		auto validMoves = selectedPiece->getLegalMoves(this);
		//Check if selectedPiece is not a nullptr, and if selected move square is valid
		if (newSelectedSquare and std::find(validMoves.begin(),validMoves.end(), newSelectedSquare) != validMoves.end())
		{
			Piece *takenPiece = newSelectedSquare->getPiece();
			//Move the piece, and take any piece on new square
			movePiece(*selectedSquare, *newSelectedSquare);
			//Reset command state
			selectedPiece = nullptr;
			selectedSquare = nullptr;
			//Check for mates
			checkForMates();

			if (whiteChecked)
				std::cout << "White is mated!\n";
			if (blackChecked)
				std::cout << "Black is mated!\n";
			//Unhighlight everything
			unhighlightAll();

			//Switch active side
			if (activeSide == Side::White)
				activeSide = Side::Black;
			else
				activeSide = Side::White;

			gameState = GameState::SelectingPiece;

			//Highlight attacked squares
			auto attackedSquares = getAttackedSquares(activeSide);
			highlight(attackedSquares, sf::Color::Red);

			if (attackedSquares.size() == 0)
				std::cout << "Checkmate!\n";
		}
	}
	}


}
//
//void Game_drawable::placePiece(Piece_draw * piece, Square_draw * square)
//{
//	//TODO Add exception handling
//	if (!piece or !square)
//		return;
//
//	//Assign piece to square
//	square->setPiece(piece);
//	//Assign square to piece
//	piece->setTakenSquare(square);
//	//Move the sprite
//	//piece.m_sprite.setPosition(square.m_shape.getPosition());  //Moved to setTakenSquare fnc
//}
//

