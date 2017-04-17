#include "ComponentSpriteRenderer.h"
#include "ModuleGraphics.h"
#include "Engine.h"
#include "ModuleSceneWindow.h"


ComponentSpriteRenderer::ComponentSpriteRenderer(GameObject* attachedObject)
{
	gameObjectSprite = new sf::Sprite();
	spriteTexture = new sf::Texture();
	gameObject = attachedObject;
	spritePath = "C:/Users/admin/Documents/FreedomTGC/Card Template/EpicIcon.png";
	if (spriteTexture->loadFromFile(spritePath)) {
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
	delete gameObjectSprite;
	delete spriteTexture;
}

void ComponentSpriteRenderer::OnEnable()
{
}

void ComponentSpriteRenderer::OnDisable()
{
}

void ComponentSpriteRenderer::Save(Data & data) const
{
	data.AddInt("Type", type);
	data.AddString("Sprite Path", spritePath);
}

void ComponentSpriteRenderer::Load(Data & data)
{
	spritePath = data.GetString("Sprite Path");
}
