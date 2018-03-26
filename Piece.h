#pragma once

#include "Direction.h"

namespace Chess
{

	class Game;
	class Square;

	class Piece :
		public sf::Drawable
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
	private:
		sf::Sprite	m_sprite;
		Type		m_type = Type::Pawn;
		Side		m_side = Side::White;

		bool		m_hasMoved = false;
		bool		m_isDead = false;

		Square*		m_takenSquare;

	public:

		Piece(Type type, Side side, sf::Texture &texture)
		{
			setTexture(type, side, texture);
			m_sprite.setScale(0.3f, 0.3f);
			auto size = m_sprite.getLocalBounds();
			m_sprite.setOrigin(size.width / 2, size.height / 2);
		}

		friend std::ostream& operator<< (std::ostream &out, const Piece &piece);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void setTexture(Type type, Side side, sf::Texture &texture);

		Square* getTakenSquare();

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

		void setTakenSquare(Square* newSquare);

		void getValidMovesInDirection(Game *board, std::list<Square*> &validSquares, Directions::DirectionSet dirSet, int maxRange, bool canJumpOver = false) const;

		std::list<Square*> getValidMoves(Game *board) const;
	};
}