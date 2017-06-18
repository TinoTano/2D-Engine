#include "PanelProperties.h"
#include "Engine.h"
#include "ModuleSceneManager.h"
#include "ModuleEditor.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ModuleEngineWindow.h"
#include "tinyfiledialogs.h"
#include "Animation.h"
#include "ModuleResources.h"
#include "ResourceScript.h"
#include "PanelAnimator.h"
#include "ResourceSound.h"
#include "ResourceMusic.h"


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
				//if (ImGui::MenuItem("RigidBody")) {
				//	if (selectedGameObject->GetComponent(Component::RigidBody) == nullptr) {
				//		selectedGameObject->AddComponent(Component::RigidBody);
				//	}
				//}
				if (ImGui::MenuItem("SpriteRenderer")) {
					if (selectedGameObject->GetComponent(Component::SpriteRenderer) == nullptr) {
						selectedGameObject->AddComponent(Component::SpriteRenderer);
					}
				}
				if (ImGui::MenuItem("Animation")) {
					if (selectedGameObject->GetComponent(Component::Animaton) == nullptr) {
						selectedGameObject->AddComponent(Component::Animaton);
					}
				}
				if (ImGui::MenuItem("Audio")) {
					if (selectedGameObject->GetComponent(Component::AudioSource) == nullptr) {
						selectedGameObject->AddComponent(Component::AudioSource);
					}
				}
				if(ImGui::BeginMenu("Scripts")) {
					if (engine->resourcesModule->GetGameScriptsList().empty()) {
						ImGui::MenuItem("No scripts");
					}
					else {
						for (int i = 0; i < engine->resourcesModule->GetGameScriptsList().size(); i++) {
							if (ImGui::MenuItem(engine->resourcesModule->GetGameScriptsList()[i]->GetName().c_str())) {
								ComponentScript* script = (ComponentScript*)selectedGameObject->AddComponent(Component::Script);
								script->SetScriptName(engine->resourcesModule->GetGameScriptsList()[i]->GetName().c_str());
								script->SetScriptPath(engine->resourcesModule->GetGameScriptsList()[i]->GetScript());
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
		DrawAudioPanel((ComponentAudio*)component);
		break;
	case Component::Animaton:
		DrawAnimatonPanel((ComponentAnimation*)component);
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
			engine->resourcesModule->SetResourcesWindowOpen(Resource::spriteResource, true);
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

	if (engine->resourcesModule->IsResourcesWindowOpen(Resource::spriteResource)) {
		ResourceSprite* sprite = (ResourceSprite*)engine->resourcesModule->DrawResourcesWindow(Resource::spriteResource);
		if (sprite != nullptr) {
			engine->resourcesModule->SetResourcesWindowOpen(Resource::spriteResource, false);
			spriteRenderer->ChangeSprite(sprite->GetSprite(),sprite->GetPath());
		}
	}
}

void PanelProperties::DrawAnimatonPanel(ComponentAnimation * animator)
{
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Spacing();

		if (animator->animationsList.empty()) {
			ImGui::Text("No animations added.");
		}
		else {
			for (int i = 0; i < animator->animationsList.size(); i++) {
				ImGui::Text("Animation %d:", i);
				ImGui::SameLine();
				ImGui::Text("%s", animator->animationsList[i]->GetName().c_str());
				if (ImGui::Button(("Remove##" + to_string(i)).c_str())) {
					animator->RemoveAnimation(i);
				}
				ImGui::SameLine();
				if (ImGui::Button(("Change##" + to_string(i)).c_str())) {
					engine->resourcesModule->SetResourcesWindowOpen(Resource::animationResource, true);
					animPos = i;
				}
			}
		}

		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("Add animation")) {
			engine->resourcesModule->SetResourcesWindowOpen(Resource::animationResource, true);
			animPos = -1;
		}
		
		ImGui::SameLine();
		if (ImGui::Button("Create animation")) {
			for (int i = 0; i < engine->editorModule->editorPanels.size(); i++) {
				if (engine->editorModule->editorPanels[i]->panelName == "Animator") {
					engine->editorModule->editorPanels[i]->SetActive(true);
				}
			}
		}
		ImGui::Spacing();
	}

	if (engine->resourcesModule->IsResourcesWindowOpen(Resource::animationResource)) {
		ResourceAnimation* animation = (ResourceAnimation*)engine->resourcesModule->DrawResourcesWindow(Resource::animationResource);
		if (animation != nullptr) {
			engine->resourcesModule->SetResourcesWindowOpen(Resource::animationResource, false);
			animator->AddAnimation(animation, animPos);
		}
	}
}

void PanelProperties::DrawAudioPanel(ComponentAudio * audio)
{
	if (ImGui::CollapsingHeader("Audio", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Spacing();

		if (audio->soundsListVector.empty()) {
			ImGui::Text("No sounds added.");
		}
		else {
			for (int i = 0; i < audio->soundsListVector.size(); i++) {
				ImGui::Text("Sound %d:", i);
				ImGui::SameLine();
				ImGui::Text("%s", audio->soundsListVector[i]->GetName().c_str());

				float actualVolume = audio->soundsListVector[i]->GetSound()->getVolume();
				ImGui::Text("Volume");
				ImGui::SameLine(70);
				if (ImGui::DragFloat(("##SoundVolume" + to_string(i)).c_str(), &actualVolume, 0.25f, 0, 100)) {
					audio->soundsListVector[i]->GetSound()->setVolume(actualVolume);
				}
				float actualPitch = audio->soundsListVector[i]->GetSound()->getPitch();
				ImGui::Text("Pitch");
				ImGui::SameLine(70);
				if (ImGui::DragFloat(("##SoundPitch" + to_string(i)).c_str(), &actualPitch, 0.25f)) {
					audio->soundsListVector[i]->GetSound()->setPitch(actualPitch);
				}
				bool actualLoop = audio->soundsListVector[i]->GetSound()->getLoop();
				ImGui::Text("Loop");
				ImGui::SameLine(70);
				if (ImGui::Checkbox(("##SoundLoop" + to_string(i)).c_str(), &actualLoop)) {
					audio->soundsListVector[i]->GetSound()->setLoop(actualLoop);
				}
				
				if (audio->soundsListVector[i]->GetSound()->getStatus() == 0 || //Stopped
					audio->soundsListVector[i]->GetSound()->getStatus() == 1) //Paused
				{
					if (ImGui::Button(("Play##Sound" + to_string(i)).c_str())) {
						for (int j = 0; j < audio->soundsListVector.size(); j++) {
							if (audio->soundsListVector[j]->GetSound()->getStatus() == 2) {
								audio->soundsListVector[j]->GetSound()->stop();
							}
						}
						audio->soundsListVector[i]->GetSound()->play();
					}
					if (audio->soundsListVector[i]->GetSound()->getStatus() == 1) {
						ImGui::SameLine();
						if (ImGui::Button(("Stop##Sound" + to_string(i)).c_str())) {
							audio->soundsListVector[i]->GetSound()->stop();
						}
					}
				}
				else {
					if (ImGui::Button(("Pause##Sound" + to_string(i)).c_str())) {
						audio->soundsListVector[i]->GetSound()->pause();
					}
					ImGui::SameLine();
					if (ImGui::Button(("Stop##Sound" + to_string(i)).c_str())) {
						audio->soundsListVector[i]->GetSound()->stop();
					}
				}
				
				ImGui::SameLine();
				if (ImGui::Button(("Change##Sound" + to_string(i)).c_str())) {
					engine->resourcesModule->SetResourcesWindowOpen(Resource::soundResource, true);
					soundPos = i;
				}
				ImGui::SameLine();
				if (ImGui::Button(("Remove##Sound" + to_string(i)).c_str())) {
					audio->soundsListVector[i]->GetSound()->stop();
					audio->RemoveSound(i);
				}
			}
		}

		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("Add sound")) {
			engine->resourcesModule->SetResourcesWindowOpen(Resource::soundResource, true);
			soundPos = -1;
		}

		ImGui::Spacing();
		ImGui::Spacing();

		if (audio->musicListVector.empty()) {
			ImGui::Text("No music added.");
		}
		else {
			for (int i = 0; i < audio->musicListVector.size(); i++) {
				ImGui::Text("Music %d:", i);
				ImGui::SameLine();
				ImGui::Text("%s", audio->musicListVector[i]->GetName().c_str());

				float actualVolume = audio->musicListVector[i]->GetMusic()->getVolume();
				ImGui::Text("Volume");
				ImGui::SameLine(70);
				if (ImGui::DragFloat(("##MusicVolume" + to_string(i)).c_str(), &actualVolume, 0.25f, 0, 100)) {
					audio->musicListVector[i]->GetMusic()->setVolume(actualVolume);
				}
				float actualPitch = audio->musicListVector[i]->GetMusic()->getPitch();
				ImGui::Text("Pitch");
				ImGui::SameLine(70);
				if (ImGui::DragFloat(("##MusicPitch" + to_string(i)).c_str(), &actualPitch)) {
					audio->musicListVector[i]->GetMusic()->setPitch(actualPitch);
				}
				bool actualLoop = audio->musicListVector[i]->GetMusic()->getLoop();
				ImGui::Text("Loop");
				ImGui::SameLine(70);
				if (ImGui::Checkbox(("##MusicLoop" + to_string(i)).c_str(), &actualLoop)) {
					audio->musicListVector[i]->GetMusic()->setLoop(actualLoop);
				}

				if (audio->musicListVector[i]->GetMusic()->getStatus() == 0 || //Stopped
					audio->musicListVector[i]->GetMusic()->getStatus() == 1) //Paused
				{
					if (ImGui::Button(("Play##Music" + to_string(i)).c_str())) {
						for (int j = 0; j < audio->soundsListVector.size(); j++) {
							if (audio->musicListVector[j]->GetMusic()->getStatus() == 2) {
								audio->musicListVector[j]->GetMusic()->stop();
							}
						}
						audio->musicListVector[i]->GetMusic()->play();
					}
					if (audio->musicListVector[i]->GetMusic()->getStatus() == 1) {
						ImGui::SameLine();
						if (ImGui::Button(("Stop##Music" + to_string(i)).c_str())) {
							audio->musicListVector[i]->GetMusic()->stop();
						}
					}
				}
				else {
					if (ImGui::Button(("Pause##Music" + to_string(i)).c_str())) {
						audio->musicListVector[i]->GetMusic()->pause();
					}
					ImGui::SameLine();
					if (ImGui::Button(("Stop##Music" + to_string(i)).c_str())) {
						audio->musicListVector[i]->GetMusic()->stop();
					}
				}

				ImGui::SameLine();
				if (ImGui::Button(("Change##Music" + to_string(i)).c_str())) {
					engine->resourcesModule->SetResourcesWindowOpen(Resource::musicResource, true);
					musicPos = i;
				}
				ImGui::SameLine();
				if (ImGui::Button(("Remove##Music" + to_string(i)).c_str())) {
					audio->RemoveMusic(i);
				}
			}
		}

		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("Add music")) {
			engine->resourcesModule->SetResourcesWindowOpen(Resource::musicResource, true);
			musicPos = -1;
		}

		ImGui::Spacing();
	}

	if (engine->resourcesModule->IsResourcesWindowOpen(Resource::soundResource)) {
		ResourceSound* sound = (ResourceSound*)engine->resourcesModule->DrawResourcesWindow(Resource::soundResource);
		if (sound != nullptr) {
			engine->resourcesModule->SetResourcesWindowOpen(Resource::soundResource, false);
			audio->AddSound(sound, soundPos);
		}
	}

	if (engine->resourcesModule->IsResourcesWindowOpen(Resource::musicResource)) {
		ResourceMusic* music = (ResourceMusic*)engine->resourcesModule->DrawResourcesWindow(Resource::musicResource);
		if (music != nullptr) {
			engine->resourcesModule->SetResourcesWindowOpen(Resource::musicResource, false);
			audio->AddMusic(music, musicPos);
		}
	}
}

void PanelProperties::DrawScriptPanel(ComponentScript * componentScript)
{
	
	if (ImGui::CollapsingHeader(componentScript->GetScriptName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Spacing();
		bool scriptHasChanged = ScriptHasChanged(componentScript->GetScriptPath());

		if (componentScript->GetScriptFields().empty() || scriptHasChanged) {
			UpdateScript(componentScript);
			componentScript->SetScriptFields();
		}

		vector<ScriptField*> scriptFields = componentScript->GetScriptFields();
		for (int j = 0; j < scriptFields.size(); j++) {
			switch (scriptFields[j]->propertyType) {
				case ScriptField::Integer:
				{
					int i = componentScript->GetScript()->GetIntProperty(scriptFields[j]->fieldName.c_str());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::InputInt(("##" + scriptFields[j]->fieldName).c_str(), &i)) {
						componentScript->GetScript()->SetIntProperty(scriptFields[j]->fieldName.c_str(), i);
					}
				}
					break;
				case ScriptField::Float:
				{
					float f = componentScript->GetScript()->GetFloatProperty(scriptFields[j]->fieldName.c_str());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::InputFloat(("##" + scriptFields[j]->fieldName).c_str(), &f, 0.001f, 0.01f, 3)) {
						componentScript->GetScript()->SetFloatProperty(scriptFields[j]->fieldName.c_str(), f);
					}
				}
					break;
				case ScriptField::Bool:
				{
					bool b = componentScript->GetScript()->GetBoolProperty(scriptFields[j]->fieldName.c_str());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::Checkbox(("##" + scriptFields[j]->fieldName).c_str(), &b)) {
						componentScript->GetScript()->SetBoolProperty(scriptFields[j]->fieldName.c_str(), b);
					}
				}
					break;
				case ScriptField::String:
				{
					static char textToRender[256];
					string str = componentScript->GetScript()->GetStringProperty(scriptFields[j]->fieldName.c_str());
					strncpy(textToRender, str.data(), str.size());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::InputText(("##" + scriptFields[j]->fieldName).c_str(), textToRender, 256)) {
						componentScript->GetScript()->SetStringProperty(scriptFields[j]->fieldName.c_str(), string(textToRender).c_str());
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
					sf::Vector3i v3i = componentScript->GetScript()->GetVec3iProperty(scriptFields[j]->fieldName.c_str());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::DragInt3(("##" + scriptFields[j]->fieldName).c_str(), (int*)&v3i, 0.25f)) {
						componentScript->GetScript()->SetVec3iProperty(scriptFields[j]->fieldName.c_str(), v3i);
					}
				}
					break;
				case ScriptField::Vector4i:
				{
					ImVec4 v4i = componentScript->GetScript()->GetVec4iProperty(scriptFields[j]->fieldName.c_str());
					ImGui::Text(" %s", scriptFields[j]->fieldName.c_str());
					ImGui::SameLine(100);
					if (ImGui::DragFloat4(("##" + scriptFields[j]->fieldName).c_str(), (float*)&v4i, 0.25f)) {
						componentScript->GetScript()->SetVec4iProperty(scriptFields[j]->fieldName.c_str(), v4i);
					}
				}
				break;
			}
		}
		componentScript->GetScript()->UpdateScript(engine->deltaTime);
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
