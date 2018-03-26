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
		sf::Color			m_originalColor;

	public:
		friend class Game_drawable;
		friend class Piece_draw;

		Square_draw()
		{

		}

		virtual ~Square_draw() = default;

		Square_draw(int row, int column, const sf::FloatRect &board) : Square(row, column)
		{
			initialize(board);
		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void initialize(const sf::FloatRect &board);

		bool checkInBounds(const sf::Vector2f &worldCoords);

		std::string identify() const;

		void highlight(bool highlight);

		int getRow() const
		{
			return m_row;
		}

		int getColumn() const
		{
			return m_column;
		}

		Piece_draw * getPiece()
		{
			Piece_draw * drawablePiece = dynamic_cast<Piece_draw*>(m_piece);
			if (drawablePiece)
				return drawablePiece;
			else
				return nullptr;
		}

		const Piece_draw * getPiece() const
		{
			const Piece_draw * drawablePiece = dynamic_cast<const Piece_draw*>(m_piece);
			if (drawablePiece)
				return drawablePiece;
			else
				return nullptr;
		}
	};
}