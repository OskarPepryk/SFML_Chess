#pragma once

#include "Square.h"
#include "Piece.h"

namespace Chess
{
	class Game :
		public sf::Drawable
	{
		//Typedefing a board array
		//Assuming that the boardArray is array of rows.
		// f.e. squares[row][column]
		//TODO: Non hardcoded board size
		//TODO: Implement own container with starting index 1
		using boardRow = std::array<Square, 8>;
		using boardArray_t = std::array<boardRow, 8>;

	public:
		enum class GameState
		{
			SelectingPiece,
			MovingPiece
		};
	private:
		sf::FloatRect			bounds{ 0, 0, 600, 600 };
		sf::VertexArray			lines;
		boardArray_t			squares;
		std::vector<Piece*>		pieces;

		sf::Texture				pieceTexture;
		GameState				gameState = GameState::SelectingPiece;
		bool					whiteSideActive = true;

		bool					whiteChecked = false;
		bool					blackChecked = false;


	public:
		Game();

		boardArray_t & getSquares();

		const boardArray_t & getSquares() const;

		void addPiece(Piece::Type type, Piece::Side side, Square & square);

		void addPiece(Piece::Type type, Piece::Side side, int row, int column);

		void populateBoard();

		void placePiece(Piece * piece, Square * square);

		Piece* pickUpPiece(Square & from);

		Square* pickUpPiece(Piece & piece);

		void movePiece(Square & oldSquare, Square & newSquare);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void unhighlightAll();

		Square* selectSquare(const sf::Vector2f &worldCoords);

		Piece* selectPiece(const sf::Vector2f &worldCoords);

		void checkForMates();

		void onMouseClick(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);

		void playGame(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);

		const std::vector<Piece*>& getPieces() const
		{
			return pieces;
		}
	};
}

