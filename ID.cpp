#include "stdafx.h"

#include "ID.h"

namespace Chess
{
	bool operator== (const PieceID & piece1, const PieceID & piece2)
	{
		return piece1.m_id == piece2.m_id;
	}
	bool operator!=(const PieceID & piece1, const PieceID & piece2)
	{
		return piece1.m_id != piece2.m_id;;
	}
	bool operator==(const Position & pos1, const Position & pos2)
	{
		return pos1.column == pos2.column and pos1.row == pos2.row;
	}
	bool operator!=(const Position & pos1, const Position & pos2)
	{
		return pos1.column != pos2.column or pos1.row != pos2.row;
	}
}