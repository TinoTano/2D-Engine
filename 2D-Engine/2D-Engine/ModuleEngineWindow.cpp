#include "ModuleEngineWindow.h"


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
	window->clear(sf::Color::Blue);
	return true;
}

bool ModuleEngineWindow::PostUpdate()
{
	ImGui::Render();
	window->display();

	return true;
}

bool ModuleEngineWindow::CleanUp()
{
	window->close();

	return true;
}
