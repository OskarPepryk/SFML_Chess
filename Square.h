#pragma once

#include <iostream>
#include <string>

namespace Chess
{

	class Piece;
	class Game;

	class Square : sf::Drawable
	{
	private:
		int m_index;
		int m_column;		//Not necessary, index is enough
		int m_row;			//Not necessary, index is enough

		sf::RectangleShape	m_shape;
		sf::Color			m_originalColor;
		//Always check for nullptr, square can be empty
		Piece				*m_piece;

	public:
		friend class Game;
		friend class Piece;

		Square()
		{

		}

		Square(int row, int column, const sf::FloatRect &board) : m_index{ row * 8 + column }, m_row{ row }, m_column{ column }
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

		Piece * getPiece()
		{
			return m_piece;
		}

		const Piece * getPiece() const
		{
			return m_piece;
		}
	};
}