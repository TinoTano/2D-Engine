#include "ComponentSpriteRenderer.h"
#include "ModuleGraphics.h"
#include "Engine.h"


ComponentSpriteRenderer::ComponentSpriteRenderer()
{
	gameObjectSprite = new sf::Sprite();
	sf::Texture texture;
	if (texture.create(50, 50)) {
		gameObjectSprite->setTexture(texture);
		engine->graphicsModule->drawableObjectsList.push_back(gameObjectSprite);
	}
}


ComponentSpriteRenderer::~ComponentSpriteRenderer()
{
}
