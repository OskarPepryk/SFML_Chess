#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>

namespace Chess
{
	class ResourceManager
	{
	private:
		sf::Font font_tahoma;
		sf::Texture texture;

	public:
		ResourceManager()
		{
			if (!(font_tahoma.loadFromFile("tahoma.ttf") and
				texture.loadFromFile("chess_sprites.png")))
				std::cout << "Failed to load resources in resource manager.\n";
		}

		const sf::Font& getFontTahoma() const
		{
			return font_tahoma;
		}

		const sf::Texture& getTexture() const
		{
			return texture;
		}
	};
}