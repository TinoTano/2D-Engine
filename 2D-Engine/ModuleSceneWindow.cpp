#include "ModuleSceneWindow.h"
#include "ComponentTransform.h"
#include "ComponentSpriteRenderer.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "ModuleEngineWindow.h"
#include "ModuleSceneManager.h"
#include "ModuleEditor.h"
#include "ComponentAnimation.h"


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
	window->setSmooth(true);
	return true;
}

bool ModuleSceneWindow::PreUpdate()
{
	/*if (engine->inputModule->IsMouseWheelScrollingUp()) {
		sf::View view = window->getView();
		view.setSize(view.getSize());
		zoomValue = 1.1f;
		view.zoom(zoomValue);
		window->setView(view);
		LOG("%.3f, %.3f", view.getSize().x, view.getSize().x);
	}
	else if (engine->inputModule->IsMouseWheelScrollingDown()) {
		sf::View view = window->getView();
		view.setSize(view.getSize());
		zoomValue = 1.1f;
		view.zoom(zoomValue);
		window->setView(view);
		LOG("%.3f, %.3f", view.getSize().x, view.getSize().x);
	}*/

	return true;
}

bool ModuleSceneWindow::PostUpdate()
{
	window->clear(sf::Color(100, 100, 100, 255));

	/*for (vector<GameObject*>::iterator it = engine->sceneManagerModule->gameObjectsList.begin(); it != engine->sceneManagerModule->gameObjectsList.end(); it++) {
		drawableObjects.push_back((*it));
	}*/

	for (int i = 0; i < drawableObjects.size(); i++) {
		if (drawableObjects[i]->isActive()) {
			//if (engine->inputModule->IsMouseButtonReleased(sf::Mouse::Left)) {
			//	if (drawableObjects[i]->gameObjectSprite->getGlobalBounds().contains(sf::Vector2f(engine->inputModule->GetmousePosition().x - windowPos.x, engine->inputModule->GetmousePosition().y - windowPos.y))) {
			//		//engine->sceneManagerModule->selectedGameObjects.push_back(drawableObjects[i]);
			//	}
			//}
			ComponentAnimation* anim = (ComponentAnimation*)drawableObjects[i]->GetComponent(Component::Animaton);
			if(anim != nullptr) anim->UpdateAnimation();
			window->draw(*drawableObjects[i]->gameObjectSprite);
		}
	}

	window->display();

	//drawableObjects.clear();

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

void ModuleSceneWindow::ResizeScene(float x, float y, sf::Vector2u engineScreenSize)
{
	sf::View view = window->getView();
	float aspecRatio = float(x) / float(y);
	view.setSize(y * aspecRatio, y);
	zoomValue = (engineScreenSize.x / (x * 2)) + (engineScreenSize.y / (y * 2));
	view.zoom(zoomValue);
	window->setView(view);
	//LOG("%.3f", zoomValue);
}


