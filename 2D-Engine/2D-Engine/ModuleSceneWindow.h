#pragma once

#include "Module.h"

class ModuleSceneWindow : public Module
{
public:
	ModuleSceneWindow();
	ModuleSceneWindow(sf::Vector2i size, string name);
	virtual ~ModuleSceneWindow();

	bool Awake();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

public:
	sf::RenderWindow* window;
	deque<sf::Drawable*> drawableObjects;
	sf::Vector2i windowSize;
	string windowName;
};

