#pragma once

#include "Game.h"

namespace Chess
{
	class Setups
	{
	public:
		static void standardGame(Game & game)
		{
			//Add white pieces
			game.addPiece(Piece::Type::Rook, Side::White, 0, 0);
			game.addPiece(Piece::Type::Knight, Side::White, 0, 1);
			game.addPiece(Piece::Type::Bishop, Side::White, 0, 2);
			game.addPiece(Piece::Type::Queen, Side::White, 0, 3);
			game.addPiece(Piece::Type::King, Side::White, 0, 4);
			game.addPiece(Piece::Type::Bishop, Side::White, 0, 5);
			game.addPiece(Piece::Type::Knight, Side::White, 0, 6);
			game.addPiece(Piece::Type::Rook, Side::White, 0, 7);
			game.addPiece(Piece::Type::Pawn, Side::White, 1, 0);
			game.addPiece(Piece::Type::Pawn, Side::White, 1, 1);
			game.addPiece(Piece::Type::Pawn, Side::White, 1, 2);
			game.addPiece(Piece::Type::Pawn, Side::White, 1, 3);
			game.addPiece(Piece::Type::Pawn, Side::White, 1, 4);
			game.addPiece(Piece::Type::Pawn, Side::White, 1, 5);
			game.addPiece(Piece::Type::Pawn, Side::White, 1, 6);
			game.addPiece(Piece::Type::Pawn, Side::White, 1, 7);

			//Add black pieces
			game.addPiece(Piece::Type::Rook, Side::Black, 7, 7);
			game.addPiece(Piece::Type::Knight, Side::Black, 7, 6);
			game.addPiece(Piece::Type::Bishop, Side::Black, 7, 5);
			game.addPiece(Piece::Type::Queen, Side::Black, 7, 3);
			game.addPiece(Piece::Type::King, Side::Black, 7, 4);
			game.addPiece(Piece::Type::Bishop, Side::Black, 7, 2);
			game.addPiece(Piece::Type::Knight, Side::Black, 7, 1);
			game.addPiece(Piece::Type::Rook, Side::Black, 7, 0);
			game.addPiece(Piece::Type::Pawn, Side::Black, 6, 7);
			game.addPiece(Piece::Type::Pawn, Side::Black, 6, 6);
			game.addPiece(Piece::Type::Pawn, Side::Black, 6, 5);
			game.addPiece(Piece::Type::Pawn, Side::Black, 6, 4);
			game.addPiece(Piece::Type::Pawn, Side::Black, 6, 3);
			game.addPiece(Piece::Type::Pawn, Side::Black, 6, 2);
			game.addPiece(Piece::Type::Pawn, Side::Black, 6, 1);
			game.addPiece(Piece::Type::Pawn, Side::Black, 6, 0);

		}

		static void stalemate(Game & game)
		{
			//game.switchActiveSide();
			//Add white pieces
			game.addPiece(Piece::Type::Queen, Side::White, 0, 6);
			game.addPiece(Piece::Type::King, Side::White, 6, 5);

			//Add black pieces
			game.addPiece(Piece::Type::King, Side::Black, 7, 7);
		}

	};
}