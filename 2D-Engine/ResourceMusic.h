#pragma once
#include "Resource.h"
#include <SFML\Audio\Music.hpp>

class ResourceMusic : public Resource
{
public:
	ResourceMusic(string musicPath, string name);
	virtual ~ResourceMusic();

	sf::Music* GetMusic() const;

private:
	sf::Music* music;
};

