#pragma once

#include <vector>
#include <memory>

#include "Piece.h"
#include "ID.h"

namespace Chess
{
	class PieceSet :
		public std::vector<std::shared_ptr<Piece>>
	{
	private:
		Game & parentGame;
		Piece nullPiece;
	public:
		PieceSet(Game & parent) : nullPiece{ parent }, parentGame{ parent }
		{

		}

		std::shared_ptr<Piece> at(const PieceID & id);
		const std::shared_ptr<Piece> at(const PieceID & id) const;

		std::shared_ptr<Piece> at(const Position& pos);
		const std::shared_ptr<Piece> at(const Position& pos) const;

		std::vector<std::shared_ptr<Piece>>::iterator begin()
		{
			return std::vector<std::shared_ptr<Piece>>::begin();
		}

		std::vector<std::shared_ptr<Piece>>::iterator end()
		{
			return std::vector<std::shared_ptr<Piece>>::end();
		}

		std::vector<std::shared_ptr<Piece>>::const_iterator begin() const
		{
			return std::vector<std::shared_ptr<Piece>>::cbegin();
		}

		std::vector<std::shared_ptr<Piece>>::const_iterator end() const
		{
			return std::vector<std::shared_ptr<Piece>>::end();
		}

	};
}