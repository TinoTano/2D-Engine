#include "ModuleEditor.h"
#include "Engine.h"
#include "ModuleEngineWindow.h"
#include "PanelHierarchy.h"
#include "PanelAssets.h"
#include "PanelProperties.h"
#include "PanelScene.h"
#include "ModuleSceneWindow.h"
#include "ModuleSceneManager.h"
#include "tinyfiledialogs.h"
#include "PanelAnimator.h"

ModuleEditor::ModuleEditor()
{
	moduleName = "Editor";
}


ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Awake()
{
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	font = io.Fonts->AddFontFromFileTTF("../Data/OpenSans-Semibold.ttf", 16);
	ImGui::SFML::Init(*(engine->engineWindow->window), NULL);
	engine->engineWindow->window->resetGLStates();
	editorPanels.push_back(scenePanel = new PanelScene());
	editorPanels.push_back(assetsPanel = new PanelAssets());
	editorPanels.push_back(hierarchyPanel = new PanelHierarchy());
	editorPanels.push_back(propertiesPanel = new PanelProperties());
	editorPanels.push_back(consolePanel = new PanelConsole());
	editorPanels.push_back(animatorPanel = new PanelAnimator());
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
	ImGui::PushFont(font);
	if (ImGui::BeginMainMenuBar())
	{
		bool selected = false;
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 previusColor = style.Colors[ImGuiCol_Text];
		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		ImGui::SetWindowFontScale(1.1f);
		if (ImGui::BeginMenu("File"))
		{
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
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
					engine->engineWindow->SetSceneName(newSceneName);
					engine->sceneManagerModule->SaveScene(path);
					engine->sceneManagerModule->savingIndex = 0;
				}
			}
			if (ImGui::MenuItem("Exit")) {
				engine->QuitEngine();
			}

			ImGui::EndMenu();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		}
		
		if (ImGui::BeginMenu("Windows"))
		{
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			//ImGui::MenuItem("Animator", "1", true);
			ImGui::EndMenu();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		}
	
		if (ImGui::BeginMenu("Help"))
		{
			style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			ImGui::EndMenu();
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
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

	/*ImGui::SameLine();
	ImGui::Text("%f", engine->GetFPS());*/

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
	ImGui::PopFont();
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

void ModuleEditor::PrintLog(string log)
{
	if (consolePanel != nullptr) {
		consolePanel->AddLog(log);
	}
}

void ModuleEditor::PrintWarningLog(string log)
{
	if (consolePanel != nullptr) {
		consolePanel->AddLog(log, false, true);
	}
}

void ModuleEditor::PrintErrorLog(string log)
{
	if (consolePanel != nullptr) {
		consolePanel->AddLog(log, true, false);
	}
}
