#include "stdafx.h"
#include "Piece.h"
#include "Game_drawable.h"

using namespace Chess;

void Piece_draw::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (m_isDead or !m_pos.valid())
		return;
	target.draw(m_sprite, states);
}

void Piece_draw::setTexture(const sf::Texture & texture)
{
	m_sprite.setTexture(texture);

	setTexture();
}

void Piece_draw::setTexture()
{
	int textureColumn;
	int textureRow;

	switch (m_type)
	{
	case Type::King:
		textureColumn = 0;
		break;
	case Type::Queen:
		textureColumn = 1;
		break;
	case Type::Bishop:
		textureColumn = 2;
		break;
	case Type::Knight:
		textureColumn = 3;
		break;
	case Type::Rook:
		textureColumn = 4;
		break;
	case Type::Pawn:
		textureColumn = 5;
		break;
	default:
		textureColumn = 0;
		break;
	}

	if (m_side == Side::White)
		textureRow = 0;
	else
		textureRow = 1;

	//m_sprite.setTextureRect(sf::IntRect((texture.getSize().x / 6) * textureColumn,
	//	(texture.getSize().y / 2)* textureRow, 213, 213));

	m_sprite.setTextureRect(sf::IntRect(static_cast<int>(213.333f * textureColumn),
		static_cast<int>(213.5f * textureRow), 213, 213));
}

void Chess::Piece_draw::setTakenSquare(Position &position)
{
	Piece::setTakenSquare(position);
	
	Square_draw* newSquareDrawable = dynamic_cast<Square_draw*>(&m_parentGame.get().getSquares().at(position));

	if (newSquareDrawable)
		m_sprite.setPosition(newSquareDrawable->getShape().getPosition());
}
