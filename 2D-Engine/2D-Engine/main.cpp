#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "imgui-1.49\imgui.h"
#include "imgui-1.49\imgui-SFML.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1366, 768), "");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	window.resetGLStates();
	sf::Clock deltaClock;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("First window"); // begin first window
		if(ImGui::Button("Button")) {
			printf("Bla");
		}// some stuff
		ImGui::BeginMainMenuBar();
		ImGui::EndMenuBar();
		ImGui::End();

		ImGui::Begin("Another window"); // begin second window
										// some another stuff
		ImGui::End();

		/*ImGui::Begin("Hello, world!");
		ImGui::Button("Look at this pretty button");
		ImGui::End();*/

		window.clear(); // fill background with color
		ImGui::Render();
		window.display();
	}

	ImGui::SFML::Shutdown();

	return 0;
}