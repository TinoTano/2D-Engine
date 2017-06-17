#pragma once
#include "Resource.h"
#include <SFML\Graphics.hpp>

class ResourceSprite : public Resource
{
public:
	ResourceSprite(string spritePath, string name);
	virtual ~ResourceSprite();

	sf::Sprite* GetSprite() const;

private:
	sf::Sprite* sprite = nullptr;
	sf::Texture* texture = nullptr;
};

