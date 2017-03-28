#pragma once

#include "Module.h"
#include <map>

enum KEY_STATE {
	KEY_IDLE, KEY_PRESSED, KEY_REPEATED, KEY_RELEASED
};

class ModuleInput : public Module
{
public:
	ModuleInput();
	virtual ~ModuleInput();

	bool PreUpdate();
	bool CleanUp();

	bool IsKeyPressed(sf::Keyboard::Key key);
	bool IsKeyRepeated(sf::Keyboard::Key key);
	bool IsKeyReleased(sf::Keyboard::Key key);

	bool IsMouseButtonPressed(sf::Mouse::Button button);
	bool IsMouseButtonRepeated(sf::Mouse::Button button);
	bool IsMouseButtonReleased(sf::Mouse::Button button);
	sf::Vector2i GetmousePosition() const;

private:
	KEY_STATE keyState = KEY_IDLE;
	map<sf::Keyboard::Key, KEY_STATE> keyboardKeys;
	map<sf::Mouse::Button, KEY_STATE> mouseBbuttons;
	sf::Vector2i mousePosition;
	bool isMouseInsideWindow = false;
	float windowWidth;
	float windowHeight;
};

