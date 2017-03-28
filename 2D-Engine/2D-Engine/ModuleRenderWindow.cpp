#include "ModuleRenderWindow.h"

#include "imgui-1.49\imgui.h"
#include "imgui-1.49\imgui-SFML.h"


ModuleRenderWindow::ModuleRenderWindow()
{
	moduleName = "Render Window";
	window = nullptr;
}


ModuleRenderWindow::~ModuleRenderWindow()
{
}

bool ModuleRenderWindow::Awake()
{
	//window = new sf::RenderWindow(sf::VideoMode().getDesktopMode(), "2D Engine");
	window = new sf::RenderWindow(sf::VideoMode(600,800), "2D Engine");
	window->setFramerateLimit(60);
	return true;
}

bool ModuleRenderWindow::PreUpdate()
{
	window->clear(sf::Color::Black);
	return true;
}

bool ModuleRenderWindow::PostUpdate()
{
	for (int i = 0; i < drawableObjects.size(); i++) {
		window->draw(*(drawableObjects[i]));
	}
	ImGui::Render();
	window->display();
	drawableObjects.clear();

	return true;
}

bool ModuleRenderWindow::CleanUp()
{
	window->close();

	return true;
}
