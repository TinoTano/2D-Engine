#pragma once

#include "Component.h"

class ComponentSpriteRenderer : public Component
{
public:
	ComponentSpriteRenderer(GameObject* attachedObject);
	virtual ~ComponentSpriteRenderer();

	void OnEnable();
	void OnDisable();

	void Save(Data& data) const;
	void Load(Data& data);

public:
	sf::Sprite* gameObjectSprite;
	sf::Texture* spriteTexture;
	string spritePath;
};

