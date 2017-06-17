#pragma once

#include "Module.h"
#include "GameObject.h"

class ModuleSceneWindow : public Module
{
public:
	ModuleSceneWindow();
	ModuleSceneWindow(sf::Vector2i size);
	virtual ~ModuleSceneWindow();

	bool Awake();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();
	void ResizeScene(float x, float y, sf::Vector2u engineScreenSize);

public:
	sf::RenderTexture* window = nullptr;
	deque<GameObject*> drawableObjects;
	sf::Vector2i windowSize;
	string windowName;
	map<int,vector<float>> layers;
	sf::Vector2f windowPos;
	float zoomValue = 0;
};

