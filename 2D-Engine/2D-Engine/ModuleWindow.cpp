#include "ModuleWindow.h"



ModuleWindow::ModuleWindow()
{
	moduleName = "Window";
	window = nullptr;
}


ModuleWindow::~ModuleWindow()
{
}

bool ModuleWindow::Start()
{
	window = new sf::Window(sf::VideoMode(800, 600), "New Window");
	window->setVerticalSyncEnabled(true);
	return true;
}

bool ModuleWindow::CleanUp()
{
	window->close();
	return true;
}

