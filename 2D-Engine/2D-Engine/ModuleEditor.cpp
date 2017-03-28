#include "ModuleEditor.h"
#include "Engine.h"
#include "ModuleRenderWindow.h"
#include "PanelHierarchy.h"
#include "PanelAssets.h"
#include "PanelDetails.h"

#include "imgui-1.49\imgui.h"
#include "imgui-1.49\imgui-SFML.h"

ModuleEditor::ModuleEditor()
{
}


ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Awake()
{
	ImGui::SFML::Init(*(engine->renderWindowModule->window), NULL);
	engine->renderWindowModule->window->resetGLStates();

	editorPanels.push_back(hierarchyPanel = new PanelHierarchy());
	editorPanels.push_back(assetsPanel = new PanelAssets());
	editorPanels.push_back(detailsPanel = new PanelDetails());

	return true;
}

bool ModuleEditor::PreUpdate() {
	ImGui::SFML::Update(*(engine->renderWindowModule->window), engine->time);
	return true;
}

bool ModuleEditor::Update(float deltaTime)
{
	bool ret = true;

	if (ImGui::BeginMainMenuBar())
	{
		bool selected = false;
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New Scene");
			ImGui::MenuItem("Load Scene");
			ImGui::MenuItem("Save Scene");
			ImGui::MenuItem("Exit");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("Console", "1", true);
			ImGui::MenuItem("Scene Hierarchy", "2", true);
			ImGui::MenuItem("Properties", "3", true);
			ImGui::MenuItem("Configuration", "4", true);
			ImGui::MenuItem("Resource Browser", "5", true);
			ImGui::MenuItem("Quick Bar", "6", true);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	for (vector<Panel*>::iterator it = editorPanels.begin(); it != editorPanels.end(); it++) {
		if ((*it)->IsActive())
		{
			ImGui::SetNextWindowPos((*it)->panelPosition, ImGuiSetCond_Always);
			ImGui::SetNextWindowSize(ImVec2((*it)->panelWidth, (*it)->panelHeight), ImGuiSetCond_Always);
			(*it)->DrawPanel();
		}
	}

	return ret;
}

bool ModuleEditor::CleanUp()
{
	ImGui::SFML::Shutdown();
	return true;
}
