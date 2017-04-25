#include "ModuleInput.h"
#include "ModuleEngineWindow.h"
#include "Engine.h"
#include "ModuleEditor.h"
#include "ModuleSceneWindow.h"


ModuleInput::ModuleInput()
{
	moduleName = "Input";
}


ModuleInput::~ModuleInput()
{
}

bool ModuleInput::PreUpdate()
{
	sf::Event event;
	windowWidth = engine->engineWindow->window->getSize().x;
	windowHeight = engine->engineWindow->window->getSize().y;
	while (engine->engineWindow->window->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		// check the type of the event...
		switch (event.type)
		{
			// window closed
		case sf::Event::Closed:
			engine->engineWindow->window->close();
			engine->quit = true;
			break;

		case sf::Event::Resized:
			break;

		case sf::Event::LostFocus:

			break;

		case sf::Event::GainedFocus:

			break;

		case sf::Event::KeyPressed:
			if (keyboardKeys.find(event.key.code) != keyboardKeys.end()) {
				keyboardKeys[event.key.code] = KEY_REPEATED;
			}
			else {
				keyboardKeys.insert(pair<sf::Keyboard::Key, KEY_STATE>(event.key.code, KEY_PRESSED));
			}
			break;

		case sf::Event::KeyReleased:
			keyboardKeys[event.key.code] = KEY_RELEASED;
			break;

		case sf::Event::MouseButtonPressed:
			if (mouseBbuttons.find(event.mouseButton.button) != mouseBbuttons.end()) {
				mouseBbuttons[event.mouseButton.button] = KEY_REPEATED;
			}
			else {
				mouseBbuttons.insert(pair<sf::Mouse::Button, KEY_STATE>(event.mouseButton.button, KEY_PRESSED));
			}
			mousePosition = { event.mouseButton.x, event.mouseButton.y };
			break;

		case sf::Event::MouseButtonReleased:
			mouseBbuttons[event.mouseButton.button] = KEY_RELEASED;
			mousePosition = { event.mouseButton.x, event.mouseButton.y };
			break;

		case sf::Event::MouseMoved:
			mousePosition = { event.mouseMove.x, event.mouseMove.y };
			break;

		case sf::Event::MouseEntered:
			isMouseInsideWindow = true;
			break;

		case sf::Event::MouseLeft:
			isMouseInsideWindow = false;
			break;

			// we don't process other types of events
		default:
			break;
		}
	}

	return true;
}

bool ModuleInput::CleanUp()
{
	return true;
}

bool ModuleInput::IsKeyPressed(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}

bool ModuleInput::IsKeyRepeated(sf::Keyboard::Key key)
{
	if (keyboardKeys[key] != NULL && keyboardKeys[key] == KEY_REPEATED) {
		return true;
	}
	return false;
}

bool ModuleInput::IsKeyReleased(sf::Keyboard::Key key)
{
	if (keyboardKeys[key] != NULL && keyboardKeys[key] == KEY_RELEASED) {
		keyboardKeys.erase(key);
		return true;
	}
	return false;
}

bool ModuleInput::IsMouseButtonPressed(sf::Mouse::Button button)
{
	return sf::Mouse::isButtonPressed(button);
}

bool ModuleInput::IsMouseButtonRepeated(sf::Mouse::Button button)
{
	if (mouseBbuttons[button] != NULL && mouseBbuttons[button] == KEY_REPEATED) {
		return true;
	}
	return false;
}

bool ModuleInput::IsMouseButtonReleased(sf::Mouse::Button button)
{
	if (mouseBbuttons[button] != NULL && mouseBbuttons[button] == KEY_RELEASED) {
		mouseBbuttons.erase(button);
		return true;
	}
	return false;
}

sf::Vector2i ModuleInput::GetmousePosition() const
{
	return mousePosition;
}
