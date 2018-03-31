#pragma once

namespace Chess 
{
	class PieceID
	{
	private:
		int m_id = -1;
	public:
		PieceID() {};
		PieceID(int id) : m_id{ id }
		{}

		bool valid() const
		{
			return m_id >= 0 and m_id < 32;
		}

		operator int() const
		{
			return m_id;
		}

		friend bool operator== (const PieceID & piece1, const PieceID & piece2);
		friend bool operator!= (const PieceID & piece1, const PieceID & piece2);

	};

	class Position //like SquareID
	{
	public:
		int row = -1;
		int column = -1;
		Position() {};

		Position(int row, int column) : row{ row }, column{ column }
		{};

		bool valid() const
		{
			return row >= 0 and row < 8 and column >= 0 and column < 8;
		}

		int getIndex() const
		{
			return row * 8 + column;
		}

		friend bool operator== (const Position & pos1, const Position & pos2);
		friend bool operator!= (const Position & pos1, const Position & pos2);
	};
}

