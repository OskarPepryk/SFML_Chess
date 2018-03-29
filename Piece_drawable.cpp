#include "stdafx.h"
#include "Piece.h"
#include "Game_drawable.h"

using namespace Chess;

void Piece_draw::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (m_isDead or !m_takenSquare)
		return;
	target.draw(m_sprite, states);
}

void Piece_draw::setTexture(Type type, Side side, const sf::Texture & texture)
{
	m_type = type;
	m_side = side;

	m_sprite.setTexture(texture);

	sf::IntRect textureRect;

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


	m_sprite.setTextureRect(sf::IntRect((texture.getSize().x / 6) * textureColumn,
		(texture.getSize().y / 2)* textureRow, 213, 213));

}

void Chess::Piece_draw::setTakenSquare(Square * newSquare)
{
	Piece::setTakenSquare(newSquare);
	
	Square_draw* newSquareDrawable = dynamic_cast<Square_draw*>(newSquare);

	if (newSquareDrawable)
		m_sprite.setPosition(newSquareDrawable->getShape().getPosition());
}
