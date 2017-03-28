#pragma once

#include "Module.h"

class ModuleRenderWindow : public Module
{
public:
	ModuleRenderWindow();
	virtual ~ModuleRenderWindow();

	bool Awake();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

public:
	sf::RenderWindow* window;
	deque<sf::Drawable*> drawableObjects;
};

