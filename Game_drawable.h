#pragma once

#include "Game.h"
#include "Square_drawable.h"
#include "Piece_drawable.h"
#include "ResourceManager.h"

namespace Chess
{
	class Game_drawable :
		public Game,
		virtual public sf::Drawable
	{
	private:
		sf::FloatRect			bounds{ 0, 0, 600, 600 };
		
		bool					upsideDown = false;

		PieceID					m_selectedPiece;
		
		Game_drawable*					undoGame = nullptr;

	public:
		const ResourceManager&	resources;

		Game_drawable() = delete;

		Game_drawable(const ResourceManager& resources, bool upsideDown);

		Game_drawable(const Game & game, const ResourceManager& resources, bool upsideDown = false);

		Game_drawable & operator=(const Game_drawable & other);



		Piece_draw * addPiece(Piece::Type type, Side side, Position square) override;

		Piece_draw * addPiece(Piece::Type type, Side side, int row, int column) override;

		//Piece_draw* pickUpPiece(Square_draw & from);

		//void movePiece(Square_draw & oldSquare, Square_draw & newSquare);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void unhighlightAll();

		Square_draw* selectSquare(const sf::Vector2f &worldCoords);

		Position selectPosition(const sf::Vector2f & worldCoords);

		PieceID selectPiece(const sf::Vector2f &worldCoords);

		void onMouseClick(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);

		void onMouseMoved(int x, int y, const sf::RenderWindow & window);

		void highlight(const std::vector<Position> & list, sf::Color color);
		void highlight(const std::vector<PieceID> & list, sf::Color color);
		void highlight(const PieceID & id, sf::Color color);
		void highlight(const Position & pos, sf::Color color);

		void playGame(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);

		void switchActiveSide() override;

		virtual void createUndo()
		{
			delete undoGame;
			undoGame = new Game_drawable{ *this, resources, upsideDown };
		}
		
		virtual void undo()
		{
			Game_drawable * temp = undoGame;
			if (undoGame)
				*this = *undoGame;
			delete temp;
		}

	};
}

