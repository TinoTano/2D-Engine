#pragma once
#include "Component.h"
#include "ResourceSound.h"
#include "ResourceMusic.h"
#include <map>

class ComponentAudio : public Component
{
public:
	ComponentAudio(GameObject* attachedObject);
	virtual ~ComponentAudio();

	void OnEnable();
	void OnDisable();

	void Save(Data& data) const;
	void Load(Data& data);

	void PlaySound(string soundName);
	void StopSound(string soundName);
	void PauseSound(string soundName);
	void SetSoundLoop(string soundName, bool loop);
	bool IsSoundLoop(string soundName);
	string GetSoundState(string soundName);
	void SetSoundPitch(string soundName, float pitch);
	float GetSoundPitch(string soundName);
	void SetSoundVolume(string soundName, float volume);
	float GetSoundVolume(string soundName);
	void AddSound(ResourceSound* sound, int index);
	void RemoveSound(int index);
	bool SoundExist(string soundName);

	void PlayMusic(string musicName);
	void StopMusic(string musicName);
	void PauseMusic(string musicName);
	void SetMusicLoop(string musicName, bool loop);
	bool IsMusicLoop(string musicName);
	string GetMusicState(string musicName);
	void SetMusicPitch(string musicName, float pitch);
	float GetMusicPitch(string musicName);
	void SetMusicVolume(string musicName, float volume);
	float GetMusicVolume(string musicName);
	void AddMusic(ResourceMusic* music, int index);
	void RemoveMusic(int index);
	bool MusicExist(string musicName);
	vector<ResourceSound*> GetSoundsList() const;
	vector<ResourceMusic*> GetMusicList() const;

private:
	map<string, ResourceSound*> soundsList;
	map<string, ResourceMusic*> musicList;
	vector<ResourceSound*> soundsListVector;
	vector<ResourceMusic*> musicListVector;
	
};

