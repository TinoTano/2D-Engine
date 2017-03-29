#pragma once

#include "Module.h"

class ModuleEngineWindow : public Module
{
public:
	ModuleEngineWindow();
	virtual ~ModuleEngineWindow();

	bool Awake();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

public:
	sf::RenderWindow* window;
	string windowName;
};

