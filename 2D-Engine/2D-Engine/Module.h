#pragma once

#include "LOG.h"
#include <string>
#include <list>
#include <deque>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\System.hpp>

using namespace std;

class Module
{
public:
	Module() : enable(true){}
	virtual ~Module(){}

	void Enable(bool active) {
		if (enable != active) {
			enable = active;
			if (active == true)
				Start();
			else
				CleanUp();
		}
	}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool PreUpdate()
	{
		return true;
	}

	virtual bool Update(float deltaTime)
	{
		return true;
	}

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual void DrawDebug()
	{}

public:
	bool enable;
	string moduleName;
};

