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
		//boardArray_t			squares;

		sf::Texture				pieceTexture;

	public:
		Game_drawable();

		void addPiece(Piece::Type type, Piece_draw::Side side, Square_draw * square);

		void addPiece(Piece_draw::Type type, Piece_draw::Side side, int row, int column);

		void populateBoard();

		Piece_draw* pickUpPiece(Square_draw & from);

		void movePiece(Square_draw & oldSquare, Square_draw & newSquare);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void unhighlightAll();

		Square_draw* selectSquare(const sf::Vector2f &worldCoords);

		Piece_draw* selectPiece(const sf::Vector2f &worldCoords);

		void onMouseClick(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);

		void playGame(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);
		
		void placePiece(Piece_draw * piece, Square_draw * square);
	};
}

