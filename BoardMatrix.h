#pragma once

// Test_container.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>


#include "Square.h"
#include "ID.h"

namespace Chess
{
	class Board
	{
	private:
		Game & parentGame;
		Square nullSquare;
		std::vector<std::vector<Square*>> array;
	public:
		Board() = delete;

		Board(Game & parentGame) : parentGame{ parentGame }
		{
			array.resize(8);

			for (auto & subarray : array)
			{
				subarray.resize(8);
			}
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

		Square& at(int row, int column);
		const Square& at(int row, int column) const;

		Square& at(const Position &&pos);
		const Square& at(const Position &&pos) const;
		Square& at(const Position &pos);
		const Square& at(const Position &pos) const;

		Square& at(const PieceID &id);
		const Square& at(const PieceID &id) const;
		Square& at(const PieceID &&id);
		const Square& at(const PieceID &&id) const;

		Square*& set(int row, int column);

		std::vector<std::vector<Square*>>::iterator begin()
		{
			return array.begin();
		}

		std::vector<std::vector<Square*>>::iterator end()
		{
			return array.end();
		}

		std::vector<std::vector<Square*>>::const_iterator begin() const
		{
			return array.cbegin();
		}

		std::vector<std::vector<Square*>>::const_iterator end() const
		{
			return array.end();
		}
	};
}