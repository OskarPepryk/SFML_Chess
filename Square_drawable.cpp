#include "stdafx.h"

#include "Square_drawable.h"

using namespace Chess;

void Square_draw::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_shape, states);
	target.draw(m_highlight, states);
	target.draw(m_notation, states);
}

void Square_draw::initialize(const sf::FloatRect & board, const sf::Font& font, bool upsideDown)
{
	static sf::Color lightColor{ 232, 235, 239 };
	static sf::Color darkColor{ 125, 135, 150 };
	
	//Set notation text
	m_notation.setFont(font);
	m_notation.setString(identify());
	m_notation.setCharacterSize(12);
	m_notation.setFillColor(sf::Color::Black);
	//
	//Set appropiate color
	if (m_pos.row % 2 == 0)
	{
		if (m_pos.column % 2 == 0)
			m_originalColor = darkColor;
		else
			m_originalColor = lightColor;
	}
	else
	{
		if (m_pos.column % 2 != 0)
			m_originalColor = darkColor;
		else
			m_originalColor = lightColor;
	}
	m_shape.setFillColor(m_originalColor);
	m_highlight.setFillColor(sf::Color::Transparent);

	sf::Vector2f size{ board.width / 8.0f, board.height / 8.0f };
	//Set size, assuming board is 8 square wide
	m_shape.setSize(size);
	m_highlight.setSize(size);
	//Set orgin to center
	m_shape.setOrigin(m_shape.getSize().x / 2.0f, m_shape.getSize().y / 2.0f);
	m_highlight.setOrigin(m_highlight.getSize().x / 2.0f, m_highlight.getSize().y / 2.0f);
	//Position the square
	sf::Vector2f position;
	if (!upsideDown)
	{
		position.x = board.left + board.width * m_pos.column / 8.0f + m_shape.getOrigin().x;
		position.y = board.top - board.height * m_pos.row / 8.0f + board.height - m_shape.getOrigin().y;
	}
	else
	{
		position.x = board.left + board.width * (7 - m_pos.column) / 8.0f + m_shape.getOrigin().x;
		position.y = board.top - board.height *(7 - m_pos.row) / 8.0f + board.height - m_shape.getOrigin().y;
	}
	m_shape.setPosition(position);
	m_highlight.setPosition(position);

	//Position the text
	m_notation.setPosition(position.x - m_shape.getOrigin().x, position.y - m_shape.getOrigin().y);
}


bool Square_draw::checkInBounds(const sf::Vector2f & worldCoords)
{
	return m_shape.getGlobalBounds().contains(worldCoords);
}

std::string Square_draw::identify() const
{
	std::string name;
	//lowercase letter for column
	name += static_cast<char>(m_pos.column + 97);
	//number for row
	name += static_cast<char>(m_pos.row + 49);
	return name;
}

void Square_draw::highlight(sf::Color color)
{
	color.a /= 2;
	m_highlight.setFillColor(color);
}

void Chess::Square_draw::setPosition(const sf::FloatRect & board, bool upsideDown)
{
	//static sf::Color lightColor{ 232, 235, 239 };
	//static sf::Color darkColor{ 125, 135, 150 };

	////Set notation text
	//m_notation.setFont(font);
	//m_notation.setString(identify());
	//m_notation.setCharacterSize(10);
	//m_notation.setFillColor(sf::Color::Black);
	////
	////Set appropiate color
	//if (m_pos.row % 2 == 0)
	//{
	//	if (m_pos.column % 2 == 0)
	//		m_originalColor = darkColor;
	//	else
	//		m_originalColor = lightColor;
	//}
	//else
	//{
	//	if (m_pos.column % 2 != 0)
	//		m_originalColor = darkColor;
	//	else
	//		m_originalColor = lightColor;
	//}
	//m_shape.setFillColor(m_originalColor);
	//m_highlight.setFillColor(sf::Color::Transparent);

	//sf::Vector2f size{ board.width / 8.0f, board.height / 8.0f };
	////Set size, assuming board is 8 square wide
	//m_shape.setSize(size);
	//m_highlight.setSize(size);
	////Set orgin to center
	//m_shape.setOrigin(m_shape.getSize().x / 2.0f, m_shape.getSize().y / 2.0f);
	//m_highlight.setOrigin(m_highlight.getSize().x / 2.0f, m_highlight.getSize().y / 2.0f);

	//Position the square
	sf::Vector2f position;
	if (!upsideDown)
	{
		position.x = board.left + board.width * m_pos.column / 8.0f + m_shape.getOrigin().x;
		position.y = board.top - board.height * m_pos.row / 8.0f + board.height - m_shape.getOrigin().y;
	}
	else
	{
		position.x = board.left + board.width * (7 - m_pos.column) / 8.0f + m_shape.getOrigin().x;
		position.y = board.top - board.height *(7 - m_pos.row) / 8.0f + board.height - m_shape.getOrigin().y;
	}
	m_shape.setPosition(position);
	m_highlight.setPosition(position);

	//Position the text
	m_notation.setPosition(position.x - m_shape.getOrigin().x, position.y - m_shape.getOrigin().y);
}
