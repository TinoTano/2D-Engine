#include "Engine.h"
#include "ModuleEngineWindow.h"
#include "ModuleSceneWindow.h"
#include "ModuleInput.h"
#include "ModuleGraphics.h"
#include "ModuleEditor.h"
#include "ModuleSceneManager.h"
#include "ModuleResources.h"


Engine::Engine()
{
	modulesList.push_back(engineWindow = new ModuleEngineWindow());
	modulesList.push_back(resourcesModule = new ModuleResources());
	modulesList.push_back(inputModule = new ModuleInput());
	modulesList.push_back(sceneWindow = new ModuleSceneWindow());
	modulesList.push_back(editorModule = new ModuleEditor());
	modulesList.push_back(sceneManagerModule = new ModuleSceneManager());
	//modulesList.push_back(graphicsModule = new ModuleGraphics());
}


Engine::~Engine()
{
	for (list<Module*>::iterator it = modulesList.begin(); it != modulesList.end(); it++) {
		RELEASE(*it);
	}

	modulesList.clear();
}

bool Engine::Awake()
{
	bool ret = false;
	for (list<Module*>::iterator it = modulesList.begin(); it != modulesList.end(); it++) {
		ret = (*it)->Awake();
	}
	return ret;
}

bool Engine::Start()
{
	bool ret = false;
	for (list<Module*>::iterator it = modulesList.begin(); it != modulesList.end(); it++) {
		ret = (*it)->Start();
	}
	return ret;
}

bool Engine::DoUpdate()
{
	bool ret = true;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = Update();

	if (ret == true)
		ret = PostUpdate();

	if (quit) ret = false;

	return ret;
}

bool Engine::PostUpdate()
{
	bool ret = true;
	for (list<Module*>::iterator it = modulesList.begin(); it != modulesList.end(); it++)
	{
		if ((*it)->enable)
			ret = (*it)->PostUpdate();
	}

	return ret;
}

void Engine::Play()
{
	if (state == OnStop) {
		state = OnPlay;
	}
}

void Engine::Pause()
{
	if (state == OnPlay) {
		state = OnPause;
	}
}

void Engine::UnPause()
{
	if (state == OnPause) {
		state = OnPlay;
	}
}

void Engine::Stop()
{
	if (state == OnPlay || state == OnPause) {
		state = OnStop;
	}
}

bool Engine::IsPlaying()
{
	return state == OnPlay;
}

bool Engine::IsPaused()
{
	return state == OnPause;
}

bool Engine::IsStopped()
{
	return state == OnStop;
}

void Engine::QuitEngine()
{
	quit = true;
}

float Engine::GetFPS() const
{
	return FPS;
}

bool Engine::CleanUp()
{
	bool ret = true;

	for (list<Module*>::reverse_iterator it = modulesList.rbegin(); it != modulesList.rend() && ret; ++it)
		if ((*it)->enable)
			ret = (*it)->CleanUp();

	return ret;
}

bool Engine::PreUpdate()
{
	time = clock.restart();
	deltaTime = time.asSeconds();
	FPS = 1000000.0f / time.asMicroseconds();

	bool ret = true;
	for (list<Module*>::iterator it = modulesList.begin(); it != modulesList.end(); it++)
	{
		if ((*it)->enable)
			ret = (*it)->PreUpdate();
	}

	return ret;
}

bool Engine::Update()
{
	bool ret = true;
	for (list<Module*>::iterator it = modulesList.begin(); it != modulesList.end(); it++)
	{
		if ((*it)->enable)
			ret = (*it)->Update(deltaTime);
	}

	return ret;
}

