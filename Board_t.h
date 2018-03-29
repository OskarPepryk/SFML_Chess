#pragma once

#include "Square.h"
#include "Square_drawable.h"
#include <array>

namespace Chess
{
	template <class T>

	class Row
		: public std::array<T, 8>
	{
	public:
		virtual T& operator[](const int index)
		{
			return std::array<T,8>::at(index);
		}

		virtual const T& operator[](const int index) const
		{
			return std::array<T, 8>::at(index);
		}

	};

	template <class T = Square*>

	class Board
		: public std::array<Row<T>, 8>
		//This class in no longer an agreggate like std::array
	{
	public:
		virtual Row<T> & operator[](const int index)
		{
			return std::array<Row<T>, 8>::at(index);
		}

		virtual Row<T> & operator[](const int index) const
		{
			return std::array<Row<T>, 8>::at(index);
		}
	};

	class Board_draw
		: public Board<Square_draw>
	{
		virtual Row<Square_draw> & operator[](const int index)
		{
			return std::array<Row<Square_draw>, 8>::at(index);
		}
	};
}