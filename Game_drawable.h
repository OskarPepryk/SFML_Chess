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
		//Typedefing a board array
		//Assuming that the boardArray is array of rows.
		// f.e. squares[row][column]
		//TODO: Non hardcoded board size
		//TODO: Implement own container with starting index 1
		using boardRow = std::array<Square_draw, 8>;
		using boardArray_t = std::array<boardRow, 8>;

	protected:
		sf::FloatRect			bounds{ 0, 0, 600, 600 };
		sf::Texture				pieceTexture;

	public:
		Game_drawable();

		Game_drawable(const Game & game);

		Piece_draw * addPiece(Piece::Type type, Side side, Square * square);

		Piece_draw * addPiece(Piece::Type type, Side side, int row, int column);

		void populateBoard();

		//Piece_draw* pickUpPiece(Square_draw & from);

		//void movePiece(Square_draw & oldSquare, Square_draw & newSquare);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void unhighlightAll();

		Square_draw* selectSquare(const sf::Vector2f &worldCoords);

		Piece* selectPiece(const sf::Vector2f &worldCoords);

		void onMouseClick(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);

		void highlight(const std::vector<Square*> & list, sf::Color color);

		void playGame(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);
		
	};
}

