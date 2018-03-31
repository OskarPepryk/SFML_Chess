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

		PieceID		m_enPassantPiece;

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

		void setPieceID(const PieceID&& newID)
		{
			m_piece = newID;
		}

		void setPieceID(const PieceID& newID)
		{
			m_piece = newID;
		}

		const PieceID& getEnPassantPieceID() const
		{
			return m_enPassantPiece;
		}

		virtual void setEnPassantPieceID(const PieceID&& newID)
		{
			m_enPassantPiece = newID;
		}

		virtual void setEnPassantPieceID(const PieceID& newID)
		{
			m_enPassantPiece = newID;
		}

		bool isValid() const
		{
			return m_pos.valid();
		}

		bool isAttacked(Game & board, Side bySide);
	};
}