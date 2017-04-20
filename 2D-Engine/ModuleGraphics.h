#pragma once

#include "Module.h"

class ModuleGraphics : public Module
{
public:
	ModuleGraphics();
	virtual ~ModuleGraphics();

	bool Start();
	bool Update(float deltaTime);
	bool CleanUp();

	sf::Texture LoadTexture(const char* path);
	sf::CircleShape* CreateShape();

public:
	list<sf::Drawable*> drawableObjectsList;
};

