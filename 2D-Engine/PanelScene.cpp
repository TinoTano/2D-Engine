#include "PanelScene.h"
#include "ModuleSceneWindow.h"
#include "ModuleEngineWindow.h"
#include "Engine.h"


PanelScene::PanelScene()
{
	active = true;
	panelName = "Scene";
}


PanelScene::~PanelScene()
{
}

void PanelScene::DrawPanel()
{
	if (ImGui::BeginDock(panelName.c_str())) {
		engine->sceneWindow->Resize((uint)ImGui::GetWindowSize().x, (uint)ImGui::GetWindowSize().y);
		ImGui::Image(engine->sceneWindow->window->getTexture(), sf::Color::White, sf::Color::Transparent);
		//engine->sceneWindow->window->setView(sf::View(sf::FloatRect(0, 0, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y)));
	}
	
	ImGui::EndDock();
}
