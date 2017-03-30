#include "ModuleSceneWindow.h"



ModuleSceneWindow::ModuleSceneWindow()
{
	moduleName = windowName = "2D Engine";
	windowSize = { 400, 400 };
	
}

ModuleSceneWindow::ModuleSceneWindow(sf::Vector2i size, string name)
{
	moduleName = windowName = "2D Engine";
	windowSize = size;
}


ModuleSceneWindow::~ModuleSceneWindow()
{
}

bool ModuleSceneWindow::Awake()
{
	//window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), windowName, sf::Style::None);
	//window->setFramerateLimit(60);
	window = new sf::RenderTexture();
	if (!window->create(windowSize.x, windowSize.y))
		return -1;
	return true;
}

bool ModuleSceneWindow::PreUpdate()
{
	//window->clear(sf::Color::White);
	return true;
}

bool ModuleSceneWindow::PostUpdate()
{
	window->clear(sf::Color::White);
	for (int i = 0; i < drawableObjects.size(); i++) {
		window->draw(*(drawableObjects[i]));
	}
	window->display();
	drawableObjects.clear();

	return true;
}

bool ModuleSceneWindow::CleanUp()
{
	//window->close();

	return true;
}
