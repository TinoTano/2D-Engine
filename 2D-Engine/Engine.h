#pragma once

#include "Module.h"

class ModuleEngineWindow;
class ModuleSceneWindow;
class ModuleInput;
class ModuleGraphics;
class ModuleAudio;
class ModuleEditor;
class ModuleSceneManager;

class Engine
{
public:

	enum EngineState {
		OnPlay, OnPause, OnStop
	};

	Engine();
	~Engine();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();
	bool PreUpdate();
	bool DoUpdate();
	bool PostUpdate();

	void Play();
	void Pause();
	void UnPause();
	void Stop();
	bool IsPlaying();
	bool IsPaused();
	bool IsStopped();

	void QuitEngine();
	
	float GetFPS()const;

public:
	ModuleEngineWindow* engineWindow = nullptr;
	ModuleSceneWindow* sceneWindow = nullptr;
	ModuleInput* inputModule = nullptr;
	ModuleGraphics* graphicsModule = nullptr;
	ModuleAudio* audioModule = nullptr;
	ModuleEditor* editorModule = nullptr;
	ModuleSceneManager* sceneManagerModule = nullptr;
	sf::Time time;

private:
	list<Module*> modulesList;
	sf::Clock clock;
	float deltaTime;
	float FPS;
	EngineState state = EngineState::OnStop;
	bool quit = false;
};

extern Engine* engine;
