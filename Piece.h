#pragma once

#include <functional>

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
		std::reference_wrapper<Game>	m_parentGame;

		Type		m_type = Type::Pawn;
		Side		m_side = Side::White;

		bool		m_hasMoved = false;
		bool		m_isDead = false;

		Position	m_pos;
		PieceID		m_id;

		std::vector<Position> m_legalMoves;

	public:

		Piece(Game & parent) : m_parentGame{ parent }
		{};

		Piece(const Piece& toCopy, Game & newParent) : Piece{ toCopy }
		{
			//Reassing parent ref to the new one
			m_parentGame = newParent;
		};

		Piece(Type type, Side side, int id, Game & parent) : m_id{ id }, m_parentGame{ parent }
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

		virtual void setTakenSquare(Position &position);

		void setTakenSquare(Position && position);

	private:
		void getPseudoLegalMovesInDirection(std::vector<Position> &validSquares, Directions::DirectionSet dirSet, int maxRange = 1, bool canJumpOver = false) const;
	public:
		std::vector<Position> getPseudoLegalMoves(bool includeNonTakingMoves = true) const;

		const std::vector<Position>&  getLegalMoves() const;

		void refreshLegalMoves(bool pseudoLegal = false);

		bool isAttacked();

		virtual void promote(Type type)
		{
			m_type = type;
		}

		bool canPromote() const;
	};
}