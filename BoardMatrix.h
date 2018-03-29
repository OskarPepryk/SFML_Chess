#pragma once

// Test_container.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <type_traits>
#include <string>
#include <chrono>


#include "Square.h"
#include "Square_drawable.h"

namespace Chess
{
	class Board
	{
	private:
		Square nullSquare;
		std::vector<std::vector<Square*>> array;
	public:
		Board() {
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

		Square* at(int row, int column)
		{
			if ((row >= 0 and row < static_cast<int>(array.size()))
				and (column >= 0 and column < static_cast<int>(array[row].size())))
				return array[row][column];
			else
				return &nullSquare;
		}

		const Square* at(int row, int column) const
		{
			if ((row >= 0 and row < static_cast<int>(array.size()))
				and (column >= 0 and column < static_cast<int>(array[row].size())))
				return array[row][column];
			else
				return &nullSquare;
		}

		Square*& set(int row, int column)
		{
			if ((row >= 0 and row < static_cast<int>(array.size()))
				and (column >= 0 and column < static_cast<int>(array[row].size())))
			{
				return array[row][column];
			}
			else
			{
				throw (std::out_of_range("Tried to assign square at out of range index."));
			}
		}

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