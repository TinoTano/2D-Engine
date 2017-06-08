#include "ModuleSceneWindow.h"
#include "ComponentTransform.h"
#include "ComponentSpriteRenderer.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "ModuleEngineWindow.h"
#include "ModuleSceneManager.h"
#include "ModuleEditor.h"


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
		zoomValue = data.GetFloat("ZoomValue");
		data.LeaveSection();
	}
	if (!window->create(windowSize.x, windowSize.y))
		return -1;

	sf::View view = window->getView();
	view.zoom(zoomValue);
	window->setView(view);
	return true;
}

bool ModuleSceneWindow::PreUpdate()
{
	if (engine->inputModule->IsKeyRepeated(sf::Keyboard::A)) {
		sf::View view = window->getView();
		zoomValue *= 1.1f;
		view.zoom(1.1f);
		window->setView(view);

	}
	else if (engine->inputModule->IsKeyRepeated(sf::Keyboard::S)) {
		sf::View view = window->getView();
		zoomValue *= 0.9f;
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
			if (engine->inputModule->IsMouseButtonReleased(sf::Mouse::Left)) {
				if (drawableObjects[i]->gameObjectSprite->getGlobalBounds().contains(sf::Vector2f(engine->inputModule->GetmousePosition().x - windowPos.x, engine->inputModule->GetmousePosition().y - windowPos.y))) {
					//engine->sceneManagerModule->selectedGameObjects.push_back(drawableObjects[i]);
				}
			}
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
	data.AddFloat("ZoomValue", zoomValue);
	data.CloseSection();
	data.SaveAsBinary("../Data/Scene_Window");

	delete window;

	return true;
}

void ModuleSceneWindow::Resize(uint width, uint height)
{
	
	//sf::View view = window->getView();
	if (!window->create(width, height)) {
		return;
	}
	////
	////view.setCenter(width / 2, height / 2);
	//view.zoom(1.01f);
	////view.setSize(width, height);
	//window->setView(view);
	window->clear(sf::Color(100, 100, 100, 255));
	
}

