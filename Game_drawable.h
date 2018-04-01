#pragma once

#include "Game.h"
#include "Square_drawable.h"
#include "Piece_drawable.h"

namespace Chess
{
	class Game_drawable :
		public Game,
		virtual public sf::Drawable
	{
	private:
		sf::FloatRect			bounds{ 0, 0, 600, 600 };
		sf::Texture				pieceTexture;
		bool					upsideDown = false;

	public:
		Game_drawable(bool upsideDown);

		Game_drawable(const Game & game, bool upsideDown = false);

		Piece_draw * addPiece(Piece::Type type, Side side, Position square) override;

		Piece_draw * addPiece(Piece::Type type, Side side, int row, int column) override;

		//Piece_draw* pickUpPiece(Square_draw & from);

		//void movePiece(Square_draw & oldSquare, Square_draw & newSquare);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void unhighlightAll();

		Square_draw* selectSquare(const sf::Vector2f &worldCoords);

		PieceID selectPiece(const sf::Vector2f &worldCoords);

		void onMouseClick(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);

		void highlight(const std::vector<Position> & list, sf::Color color);

		void playGame(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);
		
	};
}

