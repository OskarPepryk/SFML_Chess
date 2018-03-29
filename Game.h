#pragma once

#include "Square.h"
#include "Piece.h"
#include "BoardMatrix.h"

namespace Chess
{
	class Game
	{

	public:
		enum class GameState
		{
			SelectingPiece,
			MovingPiece
		};
	protected:
		Board			squares;
		std::vector<Piece*>		pieces;

		GameState				gameState = GameState::SelectingPiece;
		Side					activeSide = Side::White;

		bool					whiteChecked = false;
		bool					blackChecked = false;


	public:
		Game();

		//Deep copy constructor, pieces with unassigned squares will not be copied
		Game(const Game& other);

		Board & getSquares();

		const Board & getSquares() const;

		virtual Piece * addPiece(Piece::Type type, Side side, Square * square);

		virtual Piece * addPiece(Piece::Type type, Side side, int row, int column);

		void populateBoard();

		void placePiece(Piece * piece, Square * square);

		Piece* pickUpPiece(Square & from);

		Square* pickUpPiece(Piece & piece);

		void movePiece(Square & oldSquare, Square & newSquare);

		void takePiece(Square & square)
		{
			Piece * piece = square.getPiece();
			
			//If the square had a piece on it, take it
			if (piece)
			{
				piece->setTakenSquare(nullptr);
				piece->setDead(true);
				square.setPiece(nullptr);
			}
		}

		void checkForMates();

		//Returns true if any player is checked.
		bool getChecked()
		{
			checkForMates();
			return whiteChecked or blackChecked;
		}

		bool getChecked(Side side)
		{
			checkForMates();
			return ((whiteChecked and side == Side::White) or (blackChecked and side == Side::Black));
		}

		const std::vector<Piece*>& getPieces() const
		{
			return pieces;
		}

		std::vector<Square*> getAttackedSquares(Side bySide);
	};
}

