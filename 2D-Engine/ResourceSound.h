#pragma once
#include "Resource.h"
#include <SFML\Audio\SoundBuffer.hpp>
#include <SFML\Audio\Sound.hpp>

class ResourceSound : public Resource
{
public:
	ResourceSound(string soundPath, string name);
	virtual ~ResourceSound();

	sf::Sound* GetSound() const;

private:
	sf::SoundBuffer* buffer;
	sf::Sound* sound;

};

