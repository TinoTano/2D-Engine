#include "ModuleAudio.h"
#include "Engine.h"


ModuleAudio::ModuleAudio()
{
	moduleName = "Audio";
}


ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::CleanUp()
{
	return true;
}

sf::Sound* ModuleAudio::LoadSound(const char * path)
{
	if (!soundBuffer.loadFromFile(path)) {
		return nullptr;
	}
	sf::Sound* sound = new sf::Sound(soundBuffer);
	return sound;
}

sf::Music* ModuleAudio::LoadMusic(const char * path)
{
	sf::Music* music = new sf::Music();
	if (!music->openFromFile(path)) {
		return nullptr;
	}
	return music;
}
