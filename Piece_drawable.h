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

		Piece_draw(Type type, Side side, int id, Game & parent, const sf::Texture &texture) : Piece{type, side, id, parent}
		{
			setTexture(texture);
			m_sprite.setScale(0.3f, 0.3f);
			auto size = m_sprite.getLocalBounds();
			m_sprite.setOrigin(size.width / 2, size.height / 2);
		}

		Piece_draw(const Piece & piece, Game & parent, const sf::Texture &texture) : Piece{ piece }
		{
			setTexture(texture);
			m_sprite.setScale(0.3f, 0.3f);
			auto size = m_sprite.getLocalBounds();
			m_sprite.setOrigin(size.width / 2, size.height / 2);

		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void setTexture(const sf::Texture & texture);
		void setTexture();

		virtual void setTakenSquare(Position &position) override;

		virtual void promote(Type type) override
		{
			Piece::promote(type);
			setTexture();
		}
	};
}