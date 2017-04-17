#include "PanelAssets.h"
#include "Engine.h"
#include "ModuleEditor.h"

PanelAssets::PanelAssets()
{
	active = true;
	panelName = "Assets";
	newPanel = false;
}


PanelAssets::~PanelAssets()
{
}

void PanelAssets::DrawPanel()
{
	if (ImGui::IsMouseClicked(1) && ImGui::IsMouseHoveringWindow()) {
		
	}
}

void PanelAssets::DrawChilds()
{
}

void PanelAssets::CheckMouseHover()
{
}
