#include "PanelHierarchy.h"
#include "GameObject.h"
#include "Engine.h"
#include "ModuleSceneManager.h"
#include "ModuleInput.h"
#include "ModuleSceneWindow.h"


PanelHierarchy::PanelHierarchy()
{
	active = true;
	panelName = "Hierarchy";
}


PanelHierarchy::~PanelHierarchy()
{
}

void PanelHierarchy::DrawPanel()
{
	if (ImGui::BeginDock(panelName.c_str(), false, false, ImGuiWindowFlags_HorizontalScrollbar)) {
		node = 0;
		if (ImGui::IsMouseClicked(1) && ImGui::IsMouseHoveringWindow()) {
			ImGui::OpenPopup("GameObject Options");
		}

		if (ImGui::BeginPopup("GameObject Options"))
		{
			if (!engine->sceneManagerModule->selectedGameObjects.empty()) {
				if (ImGui::MenuItem("Duplicate")) {
					for (list<GameObject*>::iterator it = engine->sceneManagerModule->selectedGameObjects.begin(); it != engine->sceneManagerModule->selectedGameObjects.end(); it++) {
						if ((*it)->parent != nullptr) {
							if (find(engine->sceneManagerModule->selectedGameObjects.begin(), engine->sceneManagerModule->selectedGameObjects.end(), (*it)->parent) != engine->sceneManagerModule->selectedGameObjects.end()) {
								continue; //If parent will be duplicated skip this because parent will take care of childs;
							}
						}
						engine->sceneManagerModule->DuplicateGameObject(*it);
					}
				}
				if (ImGui::MenuItem("Delete")) {
					for (list<GameObject*>::iterator it = engine->sceneManagerModule->selectedGameObjects.begin(); it != engine->sceneManagerModule->selectedGameObjects.end(); it++) {
						(*it)->Destroy();
					}
				}
				if (engine->sceneManagerModule->selectedGameObjects.size() == 1) {
					if (ImGui::MenuItem("Rename")) {
						showRenameWindow = true;
					}
					if (ImGui::MenuItem("Create Child")) {
						GameObject* parent = nullptr;
						parent = engine->sceneManagerModule->selectedGameObjects.front();
						engine->sceneWindow->drawableObjects.push_back(engine->sceneManagerModule->CreateGameObject(parent));
					}
				}
				ImGui::Separator();
			}
			if (ImGui::MenuItem("Create Game Object")) {
				engine->sceneWindow->drawableObjects.push_back(engine->sceneManagerModule->CreateGameObject(nullptr));
			}

			ImGui::EndPopup();
		}

		if (showRenameWindow) {
			ImGui::Begin("Rename", &active,
				ImGuiWindowFlags_NoFocusOnAppearing |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_ShowBorders |
				ImGuiWindowFlags_NoTitleBar);

			ImGui::Text("Enter new name");
			static char inputText[20];
			ImGui::InputText("", inputText, 20);
			if (showRenameError) {
				ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Name cannot be blank");
			}
			if (ImGui::Button("Confirm")) {
				bool isBlankString = true;
				for (int i = 0; inputText[i] != '\0'; i++) {
					if (inputText[i] != ' ') {
						isBlankString = false;
						break;
					}
				}
				if (!isBlankString) {
					map<string, int>::iterator it = engine->sceneManagerModule->sceneGameObjectsNameCounter.find(inputText);
					if (it != engine->sceneManagerModule->sceneGameObjectsNameCounter.end()) {
						engine->sceneManagerModule->sceneGameObjectsNameCounter[inputText] += 1;
						engine->sceneManagerModule->selectedGameObjects.front()->Rename(inputText + '(' + to_string(it->second) + ')');
					}
					else {
						engine->sceneManagerModule->sceneGameObjectsNameCounter[inputText] = 1;
						engine->sceneManagerModule->selectedGameObjects.front()->Rename(inputText);
					}
					showRenameError = false;
					showRenameWindow = false;
				}
				else {
					showRenameError = true;
				}
				strcpy(inputText, "");
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				showRenameWindow = false;
				strcpy(inputText, "");
			}
			ImGui::End();
		}

		for (list<GameObject*>::iterator it = engine->sceneManagerModule->sceneRootObjects.begin(); it != engine->sceneManagerModule->sceneRootObjects.end(); it++) {
			DrawChilds(*it);
		}
	}

	ImGui::EndDock();
}

void PanelHierarchy::DrawChilds(GameObject* gameObject)
{
	sprintf_s(nodeName, 30, "%s##node_%i", gameObject->name.c_str(), node++);
	uint flag = 0;

	if (gameObject->childs.empty()) {
		flag |= ImGuiTreeNodeFlags_Leaf;
	}
	
	for (list<GameObject*>::iterator it = engine->sceneManagerModule->selectedGameObjects.begin(); it != engine->sceneManagerModule->selectedGameObjects.end(); it++) {
		if (gameObject == *it) {
			flag |= ImGuiTreeNodeFlags_Selected;
			break;
		}
	}

	if (ImGui::TreeNodeEx(nodeName, flag))
	{
		CheckMouseOver(gameObject);
		for (list<GameObject*>::iterator it = gameObject->childs.begin(); it != gameObject->childs.end(); ++it) {
			DrawChilds(*it);
		}

		ImGui::TreePop();
	}
	else {
		CheckMouseOver(gameObject);
	}
	
}

void PanelHierarchy::CheckMouseOver(GameObject* gameObject)
{
	if (ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)) {
		if (ImGui::IsItemHoveredRect())
		{
			list<GameObject*>::iterator it;

			if (!engine->sceneManagerModule->selectedGameObjects.empty()) {
				it = find(engine->sceneManagerModule->selectedGameObjects.begin(), engine->sceneManagerModule->selectedGameObjects.end(), gameObject);
				if (it == engine->sceneManagerModule->selectedGameObjects.end()) {
					engine->sceneManagerModule->selectedGameObjects.push_back(gameObject);
				}
			}
			else {
				engine->sceneManagerModule->selectedGameObjects.push_back(gameObject);
			}
		}
		else {
			if (ImGui::IsMouseHoveringWindow() && !ImGui::IsMouseClicked(1)) {
				if (!engine->inputModule->IsKeyRepeated(sf::Keyboard::LControl) && !engine->sceneManagerModule->selectedGameObjects.empty() &&
					!showRenameWindow) {
					engine->sceneManagerModule->selectedGameObjects.remove(gameObject);
				}
			}
		}
	}

	if (ImGui::IsMouseDoubleClicked(0))
	{
		if (ImGui::IsItemHoveredRect())
		{
			if (!showRenameWindow) {
				showRenameWindow = true;
			}
		}
	}
}
