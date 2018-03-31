#pragma once

#include <iostream>
#include <string>

#include "Square.h"
#include "Piece_drawable.h"

namespace Chess
{

	class Piece_draw;
	class Game_drawable;

	class Square_draw : 
		public Square,
		virtual public sf::Drawable
	{
	private:

		sf::RectangleShape	m_shape;
		sf::RectangleShape	m_highlight;
		sf::Color			m_originalColor;
		sf::Text			m_notation;

		sf::Font		m_font;

	public:

		Square_draw() = delete;

		virtual ~Square_draw() = default;

		Square_draw(int row, int column, const sf::FloatRect &board) : Square{ row, column }
		{
			initialize(board);
		}

		Square_draw(const Square & square, const sf::FloatRect &board) : Square{ square }
		{
			initialize(board);
		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void initialize(const sf::FloatRect &board);

		bool checkInBounds(const sf::Vector2f &worldCoords);

		std::string identify() const;

		void highlight(sf::Color color);

		sf::RectangleShape& getShape()
		{
			return m_shape;
		}

		const sf::RectangleShape& getShape() const
		{
			return m_shape;
		}
	};
}