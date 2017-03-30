#include "ModuleEditor.h"
#include "Engine.h"
#include "ModuleEngineWindow.h"
#include "PanelHierarchy.h"
#include "PanelAssets.h"
#include "PanelDetails.h"
#include "PanelScene.h"
#include "ModuleSceneWindow.h"

ModuleEditor::ModuleEditor()
{
}


ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Awake()
{
	
	ImGui::SFML::Init(*(engine->engineWindow->window), NULL);
	engine->engineWindow->window->resetGLStates();
	editorPanels.push_back(hierarchyPanel = new PanelHierarchy());
	editorPanels.push_back(assetsPanel = new PanelAssets());
	editorPanels.push_back(detailsPanel = new PanelDetails());
	editorPanels.push_back(scenePanel = new PanelScene());
	editorPanels.push_back(consolePanel = new PanelConsole());

	return true;
}

bool ModuleEditor::PreUpdate() {
	ImGui::SFML::Update(*(engine->engineWindow->window), engine->time);
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

	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0,20));
	ImGui::Begin("PanelEditor", NULL, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

	ImGui::SameLine(ImGui::GetIO().DisplaySize.x / 2 - 75);
	if (ImGui::Button("Play", { 50,40 })) {
		
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop", { 50,40 })) {
	
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause", { 50,40 })) {
	
	}

	ImGui::BeginDockspace();
	for (vector<Panel*>::iterator it = editorPanels.begin(); it != editorPanels.end(); it++) {
		if ((*it)->IsActive())
		{
			if ((*it)->newPanel) {				
			}
			ImGui::BeginDock((*it)->panelName.c_str());
			ImGui::SetNextDock((*it)->dockPos);
			(*it)->DrawPanel();
			ImGui::EndDock();
		}
	}
	ImGui::EndDockspace();
	ImGui::End();

	return ret;
}

bool ModuleEditor::CleanUp()
{
	ImGui::SFML::Shutdown();
	return true;
}
