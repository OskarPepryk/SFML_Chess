#pragma once

#include <iostream>
#include <string>

#include "Side.h"

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

		Square() : m_index{ -1 }
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

		int getIndex() const
		{
			return m_index;
		}

		bool isValid() const
		{
			if (m_index >= 0 and m_index < 64)
				return true;
			else
				return false;
		}

		bool isAtacked(Game * board, Side bySide);
	};
}