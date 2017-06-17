#include "ComponentSpriteRenderer.h"
#include "ModuleGraphics.h"
#include "Engine.h"
#include "ModuleSceneWindow.h"
#include "ComponentTransform.h"


ComponentSpriteRenderer::ComponentSpriteRenderer(GameObject* attachedObject)
{
	gameObjectSprite = new sf::Sprite();
	spriteTexture = new sf::Texture();
	gameObject = attachedObject;
	spritePath = "../Assets/bird1.png";
	if (spriteTexture->loadFromFile(spritePath)) {
		spriteTexture->setSmooth(true);
		gameObjectSprite->setTexture(*spriteTexture,true);
		gameObjectSprite->setPosition((engine->sceneWindow->window->getSize().x / 2), (engine->sceneWindow->window->getSize().y / 2));
		gameObjectSprite->setOrigin(gameObjectSprite->getLocalBounds().width / 2, gameObjectSprite->getLocalBounds().height / 2);
		ComponentTransform* transform = (ComponentTransform*)gameObject->GetComponent(Transform);
		transform->SetInitialPosition(gameObjectSprite->getPosition());
	}
	type = SpriteRenderer;
	gameObject->gameObjectSprite = gameObjectSprite;
	name = "SpriteRenderer";
	isFlippedX = false;
	isFlippedY = false;
}


ComponentSpriteRenderer::~ComponentSpriteRenderer()
{
	delete gameObjectSprite;
	delete spriteTexture;
}

bool ComponentSpriteRenderer::ChangeSprite(string path)
{
	bool ret = false;
	if (spriteTexture->loadFromFile(path)) {
		spriteTexture->setSmooth(true);
		gameObjectSprite->setTexture(*spriteTexture, true);
		gameObjectSprite->setOrigin(gameObjectSprite->getLocalBounds().width / 2, gameObjectSprite->getLocalBounds().height / 2);
		gameObject->gameObjectSprite = gameObjectSprite;
		spritePath = path;
		ret = true;
	}
	return ret;
}

bool ComponentSpriteRenderer::ChangeSprite(sf::Sprite* sprite, string path)
{
	bool ret = false;
	if (sprite != nullptr) {
		gameObjectSprite->setTexture(*sprite->getTexture(),true);
		gameObject->gameObjectSprite = gameObjectSprite;
		spritePath = path;
		ret = true;
	}
	return ret;
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
