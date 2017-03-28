#pragma once

#include "Module.h"

class ModuleRenderWindow;
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

public:
	ModuleRenderWindow* renderWindowModule = nullptr;
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
};

extern Engine* engine;
