#pragma once

#include "Direction.h"

namespace Chess
{

	class Game;
	class Square;

	class Piece
	{
	public:
		enum class Type
		{
			Pawn,
			Rook,
			Knight,
			Bishop,
			Queen,
			King
		};
		enum class Side
		{
			White,
			Black
		};
	protected:
		Type		m_type = Type::Pawn;
		Side		m_side = Side::White;

		bool		m_hasMoved = false;
		bool		m_isDead = false;

		Square*		m_takenSquare;

	public:

		Piece(Type type, Side side)
		{
			m_type = type;
			m_side = side;
		}

		virtual ~Piece() = default;

		Square* getTakenSquare();

		const Square * getTakenSquare() const;

		Side getSide() const
		{
			return m_side;
		}

		bool isDead() const
		{
			return m_isDead;
		}

		void setDead(bool dead)
		{
			m_isDead = dead;
		}

		void setMoved(bool moved)
		{
			m_hasMoved = moved;
		}

		bool getMoved() const
		{
			return m_hasMoved;
		}


		Type getType() const
		{
			return m_type;
		}

		virtual void setTakenSquare(Square* newSquare);

		void getValidMovesInDirection(Game *board, std::list<Square*> &validSquares, Directions::DirectionSet dirSet, int maxRange, bool canJumpOver = false) const;

		std::list<Square*> getValidMoves(Game *board) const;
		bool checkAttacked(Game * board);
	};
}