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

bool ModuleGraphics::Update(float deltaTime)
{
	for (list<sf::Drawable*>::iterator it = drawableObjectsList.begin(); it != drawableObjectsList.end(); it++) {
		engine->sceneWindow->drawableObjects.push_back(*it);
	}
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
	drawableObjectsList.push_back(circle);
	return circle;
}
