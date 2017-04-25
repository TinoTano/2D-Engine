#include "ModuleSceneWindow.h"
#include "ComponentTransform.h"
#include "ComponentSpriteRenderer.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "ModuleEngineWindow.h"


ModuleSceneWindow::ModuleSceneWindow()
{
	moduleName = windowName = "Game Scene";
	windowSize = { 400, 400 };
	
}

ModuleSceneWindow::ModuleSceneWindow(sf::Vector2i size)
{
	moduleName = windowName = "Game Scene";
	windowSize = size;
}


ModuleSceneWindow::~ModuleSceneWindow()
{
}

bool ModuleSceneWindow::Awake()
{
	window = new sf::RenderTexture();
	Data data;
	if (data.LoadBinary("../Data/Scene_Window")){
		data.EnterSection("Scene_Window");
		windowSize = data.GetVector2Int("SceneSize");
		data.LeaveSection();
	}
	if (!window->create(windowSize.x, windowSize.y))
		return -1;

	return true;
}

bool ModuleSceneWindow::PreUpdate()
{
	if (engine->inputModule->IsKeyRepeated(sf::Keyboard::A)) {
		sf::View view = window->getView();
		view.zoom(1.1f);
		window->setView(view);
	}
	else if (engine->inputModule->IsKeyRepeated(sf::Keyboard::S)) {
		sf::View view = window->getView();
		view.zoom(0.9f);
		window->setView(view);
	}
	return true;
}

bool ModuleSceneWindow::PostUpdate()
{
	window->clear(sf::Color(100, 100, 100, 255));
	for (int i = 0; i < drawableObjects.size(); i++) {
		if (drawableObjects[i]->isActive()) {
			window->draw(*drawableObjects[i]->gameObjectSprite);
		}
	}
	window->display();

	return true;
}

bool ModuleSceneWindow::CleanUp()
{
	Data data;
	data.CreateSection("Scene_Window");
	data.AddVector2Int("SceneSize", (sf::Vector2i)window->getSize());
	data.CloseSection();
	data.SaveAsBinary("../Data/Scene_Window");

	delete window;

	return true;
}

void ModuleSceneWindow::Resize(uint width, uint height)
{
	
	sf::View view = window->getView();
	//if (!window->create(width, height)) {
	//	return;
	//}
	//
	//view.setCenter(width / 2, height / 2);
	view.zoom(1.01f);
	//view.setSize(width, height);
	window->setView(view);
	window->clear(sf::Color(100, 100, 100, 255));
	
}

