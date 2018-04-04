#pragma once

// Test_container.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <memory>

#include "Square.h"
#include "ID.h"

namespace Chess
{
	class Board
	{
	private:
		Game & parentGame;
		Square nullSquare;
		std::vector<std::shared_ptr<Square>> array;
	public:
		Board() = delete;

		Board(Game & parentGame) : parentGame{ parentGame }
		{
			array.resize(8*8);
		};
		virtual ~Board() {};

		//Copy constructors of other types
		//template <class OtherType>
		//Board(const Board<OtherType> &otherCont)
		//{
		//	static_assert(std::is_convertible<OtherType, Square*>::value, "Board: Cannot implicitly convert pointers in copy constructor\n");
		//	std::cout << "Board: Calling copy constructor of Board\n";
		//	for (const Row<OtherType> & row : otherCont)
		//	{
		//		for (const OtherType & element : row)
		//			this->push_back(element);
		//	}
		//}

		//std::shared_ptr<Square> at(int row, int column);
		const std::shared_ptr<Square> at(int row, int column) const;

		//std::shared_ptr<Square> at(const Position &&pos);
		const std::shared_ptr<Square> at(const Position &&pos) const;
		//std::shared_ptr<Square> at(const Position &pos);
		const std::shared_ptr<Square> at(const Position &pos) const;

		//std::shared_ptr<Square> at(const PieceID &id);
		const std::shared_ptr<Square> at(const PieceID &id) const;
		//std::shared_ptr<Square> at(const PieceID &&id);
		const std::shared_ptr<Square> at(const PieceID &&id) const;

		std::shared_ptr<Square>& set(int row, int column);

		std::vector<std::shared_ptr<Square>>::iterator begin()
		{
			return array.begin();
		}

		std::vector<std::shared_ptr<Square>>::iterator end()
		{
			return array.end();
		}

		std::vector<std::shared_ptr<Square>>::const_iterator begin() const
		{
			return array.cbegin();
		}

		std::vector<std::shared_ptr<Square>>::const_iterator end() const
		{
			return array.end();
		}
	};
}