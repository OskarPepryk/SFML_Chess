#include "stdafx.h"
#include "Piece.h"
#include "Game_drawable.h"

using namespace Chess;

Piece_draw::Piece_draw(Type type, Side side, int id, Game & parent, const sf::Texture &texture) : Piece{ type, side, id, parent }
{
	setTexture(texture);
	m_sprite.setScale(0.3f, 0.3f);
	auto size = m_sprite.getLocalBounds();
	m_sprite.setOrigin(size.width / 2, size.height / 2);
}

Piece_draw::Piece_draw(const Piece & piece, Game & parent, const sf::Texture &texture) : Piece{ piece, parent }
{
	setTexture(texture);
	m_sprite.setScale(0.3f, 0.3f);
	auto size = m_sprite.getLocalBounds();
	m_sprite.setOrigin(size.width / 2, size.height / 2);

	//Reposition the sprite
	auto newSquareDrawable = std::dynamic_pointer_cast<Square_draw>(m_parentGame.get().getSquares().at(m_pos));

	if (newSquareDrawable)
	{
		m_targetPos = newSquareDrawable->getShape().getPosition();
		m_sprite.setPosition(m_targetPos);
	}
}

void Piece_draw::animate()
{
	//Animate the sprite to new position
	sf::Vector2f moveVector{ (m_targetPos - m_sprite.getPosition()) / 20.0f };
	m_sprite.move(moveVector);
}

void Piece_draw::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (m_isDead or !m_pos.valid())
		return;

	target.draw(m_sprite, states);
}

void Piece_draw::setTexture(const sf::Texture & texture)
{
	m_sprite.setTexture(texture);

	setTextureRect();
}

void Piece_draw::setTextureRect()
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

void Chess::Piece_draw::setTakenSquare(Position &position, bool isMove)
{
	Piece::setTakenSquare(position);
	
	auto newSquareDrawable = std::static_pointer_cast<Square_draw>(m_parentGame.get().getSquares().at(position));

	if (newSquareDrawable)
		m_targetPos = newSquareDrawable->getShape().getPosition();
}

void Chess::Piece_draw::promote(Type type)
{
	Piece::promote(type);
	setTextureRect();
}

void Chess::Piece_draw::teleport(Position & position, bool isMove)
{
	Piece::setTakenSquare(position, isMove);

	auto newSquareDrawable = std::static_pointer_cast<Square_draw>(m_parentGame.get().getSquares().at(position));

	if (newSquareDrawable)
	{
		auto & worldPos = newSquareDrawable->getShape().getPosition();
		m_targetPos = worldPos;
		m_sprite.setPosition(worldPos);
	}
}

void Chess::Piece_draw::teleport()
{
	teleport(m_pos, false);
}

void Chess::Piece_draw::fade(bool fade)
{
	if (fade)
	{
		m_sprite.setColor(sf::Color{ 255,255,255,180 });
	}
	else
	{
		m_sprite.setColor(sf::Color{ 255,255,255,255 });
	}
}
