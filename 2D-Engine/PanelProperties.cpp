#include "PanelProperties.h"
#include "Engine.h"
#include "ModuleSceneManager.h"
#include "ModuleEditor.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ModuleEngineWindow.h"
#include "tinyfiledialogs.h"


PanelProperties::PanelProperties()
{
	active = true;
	panelName = "Properties";
}


PanelProperties::~PanelProperties()
{
}

void PanelProperties::DrawPanel()
{
	if (ImGui::BeginDock(panelName.c_str(), false, false, ImGuiWindowFlags_HorizontalScrollbar)) {
		GameObject* selectedGameObject = nullptr;
		if (engine->sceneManagerModule->selectedGameObjects.size() == 1) {
			selectedGameObject = engine->sceneManagerModule->selectedGameObjects.front();
		}

		if (selectedGameObject != nullptr) {
			bool gameObjectActive = selectedGameObject->isActive();
			ImGui::SameLine();
			if (ImGui::Checkbox("", &gameObjectActive)) {
				selectedGameObject->SetActive(gameObjectActive);
			}
			ImGui::SameLine();
			ImGui::Text("Name: %s", selectedGameObject->name.c_str());
			ImGui::Text("Tag:");
			ImGui::SameLine();
			if (ImGui::SmallButton(selectedGameObject->tag.c_str())) {
				ImGui::OpenPopup("Tags");
			}
			if (ImGui::BeginPopup("Tags")) {
				for (int i = 0; i < 5; i++) {
					string name = "tag" + to_string(i);
					if (ImGui::MenuItem(name.c_str())) {
						selectedGameObject->tag = name;
					}
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			ImGui::Text("Layer:");
			ImGui::SameLine();
			if (ImGui::SmallButton(selectedGameObject->layer.c_str())) {
				ImGui::OpenPopup("Layers");
			}
			if (ImGui::BeginPopup("Layers")) {
				for (int i = 0; i < 5; i++) {
					string name = "layer" + to_string(i);
					if (ImGui::MenuItem(name.c_str())) {
						selectedGameObject->layer = name;
					}
				}
				ImGui::EndPopup();
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			for (list<Component*>::iterator it = selectedGameObject->componentsList.begin(); it != selectedGameObject->componentsList.end(); it++) {
				DrawComponent((*it));
				ImGui::Separator();
				ImGui::Spacing();
			}

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("Components");
			}
			if (ImGui::BeginPopup("Components"))
			{
				if (ImGui::MenuItem("Transform")) {
					if (selectedGameObject->GetComponent(Component::Transform) == nullptr) {
						selectedGameObject->AddComponent(Component::Transform);
					}
				}
				if (ImGui::MenuItem("RigidBody")) {
					if (selectedGameObject->GetComponent(Component::RigidBody) == nullptr) {
						selectedGameObject->AddComponent(Component::RigidBody);
					}
				}
				if (ImGui::MenuItem("SpriteRenderer")) {
					if (selectedGameObject->GetComponent(Component::SpriteRenderer) == nullptr) {
						selectedGameObject->AddComponent(Component::SpriteRenderer);
					}
				}
				if(ImGui::BeginMenu("Scripts")) {
					if (engine->editorModule->scriptList.empty()) {
						ImGui::MenuItem("No scripts");
					}
					else {
						for (list<fs::path*>::iterator it = engine->editorModule->scriptList.begin(); it != engine->editorModule->scriptList.end(); it++) {
							if (ImGui::MenuItem((*it)->filename().string().c_str())){
								ComponentScript* script = (ComponentScript*)selectedGameObject->AddComponent(Component::Script);
								script->SetScriptName((*it)->filename().string().c_str());
								script->SetScriptPath((*it)->string());
							}
						}
					}
					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
		}
	}

	ImGui::EndDock();
}

void PanelProperties::DrawComponent(Component* component)
{
	switch (component->type)
	{
	case Component::Transform:
		DrawTransformPanel((ComponentTransform*)component);
		break;
	case Component::Camera:
		break;
	case Component::RigidBody:
		break;
	case Component::SpriteRenderer:
		DrawSpriteRendererPanel((ComponentSpriteRenderer*)component);
		break;
	case Component::BoxCollider:
		break;
	case Component::CircleCollider:
		break;
	case Component::AudioSource:
		break;
	case Component::Animator:
		break;
	case Component::Script:
		DrawScriptPanel((ComponentScript*)component);
		break;
	default:
		break;
	}
}

void PanelProperties::DrawTransformPanel(ComponentTransform * transform)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		sf::Vector3f position;
		if (transform->gameObject->isRoot) {
			position = transform->GetGlobalPosition();
		}
		else {
			position = transform->GetLocalPosition();
		}
		float rotation = transform->GetRotation();
		sf::Vector2f scale = transform->GetScale();

		if (ImGui::DragFloat3("Position", (float*)&position, 0.25f)) {
			transform->SetPosition(position, transform->gameObject->isRoot);
		}
		if (ImGui::DragFloat("Rotation", (float*)&rotation, 0.25f)) {
			transform->SetRotation(rotation);
		}
		if (ImGui::DragFloat2("Scale", (float*)&scale, 0.25f)) {
			transform->SetScale(scale);
		}
	}
}

void PanelProperties::DrawSpriteRendererPanel(ComponentSpriteRenderer * spriteRenderer)
{
	if (ImGui::CollapsingHeader("Sprite Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
		sf::Sprite* sprite = spriteRenderer->gameObject->gameObjectSprite;
		ImGui::Image(*sprite, ImVec2(100.f, 100.f), sf::Color::White, sf::Color::Transparent);
		if (ImGui::IsItemClicked(0)) {
			char const * lFilterPatterns[2] = { "*.png", "*.jpg" };
			const char* path = tinyfd_openFileDialog("Load Image...", NULL, 1, lFilterPatterns, NULL, 0);
			if (path != NULL) {
				spriteRenderer->ChangeSprite(path);
			}
		}
		if (ImGui::Checkbox("Flip x", &spriteRenderer->isFlippedX)) {
			if (spriteRenderer->isFlippedX) {
				sprite->setTextureRect(sf::IntRect(sprite->getTextureRect().width, sprite->getTextureRect().top != 0 ? sprite->getTextureRect().top : 0, -sprite->getTextureRect().width, sprite->getTextureRect().height));
			}
			else {
				sprite->setTextureRect(sf::IntRect(0, sprite->getTextureRect().top != 0 ? sprite->getTextureRect().top : 0, -sprite->getTextureRect().width, sprite->getTextureRect().height));
			}
		}
		if (ImGui::Checkbox("Flip y", &spriteRenderer->isFlippedY)) {
			if (spriteRenderer->isFlippedY) {
				sprite->setTextureRect(sf::IntRect(sprite->getTextureRect().left != 0 ? sprite->getTextureRect().left : 0, sprite->getTextureRect().height, sprite->getTextureRect().width, -sprite->getTextureRect().height));
			}
			else {
				sprite->setTextureRect(sf::IntRect(sprite->getTextureRect().left != 0 ? sprite->getTextureRect().left : 0, 0, sprite->getTextureRect().width, -sprite->getTextureRect().height));
			}
		}
	}
}

void PanelProperties::DrawScriptPanel(ComponentScript * script)
{
	
	if (ImGui::CollapsingHeader(script->GetScriptName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Spacing();
		bool scriptHasChanged = ScriptHasChanged(script->GetScriptPath());

		if (script->GetScriptFields().empty() || scriptHasChanged) {
			UpdateScript(script);
			script->SetScriptFields();
		}

		vector<ScriptField*> scriptFields = script->GetScriptFields();
		for (int j = 0; j < scriptFields.size(); j++) {
			switch (scriptFields[j]->propertyType) {
				case ScriptField::Integer:
				{
					int i = script->GetScript()->GetIntProperty(scriptFields[j]->fieldName.c_str());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::InputInt(("##" + scriptFields[j]->fieldName).c_str(), &i)) {
						script->GetScript()->SetIntProperty(scriptFields[j]->fieldName.c_str(), i);
					}
				}
					break;
				case ScriptField::Float:
				{
					float f = script->GetScript()->GetFloatProperty(scriptFields[j]->fieldName.c_str());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::InputFloat(("##" + scriptFields[j]->fieldName).c_str(), &f, 0.001f, 0.01f, 3)) {
						script->GetScript()->SetFloatProperty(scriptFields[j]->fieldName.c_str(), f);
					}
				}
					break;
				case ScriptField::Bool:
				{
					bool b = script->GetScript()->GetBoolProperty(scriptFields[j]->fieldName.c_str());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::Checkbox(("##" + scriptFields[j]->fieldName).c_str(), &b)) {
						script->GetScript()->SetBoolProperty(scriptFields[j]->fieldName.c_str(), b);
					}
				}
					break;
				case ScriptField::String:
				{
					
					static char textToRender[256];
					string str = script->GetScript()->GetStringProperty(scriptFields[j]->fieldName.c_str());
					strncpy(textToRender, str.data(), str.size());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::InputText(("##" + scriptFields[j]->fieldName).c_str(), textToRender, 256)) {
						script->GetScript()->SetStringProperty(scriptFields[j]->fieldName.c_str(), string(textToRender).c_str());
					}
					memset(textToRender, 0, sizeof textToRender);
				}
					break;
				case ScriptField::GameObject:
				{
					/*GameObject* go = script->GetScript()->GetGameObjectProperty(scriptFields[j]->fieldName.c_str());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					ImGui::Text("%s", go->name.c_str());*/
				}
					break;
				case ScriptField::Vector3i:
				{
					sf::Vector3i v3i = script->GetScript()->GetVec3iProperty(scriptFields[j]->fieldName.c_str());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::DragInt3(("##" + scriptFields[j]->fieldName).c_str(), (int*)&v3i, 0.25f)) {
						script->GetScript()->SetVec3iProperty(scriptFields[j]->fieldName.c_str(), v3i);
					}
				}
					break;
				case ScriptField::Vector4i:
				{
					ImVec4 v4i = script->GetScript()->GetVec4iProperty(scriptFields[j]->fieldName.c_str());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::DragFloat4(("##" + scriptFields[j]->fieldName).c_str(), (float*)&v4i, 0.25f)) {
						script->GetScript()->SetVec4iProperty(scriptFields[j]->fieldName.c_str(), v4i);
					}
				}
				break;
			}
		}
	}
}

bool PanelProperties::ScriptHasChanged(string path)
{
	CFile cfile;
	cfile.Open(path.c_str(), CFile::modeRead);

	CFileStatus status;

	if (scriptsChangesData.find(path) == scriptsChangesData.end()) {
		CTime m_prevTime;
		scriptsChangesData[path] = m_prevTime;
	}

	if (cfile.GetStatus(status))
	{
		if (scriptsChangesData[path] < status.m_mtime)
		{
			scriptsChangesData[path] = status.m_mtime;
			return true;
		}
	}
	return false;
}

void PanelProperties::UpdateScript(ComponentScript * script)
{
	ifstream inFile;
	inFile.open(script->GetScriptPath().c_str());
	if (inFile.is_open()) {
		stringstream strStream;
		strStream << inFile.rdbuf();//read the file
		string str = strStream.str();//str holds the content of the file

		if (!str.empty()) {
			script->InitScript(str);
		}
		else {
			LOG_WARNING("Script %s is empty", script->GetScriptName().c_str());
		}
	}
	else {
		LOG_WARNING("Failed to open script %s", script->GetScriptName().c_str());
	}
}
