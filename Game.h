#pragma once

#include "Square.h"
#include "Piece.h"
#include "Board.h"

namespace Chess
{
	class Game
	{
		//Typedefing a board array
		//Assuming that the boardArray is array of rows.
		// f.e. squares[row][column]
		//TODO: Non hardcoded board size
		//TODO: Implement own container with starting index 1
		using boardRow = std::array<Square*, 8>;
		using boardArray_t = std::array<boardRow*, 8>;

	public:
		enum class GameState
		{
			SelectingPiece,
			MovingPiece
		};
	protected:
		Board					squares;
		std::vector<Piece*>		pieces;

		GameState				gameState = GameState::SelectingPiece;
		bool					whiteSideActive = true;

		bool					whiteChecked = false;
		bool					blackChecked = false;


	public:
		Game();

		Board & getSquares();

		const Board & getSquares() const;

		void addPiece(Piece::Type type, Piece::Side side, Square * square);

		void addPiece(Piece::Type type, Piece::Side side, int row, int column);

		void populateBoard();

		void placePiece(Piece * piece, Square * square);

		Piece* pickUpPiece(Square & from);

		Square* pickUpPiece(Piece & piece);

		void movePiece(Square & oldSquare, Square & newSquare);

		void checkForMates();

		const std::vector<Piece*>& getPieces() const
		{
			return pieces;
		}
	};
}

