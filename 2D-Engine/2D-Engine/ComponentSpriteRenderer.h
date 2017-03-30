#pragma once

#include "Component.h"

class ComponentSpriteRenderer : public Component
{
public:
	ComponentSpriteRenderer();
	virtual ~ComponentSpriteRenderer();

	sf::Sprite* gameObjectSprite;
};

