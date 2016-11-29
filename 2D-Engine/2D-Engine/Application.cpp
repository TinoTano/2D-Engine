#include "Application.h"



Application::Application()
{
	if (engineState != STOPPED)
		return;

	appWindow.create(sf::VideoMode(1024, 768, 32), "2D Engine");
	engineState = Application::PLAYING;

	while (!IsExiting())
	{
		GameLoop();
	}

	appWindow.Close();
}


Application::~Application()
{
}

void Application::Start()
{
}

bool Application::IsExiting()
{
	return false;
}

void Application::GameLoop()
{
}
