#pragma once

#include "Module.h"

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	virtual ~ModuleAudio();

	bool CleanUp();

	sf::Sound* LoadSound(const char* path);
	sf::Music* LoadMusic(const char* path);

private:
	sf::SoundBuffer soundBuffer;
};

