#include "ModuleGraphics.h"
#include "Engine.h"
#include "ModuleSceneWindow.h"


ModuleGraphics::ModuleGraphics()
{
	moduleName = "Graphics";
}


ModuleGraphics::~ModuleGraphics()
{
}

bool ModuleGraphics::Start() {
	return true;
}

bool ModuleGraphics::Update(float deltaTime)
{
	return true;
}

bool ModuleGraphics::CleanUp()
{
	drawableObjectsList.clear();
	return true;
}

sf::Texture ModuleGraphics::LoadTexture(const char * path)
{
	return sf::Texture();
}

sf::CircleShape* ModuleGraphics::CreateShape()
{
	sf::CircleShape* circle = new sf::CircleShape(50);
	circle->setFillColor(sf::Color::Green);
	circle->setPosition(50, 50);
	drawableObjectsList.push_back(circle);
	return circle;
}
