#include "ResourceSprite.h"

ResourceSprite::ResourceSprite(string spritePath, string name)
{
	this->sprite = sprite;
	texture = new sf::Texture();
	if (texture->loadFromFile(spritePath)) {
		texture->setSmooth(true);
		sprite = new sf::Sprite();
		sprite->setTexture(*texture, true);
		this->name = name;
		resourceType = spriteResource;
		path = spritePath;
		uid = ++uid;
	}
	else {
		LOG_ERROR("Cannot load Sprite %s. Is the path correct?", name.c_str());
	}
	
}

ResourceSprite::~ResourceSprite()
{
	delete sprite;
	delete texture;
}

sf::Sprite* ResourceSprite::GetSprite() const
{
	return sprite;
}
