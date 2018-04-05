#pragma once

#include <TGUI\TGUI.hpp>
#include "Game_drawable.h"

namespace Chess
{
	void createUndoButton(tgui::Gui & gui, Game_drawable & game)
	{
		tgui::Button::Ptr button = tgui::Button::create();
		button->setPosition(602, 0);
		button->setSize(198, 50);
		button->setText("Undo");
		button->connect("pressed", &Game_drawable::undo, &game);
		gui.add(button, "undo");
	}

	void createFlipButton(tgui::Gui & gui, Game_drawable & game)
	{
		tgui::Button::Ptr button = tgui::Button::create();
		button->setPosition(602, 50);
		button->setSize(198, 50);
		button->setText("Flip board");
		button->connect("pressed", &Game_drawable::flipBoard, &game);
		gui.add(button, "flip");
	}
}