#include "ComponentAudio.h"


ComponentAudio::ComponentAudio(GameObject * attachedObject)
{
	gameObject = attachedObject;
	type = AudioSource;
	name = "AudioSource";
}

ComponentAudio::~ComponentAudio()
{
}

void ComponentAudio::OnEnable()
{
}

void ComponentAudio::OnDisable()
{
}

void ComponentAudio::Save(Data & data) const
{
}

void ComponentAudio::Load(Data & data)
{
}

////////// Sounds //////////

void ComponentAudio::PlaySound(string soundName)
{
	soundsList[soundName]->GetSound()->play();
}

void ComponentAudio::StopSound(string soundName)
{
	soundsList[soundName]->GetSound()->stop();
}

void ComponentAudio::PauseSound(string soundName)
{
	soundsList[soundName]->GetSound()->pause();
}

void ComponentAudio::SetSoundLoop(string soundName, bool loop)
{
	soundsList[soundName]->GetSound()->setLoop(loop);
}

bool ComponentAudio::IsSoundLoop(string soundName)
{
	return soundsList[soundName]->GetSound()->getLoop();
}

string ComponentAudio::GetSoundState(string soundName)
{
	string status;
	switch (soundsList[soundName]->GetSound()->getStatus()) {
	case sf::SoundSource::Status::Playing:
		status = "Playing";
		break;
	case sf::SoundSource::Status::Paused:
		status = "Paused";
		break;
	case sf::SoundSource::Status::Stopped:
		status = "Stopped";
		break;
	}
	return status;
}

void ComponentAudio::SetSoundPitch(string soundName, float pitch)
{
	soundsList[soundName]->GetSound()->setPitch(pitch);
}

float ComponentAudio::GetSoundPitch(string soundName)
{
	return soundsList[soundName]->GetSound()->getPitch();
}

void ComponentAudio::SetSoundVolume(string soundName, float volume)
{
	soundsList[soundName]->GetSound()->setVolume(volume);
}

float ComponentAudio::GetSoundVolume(string soundName)
{
	return soundsList[soundName]->GetSound()->getVolume();
}

void ComponentAudio::AddSound(ResourceSound * sound, int index)
{
	if (index == -1) {
		string name = sound->GetName();
		soundsList[name] = sound;
		if (find(soundsListVector.begin(), soundsListVector.end(), sound) == soundsListVector.end()) {
			soundsListVector.push_back(sound);
		}
	}
	else {
		if (soundsList.find(sound->GetName()) == soundsList.end()) {
			map<string, ResourceSound*>::iterator it;
			int i = 0;
			for (map<string, ResourceSound*>::iterator it2 = soundsList.begin(); it2 != soundsList.end(); it2++) {
				if (i == index) {
					it = it2;
					break;
				}
				i++;
			}
			soundsList.erase(it);
			string name = sound->GetName();
			soundsList[name] = sound;
			soundsListVector[index] = sound;
		}
	}
}

void ComponentAudio::RemoveSound(int index)
{
	string soundName = soundsListVector[index]->GetName();
	if (index < soundsListVector.size()) {
		soundsListVector.erase(soundsListVector.begin() + index);
	}
	soundsList.erase(soundName);
}

bool ComponentAudio::SoundExist(string soundName)
{
	return soundsList.find(soundName) != soundsList.end();
}


////////// Music //////////

void ComponentAudio::PlayMusic(string musicName)
{
	musicList[musicName]->GetMusic()->play();
}

void ComponentAudio::StopMusic(string musicName)
{
	musicList[musicName]->GetMusic()->stop();
}

void ComponentAudio::PauseMusic(string musicName)
{
	musicList[musicName]->GetMusic()->pause();
}

void ComponentAudio::SetMusicLoop(string musicName, bool loop)
{
	musicList[musicName]->GetMusic()->setLoop(loop);
}

bool ComponentAudio::IsMusicLoop(string musicName)
{
	return musicList[musicName]->GetMusic()->getLoop();
}

string ComponentAudio::GetMusicState(string musicName)
{
	string status;
	switch (musicList[musicName]->GetMusic()->getStatus()) {
	case sf::Music::Status::Playing:
		status = "Playing";
		break;
	case sf::Music::Status::Paused:
		status = "Paused";
		break;
	case sf::Music::Status::Stopped:
		status = "Stopped";
		break;
	}
	return status;
}

void ComponentAudio::SetMusicPitch(string musicName, float pitch)
{
	musicList[musicName]->GetMusic()->setPitch(pitch);
}

float ComponentAudio::GetMusicPitch(string musicName)
{
	return musicList[musicName]->GetMusic()->getPitch();
}

void ComponentAudio::SetMusicVolume(string musicName, float volume)
{
	musicList[musicName]->GetMusic()->setVolume(volume);
}

float ComponentAudio::GetMusicVolume(string musicName)
{
	return musicList[musicName]->GetMusic()->getVolume();
}

void ComponentAudio::AddMusic(ResourceMusic * music, int index)
{
	if (index == -1) {
		string name = music->GetName();
		musicList[name] = music;
		if (find(musicListVector.begin(), musicListVector.end(), music) == musicListVector.end()) {
			musicListVector.push_back(music);
		}
	}
	else {
		if (musicList.find(music->GetName()) == musicList.end()) {
			map<string, ResourceMusic*>::iterator it;
			int i = 0;
			for (map<string, ResourceMusic*>::iterator it2 = musicList.begin(); it2 != musicList.end(); it2++) {
				if (i == index) {
					it = it2;
					break;
				}
				i++;
			}
			musicList.erase(it);
			string name = music->GetName();
			musicList[name] = music;
			musicListVector[index] = music;
		}
	}
}

void ComponentAudio::RemoveMusic(int index)
{
	string musicName = musicListVector[index]->GetName();
	if (index < musicListVector.size()) {
		musicListVector.erase(musicListVector.begin() + index);
	}

	musicList.erase(musicName);
}

bool ComponentAudio::MusicExist(string musicName)
{
	return musicList.find(musicName) != musicList.end();
}

vector<ResourceSound*> ComponentAudio::GetSoundsList() const
{
	return soundsListVector;
}

vector<ResourceMusic*> ComponentAudio::GetMusicList() const
{
	return musicListVector;
}

