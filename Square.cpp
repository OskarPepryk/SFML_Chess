#include "stdafx.h"

#include "Square.h"

using namespace Chess;


std::string Square::identify() const
{
	std::string name;
	//lowercase letter for column
	name += static_cast<char>(m_column + 97);
	//number for row
	name += static_cast<char>(m_row + 49);
	return name;
}
