#include "stdafx.h"

#include "Square.h"

using namespace Chess;

void Square::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_shape, states);
}

void Square::initialize(const sf::FloatRect & board)
{
	static sf::Color lightColor{ 232, 235, 239 };
	static sf::Color darkColor{ 125, 135, 150 };

	//Set appropiate color
	if (m_row % 2 == 0)
	{
		if (m_column % 2 == 0)
			m_originalColor = darkColor;
		else
			m_originalColor = lightColor;
	}
	else
	{
		if (m_column % 2 != 0)
			m_originalColor = darkColor;
		else
			m_originalColor = lightColor;
	}
	m_shape.setFillColor(m_originalColor);

	//Set size, assuming board is 8 square wide
	m_shape.setSize(sf::Vector2f(board.width / 8.0f, board.height / 8.0f));
	//Set orgin to bottom left corner
	m_shape.setOrigin(m_shape.getSize().x / 2.0f, m_shape.getSize().y / 2.0f);
	//Position the square
	sf::Vector2f position;
	position.x = board.left + board.width * m_column / 8.0f + m_shape.getOrigin().x;
	position.y = board.top - board.height * m_row / 8.0f + board.height - m_shape.getOrigin().y;
	m_shape.setPosition(position);
}


bool Square::checkInBounds(const sf::Vector2f & worldCoords)
{
	return m_shape.getGlobalBounds().contains(worldCoords);
}

std::string Square::identify() const
{
	std::string name;
	//lowercase letter for column
	name += static_cast<char>(m_column + 97);
	//number for row
	name += static_cast<char>(m_row + 49);
	return name;
}

void Square::highlight(bool highlight)
{
	if (highlight)
		m_shape.setFillColor(sf::Color(100, 200, 100));
	else
		m_shape.setFillColor(m_originalColor);
}
