#include "ResourceMusic.h"

ResourceMusic::ResourceMusic(string musicPath, string name)
{
	music = new sf::Music();
	if (music->openFromFile(musicPath)) {
		this->name = name;
		path = musicPath;
		resourceType = musicResource;
		uid = ++uid;
	}
	else {
		LOG_ERROR("Cannot load Music %s. Is the path correct?", name.c_str());
	}
}

ResourceMusic::~ResourceMusic()
{
	delete music;
}

sf::Music * ResourceMusic::GetMusic() const
{
	return music;
}
