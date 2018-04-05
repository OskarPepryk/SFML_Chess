#pragma once

#include <memory>
#include <vector>

#include "Game.h"
#include "Square_drawable.h"
#include "Piece_drawable.h"
#include "ResourceManager.h"

namespace Chess
{
	class Game_drawable :
		public Game,
		virtual public sf::Drawable
	{
	private:
		sf::FloatRect			bounds{ 0, 0, 600, 600 };
		
		bool					upsideDown = false;

		PieceID					m_selectedPiece;
		
		std::vector<std::unique_ptr<Game>>	undos;

	public:
		const ResourceManager&	resources;

		Game_drawable() = delete;

		Game_drawable(const ResourceManager& resources, bool upsideDown);

		Game_drawable(const Game & game, const ResourceManager& resources, bool upsideDown = false);

		Game_drawable & operator=(const Game & other);



		void addPiece(Piece::Type type, Side side, Position square) override;

		void addPiece(Piece::Type type, Side side, int row, int column) override;

		//Piece_draw* pickUpPiece(Square_draw & from);

		//void movePiece(Square_draw & oldSquare, Square_draw & newSquare);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void unhighlightAll();

		std::shared_ptr<Square_draw> selectSquare(const sf::Vector2f &worldCoords);

		Position selectPosition(const sf::Vector2f & worldCoords);

		PieceID selectPiece(const sf::Vector2f &worldCoords);

		void onMouseClick(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);

		void onMouseMoved(int x, int y, const sf::RenderWindow & window);

		void highlight(const std::vector<Position> & list, sf::Color color);
		void highlight(const std::vector<PieceID> & list, sf::Color color);
		void highlight(const PieceID & id, sf::Color color);
		void highlight(const Position & pos, sf::Color color);

		void playGame(const sf::Event::MouseButtonEvent & event, const sf::RenderWindow & window);

		void animate();

		void switchActiveSide() override;

		void createUndo();
		void undo();

		void flipBoard()
		{
			upsideDown = !upsideDown;
			//Flip squares
			for (auto square : squares)
			{
				//Static cast, as square has to be Square_drawable if this is Game_drawable
				auto drawableSquare = std::static_pointer_cast<Square_draw>(square);
				drawableSquare->setPosition(bounds, upsideDown);
			}
			//Reposition the pieces
			for (auto piece : pieces)
			{
				auto drawablePiece = std::static_pointer_cast<Piece_draw>(piece);
				//Move piece to correct place without animation
				drawablePiece->teleport();
				//
			}

		}

	};
}

