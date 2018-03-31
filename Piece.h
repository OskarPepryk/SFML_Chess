#pragma once

#include "Direction.h"
#include "Side.h"
#include "ID.h"

namespace Chess
{

	class Game;
	class Square;
	class Board;

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
	protected:
		Type		m_type = Type::Pawn;
		Side		m_side = Side::White;

		bool		m_hasMoved = false;
		bool		m_isDead = false;

		Position	m_pos;
		PieceID		m_id;

	public:

		Piece()
		{};

		Piece(Type type, Side side, int id) : m_id{ id }
		{
			m_type = type;
			m_side = side;
		}

		virtual ~Piece() = default;

		Position& getTakenSquare();

		const Position& getTakenSquare() const;

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

		PieceID getID() const
		{
			return m_id;
		}

		virtual void setTakenSquare(Position &position, Board * board = nullptr);

		void setTakenSquare(Position && position);

	private:
		void getPseudoLegalMovesInDirection(Game & game, std::vector<Position> &validSquares, Directions::DirectionSet dirSet, int maxRange = 1, bool canJumpOver = false) const;
	public:
		std::vector<Position> getPseudoLegalMoves(Game & board, bool includeNonTakingMoves = true) const;

		std::vector<Position> getLegalMoves(Game & game) const;

		bool isAttacked(Game & game);
	};
}