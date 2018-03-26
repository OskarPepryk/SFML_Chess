#pragma once

#include <iostream>
#include <string>

namespace Chess
{

	class Piece;
	class Game;

	class Square
	{
	protected:
		int m_index;
		int m_column;		//Not necessary, index is enough
		int m_row;			//Not necessary, index is enough

		Piece	*m_piece;

	public:
		friend class Game;
		friend class Piece;

		Square()
		{

		}

		virtual ~Square() = default;

		Square(int row, int column) : m_index{ row * 8 + column }, m_row{ row }, m_column{ column }
		{
			//initialize(board);
		}

		std::string identify() const;

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

		void setPiece(Piece * piece)
		{
			m_piece = piece;
		}
	};
}