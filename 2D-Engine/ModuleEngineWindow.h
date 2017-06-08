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
	void SetSceneName(string name);

public:
	sf::RenderWindow* window = nullptr;

private:
	string windowName;
};

