#pragma once

// Test_container.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <array>
#include <type_traits>
#include <string>
#include <chrono>


#include "Square.h"
#include "Square_drawable.h"

namespace Chess
{
	template<class Type = Square *>
	class Row
		: public std::array<Type, 8>
	{
	private:
		//typename std::remove_pointer<Type>::type noPointer;
		typename std::remove_pointer<Type>::type nullValue;
		Type nullAddress = &nullValue;
	public:

		Row() 
		{	
			//nullAddress = &nullValue;
		};
		virtual ~Row() {};

		//Copy constructors of other types
		template <class OtherType>
		Row(const Row<OtherType> &otherCont)
		{

			static_assert(std::is_convertible<OtherType, Type>::value, "Row: Cannot implicitly convert pointers in copy constructor\n");
			std::cout << "Row: Calling copy constructor of Row\n";
			for (const auto & element : otherCont)
			{
				this->push_back(element);
			}
		}

		Type& operator[](const int index)
		{
			if (index >= 0 and index < 8)
				return std::array<Type, 8>::operator[](index);
			else
				return nullAddress;
		}

		const Type& operator[](const int index) const
		{
			if (index >= 0 and index < 8)
				return std::array<Type, 8>::operator[](index);
			else
			{
				return nullAddress;
			}
		}

		//iterator begin()
		//{
		//	return &(*(array.begin() + 2));
		//}

		//const_iterator begin() const
		//{
		//	return &(*(array.cbegin() + 2));
		//}

		//iterator end()
		//{
		//	return &(*(array.end() - 2));
		//}

		//const_iterator end() const
		//{
		//	return &(*(array.end() - 2));
		//}
	};

	template<class Type = Square * >
	class Board :
		public std::array<Row<Type>, 8> 
	{
	private:
		Row<Type> nullValue = {};
	public:
		typedef Row<Type>* iterator;
		typedef const Row<Type>* const_iterator;

		Board() {};
		virtual ~Board() {};

		//Copy constructors of other types
		template <class OtherType>
		Board(const Board<OtherType> &otherCont)
		{
			static_assert(std::is_convertible<OtherType, Type>::value, "Board: Cannot implicitly convert pointers in copy constructor\n");
			std::cout << "Board: Calling copy constructor of Board\n";
			for (const Row<OtherType> & row : otherCont)
			{
				for (const OtherType & element : row)
					this->push_back(element);
			}
		}

		virtual Row<Type> & operator[](const int index)
		{
			if (index >= 0 and index < 8)
				return std::array<Row<Type>, 8>::operator[](index);
			else
				return nullValue;
		}

		virtual const Row<Type> & operator[](const int index) const
		{
			if (index >= 0 and index < 8)
				return std::array<Row<Type>, 8>::operator[](index);
			else
				return nullValue;
		}

		//iterator begin()
		//{
		//	return &(*(array.begin() + 2));
		//}

		//const_iterator begin() const
		//{
		//	return &(*(array.begin() + 2));
		//}

		//iterator end()
		//{
		//	return &(*(array.end() - 2));
		//}

		//const_iterator end() const
		//{
		//	return &(*(array.end() - 2));
		//}
	};
}