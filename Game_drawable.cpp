#include "stdafx.h"
#include "Game_drawable.h"
#include "Timer.h"

using namespace Chess;

Game_drawable::Game_drawable(bool upsideDown) : upsideDown{ upsideDown }
{
	//Drawable game should return messages
	quiet = false;
	//Load piece texture
	if (!pieceTexture.loadFromFile("chess_sprites.png"))
		throw "Error loading piece texture";

	pieceTexture.setSmooth(true);
	//Popualte the board with squares
	for (int row = 0; row < 8; row++)
		for (int column = 0; column < 8; column++)
		{
			Square_draw *square = new Square_draw{ row, column, bounds, upsideDown };
			squares.set(row, column) = square;
		}
}

//Deep copy constructor from non-drawable (or drawable) game. pieces with unassigned squares will not be copied
Game_drawable::Game_drawable(const Game & other, bool upsideDown) : upsideDown{ upsideDown }
{
	//Drawable game should return messages
	quiet = false;
	//Load piece texture
	if (!pieceTexture.loadFromFile("chess_sprites.png"))
		throw "Error loading piece texture";
	pieceTexture.setSmooth(true);
	//std::cout << "Calling copy constructor of base Game class\n";
	//Deep copy squares
	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			const Square & oldSquare = other.getSquares().at(row, column);
			//Create new drawable square from copy
			Square_draw *newSquare = new Square_draw(oldSquare, bounds, upsideDown);
			//Add square ptr to table (overwrite really)
			squares.set(row, column) = newSquare;
		}
	}
	//Deep copy pieces
	for (const Piece * oldPiece : other.getPieces())
	{
		//Create copy of old piece
		Piece_draw *newPiece = new Piece_draw{ *oldPiece, *this, pieceTexture };
		pieces.push_back(newPiece);
		++piece_count;
	}
}

Piece_draw * Game_drawable::addPiece(Piece::Type type, Side side, Position square)
{
	Piece_draw *newPiece = new Piece_draw{ type, side, piece_count, *this, pieceTexture };

	if (newPiece)
	{
		pieces.push_back(newPiece);	//C++17
		placePiece(piece_count, square);
		piece_count++;
		newPiece->setMoved(false);
	}
	return newPiece;

}
//Add piece at a row and column, row and column indices start with 0
Piece_draw * Game_drawable::addPiece(Piece::Type type, Side side, int row, int column)
{
	if (row > 7 or column > 7)
		return nullptr;

	return addPiece(type, side, Position(row, column));
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

PieceID Game_drawable::selectPiece(const sf::Vector2f & worldCoords)
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
					return drawableSquare->getPieceID();
				}
			}
		}
	}
	return PieceID{};
}

void Game_drawable::onMouseClick(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window)
{

	//std::string print;

	////if (whiteSideActive)
	////	print += "White ";
	////else
	////	print += "Black ";

	//switch (gameState)
	//{
	//case Game_drawable::GameState::SelectingPiece:
	//	//print += "Selecting piece\n";
	//	break;
	//case Game_drawable::GameState::MovingPiece:
	//	//print += "Moving piece\n";
	//	break;
	//default:
	//	break;
	//}

	//std::cout << print;

	playGame(event, window);

	//std::cout << selectSquare(window.mapPixelToCoords(sf::Vector2i(event.x, event.y)))->identify() << "\n";
}

void Game_drawable::highlight(const std::vector<Position> & list, sf::Color color)
{
	for (const Position & validMove : list)
	{
		Square_draw & drawableSquare = static_cast<Square_draw&>(squares.at(validMove));
		drawableSquare.highlight(color);
	}
}

void Game_drawable::playGame(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window)
{
	static Piece *selectedPiece = nullptr;
	static Square_draw *selectedSquare = nullptr;

	//Allow to deselect piece with right-click - noob mode
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
		selectedPiece = pieces.at(selectPiece(worldCoords));
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
			auto validMoves = selectedPiece->getLegalMoves();
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
		Timer timer{ "MovingPiece" };
		sf::Vector2f worldCoords = window.mapPixelToCoords(sf::Vector2i(event.x, event.y));
		Square_draw *newSelectedSquare = selectSquare(worldCoords);
		auto validMoves = selectedPiece->getLegalMoves();
		//Check if selectedPiece is not a nullptr, and if selected move square is valid
		if (newSelectedSquare and std::find(validMoves.begin(),validMoves.end(), newSelectedSquare->getPos()) != validMoves.end())
		{
			//Move the piece, and take any piece on new square
			movePiece(selectedSquare->getPos(), newSelectedSquare->getPos());
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

			gameState = GameState::SelectingPiece;

			//Switch active side
			switchActiveSide();

			//Highlight attacked squares by new side
			auto attackedSquares = getAttackedSquares(activeSide);
			highlight(attackedSquares, sf::Color::Red);

			//Check if game ended.
			if (attackedSquares.size() == 0)
			{
				if (whiteChecked and !blackChecked)
					std::cout << "Checkmate! Black wins.\n";
				else if (blackChecked and !whiteChecked)
					std::cout << "Checkmate! White wins.\n";
				else
					std::cout << "Stalemate. Game ends with draw.\n";
			}
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

