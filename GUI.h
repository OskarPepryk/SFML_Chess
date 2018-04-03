#pragma once

#include <TGUI\TGUI.hpp>


namespace Chess
{
	void createSplashScreen(tgui::Gui & gui)
	{
		tgui::Button::Ptr button = tgui::Button::create();
		button->setPosition(600, 300);
		gui.add(button, "MyButton");
	}
}