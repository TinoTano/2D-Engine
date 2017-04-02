#include "ComponentSpriteRenderer.h"
#include "ModuleGraphics.h"
#include "Engine.h"
#include "ModuleSceneWindow.h"


ComponentSpriteRenderer::ComponentSpriteRenderer(GameObject* attachedObject)
{
	gameObjectSprite = new sf::Sprite();
	spriteTexture = new sf::Texture();
	gameObject = attachedObject;
	if (spriteTexture->loadFromFile("C:/Users/admin/Documents/FreedomTGC/Card Template/EpicIcon.png")) {
		spriteTexture->setSmooth(true);
		gameObjectSprite->setTexture(*spriteTexture,true);
		gameObjectSprite->setPosition(engine->sceneWindow->window->getSize().x / 2, engine->sceneWindow->window->getSize().y / 2);
		gameObjectSprite->setScale(1, -1);
	}
	type = SpriteRenderer;
	gameObject->gameObjectSprite = gameObjectSprite;
	name = "SpriteRenderer";
}


ComponentSpriteRenderer::~ComponentSpriteRenderer()
{
}
