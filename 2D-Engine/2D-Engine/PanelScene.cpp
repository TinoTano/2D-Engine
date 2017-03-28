#include "PanelScene.h"



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
}
