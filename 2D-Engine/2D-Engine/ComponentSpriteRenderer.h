#pragma once

#include "Component.h"

class ComponentSpriteRenderer : public Component
{
public:
	ComponentSpriteRenderer(GameObject* attachedObject);
	virtual ~ComponentSpriteRenderer();

	sf::Sprite* gameObjectSprite;
	sf::Texture* spriteTexture;
};

