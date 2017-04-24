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
	if (ImGui::BeginDock(panelName.c_str(),false, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
		if ((uint)ImGui::GetWindowSize().x - 13 != engine->sceneWindow->window->getSize().x || (uint)ImGui::GetWindowSize().y - 15 != engine->sceneWindow->window->getSize().y) {
			engine->sceneWindow->Resize((uint)ImGui::GetWindowSize().x - 13, (uint)ImGui::GetWindowSize().y - 15);
		}
		engine->sceneWindow->Draw();
		ImGui::Image(engine->sceneWindow->window->getTexture(), sf::Color::White, sf::Color::Transparent);
	}
	
	ImGui::EndDock();
}
