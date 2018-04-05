#include "stdafx.h"
#include "Game_drawable.h"
#include "Timer.h"

using namespace Chess;

Game_drawable::Game_drawable(const ResourceManager& resources, bool upsideDown) : upsideDown{ upsideDown }, resources{ resources }
{
	////Load piece texture
	//if (!pieceTexture.loadFromFile("chess_sprites.png"))
	//	throw "Error loading piece texture";

	//pieceTexture.setSmooth(true);
	//Popualte the board with squares
	for (int row = 0; row < 8; row++)
		for (int column = 0; column < 8; column++)
		{
			Square_draw *square = new Square_draw{ row, column, bounds, resources.getFontTahoma(), upsideDown };
			squares.set(row, column).reset(square);
		}
}

//Deep copy constructor from non-drawable (or drawable) game. pieces with unassigned squares will not be copied
Game_drawable::Game_drawable(const Game & other, const ResourceManager& resources, bool upsideDown) : upsideDown{ upsideDown }, resources{ resources }
{

	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			auto oldSquare = other.getSquares().at(row, column);
			//Create new drawable square from copy
			Square_draw *newSquare = new Square_draw(*oldSquare, bounds, resources.getFontTahoma(), upsideDown);
			//Add square ptr to table (overwrite really)
			squares.set(row, column).reset(newSquare);
		}
	}
	//Deep copy pieces
	for (const auto oldPiece : other.getPieces())
	{
		//Create copy of old piece
		auto newPiece = std::make_shared<Piece_draw>(*oldPiece, *this, resources.getTexture());
		pieces.push_back(newPiece);
		++piece_count;
	}
}

Game_drawable& Game_drawable::operator=(const Game & other)
{
	if (this == &other)
		return *this;

	std::cout << "Called operator= of Game_drawable\n";

	piece_count = other.getPieceCount();

	gameState = other.getGameState();
	activeSide = other.getActiveSide();

	whiteChecked = other.getWhiteChecked();
	blackChecked = other.getBlackChecked();

	//m_selectedPiece = other.m_selectedPiece;

	//std::cout << "Calling copy constructor of base Game class\n";
	//Deep copy squares
	for (int row = 0; row < 8; row++)
	{
		for (int column = 0; column < 8; column++)
		{
			auto oldSquare = other.getSquares().at(row, column);
			//Create new drawable square from copy
			Square_draw *newSquare = new Square_draw(*oldSquare, bounds, resources.getFontTahoma(), upsideDown);
			//Add square ptr to table (overwrite really)
			squares.set(row, column).reset(newSquare);
		}
	}
	//Deep copy pieces
	for (auto piece : pieces )
	{
		//Delete current piece
		piece.reset();
	}
	piece_count = 0;
	pieces.clear();

	for (PieceID id = 0; id.valid(); id = id + 1)
	{
		auto newPiece = std::make_shared<Piece_draw>(*other.getPieces().at(id), *this, resources.getTexture());
		pieces.push_back(newPiece);
		++piece_count;
	}


	return *this;
}

void Game_drawable::addPiece(Piece::Type type, Side side, Position square)
{
	auto newPiece = std::make_shared<Piece_draw>(type, side, piece_count, *this, resources.getTexture());

	if (newPiece)
	{
		//Add new piece ptr to PieceSet
		pieces.push_back(newPiece);	//C++17
		//Place the piece (using old piece_count which is same as newPiece PieceID
		//piece_count starts as 0 during construction of Game
		placePiece(piece_count, square);
		//Increment piece count for next pieces.
		piece_count++;
		//Teleport sprite to it's location, to avoid animation.
		newPiece->teleport();
		newPiece->setMoved(false);
	}

}
//Add piece at a row and column, row and column indices start with 0
void Game_drawable::addPiece(Piece::Type type, Side side, int row, int column)
{
	if (row > 7 or column > 7)
		return;

	addPiece(type, side, Position(row, column));
}

void Game_drawable::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (auto square : squares)
	{
		auto drawableSquare = std::static_pointer_cast<Square_draw>(square);
		drawableSquare->draw(target, states);
	}

	for (auto piece : pieces)
	{
		auto drawablePiece = std::static_pointer_cast<Piece_draw>(piece);
		drawablePiece->draw(target, states);
	}
}

void Game_drawable::unhighlightAll()
{
	for (auto & square : squares)
	{
		auto drawableSquare = std::static_pointer_cast<Square_draw>(square);
		drawableSquare->highlight(sf::Color::Transparent);
	}
}

std::shared_ptr<Square_draw> Game_drawable::selectSquare(const sf::Vector2f & worldCoords)
{
	for (auto & square : squares)
	{
		auto drawableSquare = std::static_pointer_cast<Square_draw>(square);
		if (drawableSquare->checkInBounds(worldCoords))
		{
			return drawableSquare;
		}
	}
	return nullptr;
}

Position Game_drawable::selectPosition(const sf::Vector2f & worldCoords) 
{
	for (auto & square : squares)
	{
		auto drawableSquare = std::static_pointer_cast<Square_draw>(square);
		if (drawableSquare->checkInBounds(worldCoords))
		{
			return drawableSquare->getPos();
		}
	}
	return Position{};
}

PieceID Game_drawable::selectPiece(const sf::Vector2f & worldCoords)
{

	for (auto & square : squares)
	{
		auto drawableSquare = std::static_pointer_cast<Square_draw>(square);
		if (drawableSquare->checkInBounds(worldCoords))
		{
			return drawableSquare->getPieceID();
		}
	}
	return PieceID{};
}

void Game_drawable::onMouseClick(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window)
{
	playGame(event, window);
}

void Game_drawable::onMouseMoved(int x, int y, const sf::RenderWindow & window)
{
	static Position selectedSquare;
	static GameState gameStateOld;

	sf::Vector2f worldCoords = window.mapPixelToCoords(sf::Vector2i(x, y));

	Position newSelectedSquare = selectPosition(worldCoords);
	//Ignore event if selected piece is still the same, unless gamestate changed.
	if (!newSelectedSquare.valid() or ((newSelectedSquare == selectedSquare) and (gameState == gameStateOld)))
		return;

	switch (gameState)
	{
		//White player selects a piece to move.
	case GameState::SelectingPiece:
	{
		unhighlightAll();
		selectedSquare = newSelectedSquare;
		gameStateOld = gameState;
		//Always highlight selected square:
		highlight(selectedSquare, sf::Color{ 0,0,255,150 });
		const auto & pieceID = squares.at(selectedSquare)->getPieceID();
		if (pieceID.valid())
		{
			auto hoveredPiece = pieces.at(pieceID);
			//If selected piece is of the current active side, highlight legal moves of the piece
			if (pieces.at(pieceID)->getSide() == activeSide)
			{
				highlight(pieces.at(pieceID)->getLegalMoves(), sf::Color::Green);
				//Highlight attacking pieces in red
				highlight(hoveredPiece->getAttackingPieces(), sf::Color::Red);
			}
			else
			{	//For opponent pieces
				//Highlight own side attacking pieces in green
				highlight(hoveredPiece->getAttackingPieces(), sf::Color::Green);
				//Highlight squares it can attack in red
				highlight(pieces.at(pieceID)->getLegalMoves(), sf::Color::Red);
			}

		}
		break;
	}
	case GameState::MovingPiece:
	{
		unhighlightAll();
		selectedSquare = newSelectedSquare;
		//Always highlight selected square:
		highlight(selectedSquare, sf::Color{ 0,0,255,150 });
		//Highlight selected Piece
		highlight(m_selectedPiece, sf::Color::Green);
		//Highlight legal moves
		const auto & legalMoves = pieces.at(m_selectedPiece)->getLegalMoves();
		highlight(legalMoves, sf::Color::Green);
		//Highlight red opponent pieces that attack selected legal move square
		if (std::find(legalMoves.begin(), legalMoves.end(), selectedSquare) != legalMoves.end())
		{
			//Create a copy of the game with simulated move
			Game copy{ *this };
			copy.movePiece(pieces.at(m_selectedPiece)->getPos(), selectedSquare, true);
			//Highlight red all enemy pieces that would attack selected piece after making this move
			for (const auto piece : copy.getPieces())
			{
				if (piece->getSide() == activeSide)
					continue;
				const auto & otherlegalMoves = piece->getLegalMoves();
				if (std::find(otherlegalMoves.begin(), otherlegalMoves.end(), selectedSquare) != otherlegalMoves.end())
					highlight(piece->getPos(), sf::Color::Red);
			}
			//Highlight green all enemy pieces that selected piece attack after making this move
			auto legalMovesOfMe = copy.getPieces().at(m_selectedPiece)->getLegalMoves();
			for (const auto piece : copy.getPieces())
			{
				auto enemyPos = piece->getPos();
				if (std::find(legalMovesOfMe.begin(), legalMovesOfMe.end(), enemyPos) != legalMovesOfMe.end())
					highlight(piece->getID(), sf::Color::Green);
			}
		}
		break;
	}
	}
}

void Game_drawable::highlight(const std::vector<Position> & list, sf::Color color)
{
	for (const Position & validMove : list)
	{
		auto drawableSquare = std::static_pointer_cast<Square_draw>(squares.at(validMove));
		drawableSquare->highlight(color);
	}
}

void Game_drawable::highlight(const Position & pos, sf::Color color)
{
	auto drawableSquare = std::static_pointer_cast<Square_draw>(squares.at(pos));
	drawableSquare->highlight(color);
}

void Game_drawable::highlight(const std::vector<PieceID> & list, sf::Color color)
{
	for (const PieceID & id : list)
	{
		const Position & pos = pieces.at(id)->getPos();
		if (!pos.valid())
			continue;
		highlight(pos, color);
	}
}

void Game_drawable::highlight(const PieceID & id, sf::Color color)
{

	const Position & pos = pieces.at(id)->getPos();
	if (!pos.valid())
		return;
	highlight(pos, color);

}

void Game_drawable::playGame(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window)
{
	//Allow to deselect piece with right-click - noob mode
	if (event.button == sf::Mouse::Button::Right)
	{
		m_selectedPiece = PieceID{};
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
		m_selectedPiece = selectPiece(worldCoords);
		//If no selected piece, don't advance the game !!!!!!!
		if (!m_selectedPiece.valid())
		{
			//std::cout << "Failed with selecting a piece.\n";
			break;
		}
		//Check if selected piece belongs to player
		if (pieces.at(m_selectedPiece)->getSide() == activeSide)
		{
			//std::cout << "Succeeded with selecting a piece.\n";
			//Highlight square with selected piece
			//highlight(m_selectedPiece, sf::Color::Green);
			//Check if the piece can move.
			const auto & validMoves = pieces.at(m_selectedPiece)->getLegalMoves();
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
		Position newSelectedSquare = selectPosition(worldCoords);
		const auto & validMoves = pieces.at(m_selectedPiece)->getLegalMoves();
		//Check if selectedPiece is not a nullptr, and if selected move square is valid
		if (newSelectedSquare.valid() and std::find(validMoves.begin(),validMoves.end(), newSelectedSquare) != validMoves.end())
		{
			//Create undo checkpoint
			createUndo();
			//Move the piece, and take any piece on new square
			movePiece(pieces.at(m_selectedPiece)->getPos(), newSelectedSquare);
			//Reset command state
			m_selectedPiece = PieceID{};
			//Check for mates
			checkForMates();

			if (whiteChecked)
				std::cout << "White is mated!\n";
			if (blackChecked)
				std::cout << "Black is mated!\n";

			gameState = GameState::SelectingPiece;

			//Switch active side
			switchActiveSide();

			//Highlight attacked squares by new side
			auto attackedSquares = getAttackedSquares(activeSide);
			//highlight(attackedSquares, sf::Color::Red);

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
			unhighlightAll();
			break;
		}
	}
	}

}
void Chess::Game_drawable::switchActiveSide()
{
	Game::switchActiveSide();

	for (auto piece : pieces)
	{
		auto drawable_piece = std::static_pointer_cast<Piece_draw>(piece);
		if (drawable_piece->getSide() == activeSide)
			drawable_piece->fade(false);
		else
			drawable_piece->fade(true);

	}
}

void Game_drawable::animate()
{
	for (auto piece : pieces)
	{
		auto drawablePiece = std::static_pointer_cast<Piece_draw>(piece);
		drawablePiece->animate();
	}
}
