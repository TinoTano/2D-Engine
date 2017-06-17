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

bool ModuleInput::Awake()
{
	StoreStringKeys();

	return true;
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
			engine->QuitEngine();
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

		case sf::Event::MouseWheelScrolled:
			if (event.mouseWheelScroll.delta == -1) {
				mouseWheelScrollDown = true;
				mouseWheelScrollUp = false;
			}
			else if (event.mouseWheelScroll.delta == 1) {
				mouseWheelScrollDown = false;
				mouseWheelScrollUp = true;
			}
			break;

			// we don't process other types of events
		default:
			break;
		}
	}
	
	return true;
}

bool ModuleInput::PostUpdate()
{
	mouseWheelScrollDown = false;
	mouseWheelScrollUp = false;
	return true;
}

bool ModuleInput::CleanUp()
{
	return true;
}

bool ModuleInput::IsKeyPressed(sf::Keyboard::Key key) const
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

bool ModuleInput::IsMouseButtonPressed(sf::Mouse::Button button) const
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

bool ModuleInput::IsMouseWheelScrollingUp() const
{
	return mouseWheelScrollUp;
}

bool ModuleInput::IsMouseWheelScrollingDown() const
{
	return mouseWheelScrollDown;
}

void ModuleInput::StoreStringKeys()
{
	stringKeys["A"] = sf::Keyboard::A;
	stringKeys["B"] = sf::Keyboard::B;
	stringKeys["C"] = sf::Keyboard::C;
	stringKeys["D"] = sf::Keyboard::D;
	stringKeys["E"] = sf::Keyboard::E;
	stringKeys["F"] = sf::Keyboard::F;
	stringKeys["G"] = sf::Keyboard::G;
	stringKeys["H"] = sf::Keyboard::H;
	stringKeys["I"] = sf::Keyboard::I;
	stringKeys["J"] = sf::Keyboard::J;
	stringKeys["K"] = sf::Keyboard::K;
	stringKeys["L"] = sf::Keyboard::L;
	stringKeys["M"] = sf::Keyboard::M;
	stringKeys["N"] = sf::Keyboard::N;
	stringKeys["O"] = sf::Keyboard::O;
	stringKeys["P"] = sf::Keyboard::P;
	stringKeys["Q"] = sf::Keyboard::Q;
	stringKeys["R"] = sf::Keyboard::R;
	stringKeys["S"] = sf::Keyboard::S;
	stringKeys["T"] = sf::Keyboard::T;
	stringKeys["U"] = sf::Keyboard::U;
	stringKeys["V"] = sf::Keyboard::V;
	stringKeys["W"] = sf::Keyboard::W;
	stringKeys["X"] = sf::Keyboard::X;
	stringKeys["Y"] = sf::Keyboard::Y;
	stringKeys["Z"] = sf::Keyboard::Z;
	stringKeys["0"] = sf::Keyboard::Num0;
	stringKeys["1"] = sf::Keyboard::Num1;
	stringKeys["2"] = sf::Keyboard::Num2;
	stringKeys["3"] = sf::Keyboard::Num3;
	stringKeys["4"] = sf::Keyboard::Num4;
	stringKeys["5"] = sf::Keyboard::Num5;
	stringKeys["6"] = sf::Keyboard::Num6;
	stringKeys["7"] = sf::Keyboard::Num7;
	stringKeys["8"] = sf::Keyboard::Num8;
	stringKeys["9"] = sf::Keyboard::Num9;
	stringKeys["ESC"] = sf::Keyboard::Escape;
	stringKeys["L_CTRL"] = sf::Keyboard::LControl;
	stringKeys["L_SHIFT"] = sf::Keyboard::LShift;
	stringKeys["L_ALT"] = sf::Keyboard::LAlt;
	stringKeys["L_SYSTEM"] = sf::Keyboard::LSystem;
	stringKeys["R_CTRL"] = sf::Keyboard::RControl;
	stringKeys["R_SHIFT"] = sf::Keyboard::RShift;
	stringKeys["R_ALT"] = sf::Keyboard::RAlt;
	stringKeys["R_SYSTEM"] = sf::Keyboard::RSystem;
	stringKeys["MENU"] = sf::Keyboard::Menu;
	stringKeys["["] = sf::Keyboard::LBracket;
	stringKeys["]"] = sf::Keyboard::RBracket;
	stringKeys[";"] = sf::Keyboard::SemiColon;
	stringKeys[","] = sf::Keyboard::Comma;
	stringKeys["."] = sf::Keyboard::Period;
	stringKeys["'"] = sf::Keyboard::Quote;
	stringKeys["/"] = sf::Keyboard::Slash;
	stringKeys["BACK_SLASH"] = sf::Keyboard::BackSlash;
	stringKeys["~"] = sf::Keyboard::Tilde;
	stringKeys["="] = sf::Keyboard::Equal;
	stringKeys["-"] = sf::Keyboard::Dash;
	stringKeys["SPACE"] = sf::Keyboard::Space;
	stringKeys["RETURN"] = sf::Keyboard::Return;
	stringKeys["BACK_SPACE"] = sf::Keyboard::BackSpace;
	stringKeys["TAB"] = sf::Keyboard::Tab;
	stringKeys["PAGE_UP"] = sf::Keyboard::PageUp;
	stringKeys["PAGE_DOWN"] = sf::Keyboard::PageDown;
	stringKeys["END"] = sf::Keyboard::End;
	stringKeys["HOME"] = sf::Keyboard::Home;
	stringKeys["INSERT"] = sf::Keyboard::Insert;
	stringKeys["DELETE"] = sf::Keyboard::Delete;
	stringKeys["+"] = sf::Keyboard::Add;
	stringKeys["-"] = sf::Keyboard::Subtract;
	stringKeys["*"] = sf::Keyboard::Multiply;
	stringKeys["/"] = sf::Keyboard::Divide;
	stringKeys["LEFT_ARROW"] = sf::Keyboard::Left;
	stringKeys["RIGHT_ARROW"] = sf::Keyboard::Right;
	stringKeys["UP_ARROW"] = sf::Keyboard::Up;
	stringKeys["DOWN_ARROW"] = sf::Keyboard::Down;
	stringKeys["NUM_PAD_0"] = sf::Keyboard::Numpad0;
	stringKeys["NUM_PAD_1"] = sf::Keyboard::Numpad1;
	stringKeys["NUM_PAD_2"] = sf::Keyboard::Numpad2;
	stringKeys["NUM_PAD_3"] = sf::Keyboard::Numpad3;
	stringKeys["NUM_PAD_4"] = sf::Keyboard::Numpad4;
	stringKeys["NUM_PAD_5"] = sf::Keyboard::Numpad5;
	stringKeys["NUM_PAD_6"] = sf::Keyboard::Numpad6;
	stringKeys["NUM_PAD_7"] = sf::Keyboard::Numpad7;
	stringKeys["NUM_PAD_8"] = sf::Keyboard::Numpad8;
	stringKeys["NUM_PAD_9"] = sf::Keyboard::Numpad9;
	stringKeys["F1"] = sf::Keyboard::F1;
	stringKeys["F2"] = sf::Keyboard::F2;
	stringKeys["F3"] = sf::Keyboard::F3;
	stringKeys["F4"] = sf::Keyboard::F4;
	stringKeys["F5"] = sf::Keyboard::F5;
	stringKeys["F6"] = sf::Keyboard::F6;
	stringKeys["F7"] = sf::Keyboard::F7;
	stringKeys["F8"] = sf::Keyboard::F8;
	stringKeys["F9"] = sf::Keyboard::F9;
	stringKeys["F10"] = sf::Keyboard::F10;
	stringKeys["F11"] = sf::Keyboard::F11;
	stringKeys["F12"] = sf::Keyboard::F12;
	stringKeys["F13"] = sf::Keyboard::F13;
	stringKeys["F14"] = sf::Keyboard::F14;
	stringKeys["F15"] = sf::Keyboard::F15;
	stringKeys["PAUSE"] = sf::Keyboard::Pause;
}

sf::Keyboard::Key ModuleInput::StringToKey(string key)
{
	sf::Keyboard::Key ret;
	if (stringKeys.find(key) != stringKeys.end()) {
		ret = stringKeys[key];
	}
	else {
		ret = sf::Keyboard::Unknown;
	}
	return ret;
}

sf::Vector2i ModuleInput::GetmousePosition() const
{
	return mousePosition;
}
