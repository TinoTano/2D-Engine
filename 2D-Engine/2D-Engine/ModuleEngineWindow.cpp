#include "ModuleEngineWindow.h"
#include "Engine.h"
#include "ModuleSceneWindow.h"

ModuleEngineWindow::ModuleEngineWindow()
{
	moduleName = windowName = "2D Engine";
	window = nullptr;
}


ModuleEngineWindow::~ModuleEngineWindow()
{
}

bool ModuleEngineWindow::Awake()
{
	window = new sf::RenderWindow(sf::VideoMode(1600, 900), windowName);
	window->setFramerateLimit(60);
	return true;
}

bool ModuleEngineWindow::PreUpdate()
{
	
	return true;
}

bool ModuleEngineWindow::PostUpdate()
{
	window->clear(sf::Color::Blue);
	//window->draw((sf::Sprite)engine->sceneWindow->window->getTexture());
	
	ImGui::Render();
	window->display();

	return true;
}

bool ModuleEngineWindow::CleanUp()
{
	window->close();

	return true;
}
