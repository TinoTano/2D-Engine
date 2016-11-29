#ifndef _APPLICATION_H_
#define _APPLICATON_H_

#include <list>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

class Application
{

public:
	Application();
	~Application();
	void Start();
	bool IsExiting();
	void GameLoop();

	enum State
	{
		PLAYING,
		STOPPED,
		PAUSED
	};

private:
	std::list<Module*> modules;
	std::string app_name;
	State engineState = State::STOPPED;
	sf::RenderWindow appWindow;
};
extern Application* App;

#endif

