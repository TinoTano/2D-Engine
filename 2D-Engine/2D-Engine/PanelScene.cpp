#include "PanelScene.h"
#include "ModuleSceneWindow.h"
#include "ModuleEngineWindow.h"
#include "Engine.h"


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
	ImGui::Image(engine->sceneWindow->window->getTexture(), sf::Color::White, sf::Color::Transparent);
}
