#pragma once

#include <TGUI\TGUI.hpp>


namespace Chess
{
	void createSplashScreen(tgui::Gui & gui)
	{
		tgui::Button::Ptr button = tgui::Button::create();
		button->setPosition(600, 300);
		button->setSize(200, 100);
		button->setText("Undo");
		gui.add(button, "MyButton");
	}
}