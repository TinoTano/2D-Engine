#pragma once

#include "Component.h"
#include <SFML\Graphics.hpp>

class ComponentSpriteRenderer : public Component
{
public:
	ComponentSpriteRenderer(GameObject* attachedObject);
	virtual ~ComponentSpriteRenderer();

	void ChangeSprite(string path);

	void OnEnable();
	void OnDisable();

	void Save(Data& data) const;
	void Load(Data& data);

public:
	sf::Sprite* gameObjectSprite;
	sf::Texture* spriteTexture;
	string spritePath;
};

