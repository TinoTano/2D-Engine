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
	void Resize(uint width, uint height);

public:
	sf::RenderTexture* window;
	deque<GameObject*> drawableObjects;
	sf::Vector2i windowSize;
	string windowName;
};

