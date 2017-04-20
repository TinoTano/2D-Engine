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
	Engine();
	~Engine();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();
	bool PreUpdate();
	bool DoUpdate();
	bool PostUpdate();
	
	float GetFPS()const;

public:
	ModuleEngineWindow* engineWindow = nullptr;
	ModuleSceneWindow* sceneWindow = nullptr;
	ModuleInput* inputModule = nullptr;
	ModuleGraphics* graphicsModule = nullptr;
	ModuleAudio* audioModule = nullptr;
	ModuleEditor* editorModule = nullptr;
	ModuleSceneManager* sceneManagerModule = nullptr;

	bool quit = false;
	sf::Time time;

private:
	list<Module*> modulesList;
	sf::Clock clock;
	float deltaTime;
	float FPS;
};

extern Engine* engine;
