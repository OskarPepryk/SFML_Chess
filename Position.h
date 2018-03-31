#pragma once

#include <SFML\Graphics.hpp>

namespace Chess
{
	class Position
		: public sf::Vector2i
	{
	public:
		Position() : sf::Vector2i{ -1, -1 }
		{};

		Position(int row, int column) : sf::Vector2i{row, column}
		{};

		bool isValid() const
		{
			return (x >= 0 and x < 8 and y >= 0 and y < 8);
		}
	};

}