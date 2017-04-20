#include "ModuleEngineWindow.h"
#include "Engine.h"
#include "ModuleSceneWindow.h"

ModuleEngineWindow::ModuleEngineWindow()
{
	moduleName = "Engine Window";
	windowName = "2D Engine - Untitled Scene";
	window = nullptr;
}


ModuleEngineWindow::~ModuleEngineWindow()
{
	
}

bool ModuleEngineWindow::Awake()
{
	window = new sf::RenderWindow(sf::VideoMode(1600, 900), windowName);
	window->setVerticalSyncEnabled(true);
	return true;
}

bool ModuleEngineWindow::PreUpdate()
{
	
	return true;
}

bool ModuleEngineWindow::PostUpdate()
{
	window->clear(sf::Color::Blue);
	ImGui::Render();
	window->display();

	return true;
}

bool ModuleEngineWindow::CleanUp()
{
	window->close();
	delete window;

	return true;
}

void ModuleEngineWindow::SetSceneName(string name)
{
	window->setTitle("2D Engine - " + name);
}
