//Chess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Game_drawable.h"


int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window;
	window.create(sf::VideoMode(800,600), "Chess", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	//window.setActive(false);

	using namespace Chess;

	Game_drawable game;

	game.populateBoard();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				//rozmiarOkna(event.size.width, event.size.height);
				break;
			case sf::Event::MouseButtonPressed:
			{
				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
				case sf::Mouse::Right:
				{
					game.onMouseClick(event.mouseButton, window);
				}
				}
			}
			case sf::Event::KeyPressed:
			{
				switch (event.key.code)
				{
					//Show squares attacked by white
				case sf::Keyboard::W:
					game.unhighlightAll();
					game.highlight(game.getAttackedSquares(Side::White), sf::Color::Red);
					break;
					//Show squares attacked by black
				case sf::Keyboard::B:
					game.unhighlightAll();
					game.highlight(game.getAttackedSquares(Side::Black), sf::Color::Red);
					break;
				}
			}
			}
		}

		window.clear(sf::Color::Black);

		window.draw(game);

		window.display();
	}

    return 0;
}

