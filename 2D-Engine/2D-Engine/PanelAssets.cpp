#include "PanelAssets.h"
#include "Engine.h"
#include "ModuleEditor.h"


PanelAssets::PanelAssets()
{
	active = true;
	panelName = "Assets Panel";
	panelPosition = { 0,engine->editorModule->editorPanels[0]->panelHeight };
	panelWidth = 200;
	panelHeight = engine->renderWindowModule->window->getSize().y - panelPosition.y;
}


PanelAssets::~PanelAssets()
{
}

void PanelAssets::DrawPanel()
{
	ImGui::Begin("Assets", &active,
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_ShowBorders);

	ImGui::End();
}

void PanelAssets::DrawChilds()
{
}

void PanelAssets::CheckMouseHover()
{
}

void PanelAssets::Resize(float w, float h)
{
	panelWidth += w;
	panelHeight += h;
	panelPosition.y = engine->editorModule->editorPanels[0]->panelHeight;
}
