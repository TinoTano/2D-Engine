#pragma once

#include "Module.h"

class ModuleWindow : public Module
{
public:
	ModuleWindow();
	virtual ~ModuleWindow();

	bool Start();
	bool CleanUp();

public:
	sf::Window* window;
};

