#include "ModuleEditor.h"
#include "Engine.h"
#include "ModuleEngineWindow.h"
#include "PanelHierarchy.h"
#include "PanelAssets.h"
#include "PanelDetails.h"
#include "PanelScene.h"
#include "ModuleSceneWindow.h"
#include "ModuleSceneManager.h"
#include "tinyfiledialogs.h"

ModuleEditor::ModuleEditor()
{
	moduleName = "Editor";
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
	ImGui::LoadDocks();
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
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 previusColor = style.Colors[ImGuiCol_Text];
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene")) {
				engine->sceneManagerModule->NewScene();
			}
			if (ImGui::MenuItem("Load Scene")) {
				char const * lFilterPatterns[1] = { "*.scene" };
				const char* path = tinyfd_openFileDialog("Load Scene...", NULL, 1, lFilterPatterns, NULL, 0);
				if (path != NULL) {
					engine->sceneManagerModule->LoadScene(path);
				}
			}
			if (ImGui::MenuItem("Save Scene")) {
				char const * lFilterPatterns[1] = { "*.scene" };
				const char* path = tinyfd_saveFileDialog("Save Scene...", (engine->sceneManagerModule->sceneName + ".scene").c_str(), 1, lFilterPatterns, NULL);
				if (path != NULL) {
					string str(path);
					bool getChar = false;
					string newSceneName;
					for (string::reverse_iterator it = str.rbegin(); it != str.rend(); it++) {
						if (*it == '\\') {
							getChar = false;
						}
						if (getChar) {
							newSceneName.insert(0, 1, *it);
						}
						if (*it == '.') {
							getChar = true;
						}
					}
					engine->sceneManagerModule->SaveScene(path);
					engine->sceneManagerModule->savingIndex = 0;
					engine->engineWindow->SetSceneName(newSceneName);
				}
			}
			if (ImGui::MenuItem("Exit")) {
				engine->quit = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			//ImGui::MenuItem("Animator", "1", true);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
		style.Colors[ImGuiCol_Text] = previusColor;
	}

	ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	displaySize.y -= 10;
	ImGui::SetNextWindowSize(displaySize);
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

	ImGui::Separator();
	ImGui::BeginDockspace();
	for (vector<Panel*>::iterator it = editorPanels.begin(); it != editorPanels.end(); it++) {
		if ((*it)->IsActive())
		{
			(*it)->DrawPanel();
		}
	}
	ImGui::EndDockspace();
	ImGui::End();
	return ret;
}

bool ModuleEditor::CleanUp()
{
	LOG("Freeing module editor");

	for (vector<Panel*>::iterator it = editorPanels.begin(); it != editorPanels.end(); ++it)
		RELEASE(*it);

	editorPanels.clear();
	ImGui::SaveDocks();
	ImGui::SFML::Shutdown();
	return true;
}
