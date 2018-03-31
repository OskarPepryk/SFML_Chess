#pragma once

#include "Direction.h"
#include "Piece.h"

namespace Chess
{

	class Game_drawable;
	class Square_draw;

	class Piece_draw :
		public Piece,
		virtual public sf::Drawable
	{
	private:
		sf::Sprite	m_sprite;

	public:

		Piece_draw(Type type, Side side, int id, const sf::Texture &texture) : Piece(type, side, id)
		{
			setTexture(type, side, texture);
			m_sprite.setScale(0.3f, 0.3f);
			auto size = m_sprite.getLocalBounds();
			m_sprite.setOrigin(size.width / 2, size.height / 2);
		}

		Piece_draw(const Piece & piece, const sf::Texture &texture) : Piece{ piece }
		{
			setTexture(m_type, m_side, texture);
			m_sprite.setScale(0.3f, 0.3f);
			auto size = m_sprite.getLocalBounds();
			m_sprite.setOrigin(size.width / 2, size.height / 2);

		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void setTexture(Type type, Side side, const sf::Texture &texture);

		virtual void setTakenSquare(Position &position, Board * board = nullptr) override;
	};
}