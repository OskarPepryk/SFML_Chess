#pragma once

#include <iostream>
#include <string>

#include "Side.h"
#include "ID.h"

namespace Chess
{

	class Piece;
	class Game;

	class Square
	{
	protected:
		Position	m_pos;
		PieceID		m_piece;

	public:

		Square()
		{
			
		}

		virtual ~Square() = default;

		Square(int row, int column) : m_pos{ row, column }
		{
			//initialize(board);
		}

		std::string identify() const;

		const Position& getPos() const
		{
			return m_pos;
		}

		const PieceID& getPieceID() const
		{
			return m_piece;
		}

		void setPieceID(PieceID newID)
		{
			m_piece = newID;
		}

		bool isValid() const
		{
			return m_pos.valid();
		}

		bool isAtacked(Game & board, Side bySide);
	};
}