#include "PanelScene.h"
#include "ModuleSceneWindow.h"
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
	//engine->sceneWindow->window->setPosition(ImGui::GetWindowPos());
	//engine->sceneWindow->window->setSize(ImGui::GetWindowSize());
}
