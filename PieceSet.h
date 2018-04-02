#pragma once

#include <vector>

#include "Piece.h"
#include "ID.h"

namespace Chess
{
	class PieceSet :
		public std::vector<Piece*>
	{
	private:
		
		Piece nullPiece;
	public:
		PieceSet(Game & parent) : nullPiece{ parent }
		{

		}

		Piece* at(const PieceID & id)
		{
			if (id.valid())
				return std::vector<Piece*>::operator[](id);
			else
				return &nullPiece;
		}

		const Piece* at(const PieceID & id) const
		{
			if (id.valid())
				return std::vector<Piece*>::operator[](id);
			else
				return &nullPiece;
		}

		std::vector<Piece*>::iterator begin()
		{
			return std::vector<Piece*>::begin();
		}

		std::vector<Piece*>::iterator end()
		{
			return std::vector<Piece*>::end();
		}

		std::vector<Piece*>::const_iterator begin() const
		{
			return std::vector<Piece*>::cbegin();
		}

		std::vector<Piece*>::const_iterator end() const
		{
			return std::vector<Piece*>::end();
		}

	};
}