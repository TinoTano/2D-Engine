#include "ResourceSound.h"

ResourceSound::ResourceSound(string soundPath, string name)
{
	buffer = new sf::SoundBuffer();
	sound = new sf::Sound();
	if (buffer->loadFromFile(soundPath)) {
		sound->setBuffer(*buffer);
		this->name = name;
		path = soundPath;
		resourceType = soundResource;
		uid = ++uid;
	}
	else {
		LOG_ERROR("Cannot load Sound %s. Is the path correct?", name.c_str());
	}
}

ResourceSound::~ResourceSound()
{
	delete sound;
	delete buffer;
}

sf::Sound * ResourceSound::GetSound() const
{
	return sound;
}
