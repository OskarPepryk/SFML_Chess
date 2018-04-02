#pragma once

#include "Square.h"
#include "Piece.h"
#include "BoardMatrix.h"
#include "PieceSet.h"

namespace Chess
{
	class Setups;

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
		PieceSet		pieces;
		int				piece_count = 0;

		GameState		gameState = GameState::SelectingPiece;
		Side			activeSide = Side::White;

		bool			whiteChecked = false;
		bool			blackChecked = false;

		bool			quiet = true;


	public:
		friend class Setups;

		Game();

		virtual ~Game()
		{
			//Delete pieces
			for (Piece* piece : pieces)
			{
				delete piece;
			}

			//Delete squares
			for (auto row : squares)
			{
				for (Square* square : row)
				{
					delete square;
				}
			}
		}

		//Deep copy constructor, pieces with unassigned squares will not be copied
		Game(const Game& other);

		Board & getSquares();

		const Board & getSquares() const;

		virtual Piece * addPiece(Piece::Type type, Side side, Position square);

		virtual Piece * addPiece(Piece::Type type, Side side, int row, int column);

		void populateBoard();

		void placePiece(PieceID pieceID, Position pos);

		PieceID pickUpPiece(Position from);

		Position pickUpPiece(PieceID piece);

		void movePiece(Position oldSquare, Position newSquare, bool simulated = false);

		void takePiece(Position pos);

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

		const PieceSet& getPieces() const
		{
			return pieces;
		}

		std::vector<Position> getAttackedSquares(Side bySide);
		void switchActiveSide();

		void refreshAllLegalMoves(bool pseudoLegal = false);
	};
}

