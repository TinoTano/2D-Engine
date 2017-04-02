#include "PanelScene.h"
#include "ModuleSceneWindow.h"
#include "ModuleEngineWindow.h"
#include "Engine.h"
#


PanelScene::PanelScene()
{
	active = true;
	panelName = "Scene";
	dockPos = ImGuiDockSlot_Bottom;
	newPanel = true;
}


PanelScene::~PanelScene()
{
}

void PanelScene::DrawPanel()
{
	engine->sceneWindow->window->setView(sf::View(sf::FloatRect(0, 0, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y)));
	ImGui::Image(engine->sceneWindow->window->getTexture(), sf::Color::White, sf::Color::Transparent);
	
}
