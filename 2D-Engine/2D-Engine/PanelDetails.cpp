#include "PanelDetails.h"
#include "Engine.h"
#include "ModuleSceneManager.h"
#include "ModuleEditor.h"
#include "Component.h"
#include "ComponentTransform.h"


PanelDetails::PanelDetails()
{
	active = true;
	panelName = "Details";
	newPanel = true;
}


PanelDetails::~PanelDetails()
{
}

void PanelDetails::DrawPanel()
{
	GameObject* selectedGameObject = nullptr;
	if (engine->sceneManagerModule->selectedGameObjects.size() == 1) {
		selectedGameObject = engine->sceneManagerModule->selectedGameObjects.front();
	}

	if (selectedGameObject != nullptr) {
		ImGui::Text("Name: %s", selectedGameObject->name.c_str());
		ImGui::SameLine();
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

			ImGui::EndPopup();
		}
	}
}

void PanelDetails::DrawComponent(Component* component)
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
		break;
	default:
		break;
	}
}

void PanelDetails::DrawTransformPanel(ComponentTransform * transform)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		sf::Vector3f position = transform->position;
		float rotation = transform->rotation;
		sf::Vector2f scale = transform->scale;

		if (ImGui::DragFloat3("Position", (float*)&position, 0.25f)) {
			transform->SetPosition(position);
		}
		if (ImGui::DragFloat("Rotation", (float*)&rotation, 0.25f)) {
			transform->SetRotation(rotation);
		}
		if (ImGui::DragFloat2("Scale", (float*)&scale, 0.25f)) {
			transform->SetScale(scale);
		}
	}
}

void PanelDetails::DrawSpriteRendererPanel(ComponentSpriteRenderer * spriteRenderer)
{
	if (ImGui::CollapsingHeader("Sprite Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
		sf::Sprite* sprite = spriteRenderer->gameObjectSprite;
		ImGui::Image(*sprite, ImVec2(100.f, 100.f), sf::Color::White, sf::Color::Transparent);

	}
}
