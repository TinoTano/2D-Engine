#include "PanelScene.h"
#include "ModuleSceneWindow.h"
#include "ModuleEngineWindow.h"
#include "Engine.h"
#include "ModuleSceneManager.h"
#include "ModuleEditor.h"
#include "Component.h"
#include "ModuleInput.h"


PanelScene::PanelScene()
{
	active = true;
	panelName = "Scene";
}


PanelScene::~PanelScene()
{
}

void PanelScene::DrawPanel()
{
	if (ImGui::BeginDock(panelName.c_str(),false, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
		sf::Vector2u size = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)engine->sceneWindow->window->getTexture().getNativeHandle(), size, ImVec2(0, 1), ImVec2(1, 0), sf::Color::White, sf::Color::Transparent);

		//Dragging Images/Prefabs to the scene
		if (ImGui::IsItemHoveredRect()) {
			if (engine->editorModule->dragData.hasData) {
				if (engine->editorModule->dragData.fromPanel == "Files") {
					if (ImGui::IsMouseReleased(0)) {
						GameObject* gameObject = nullptr;
						if (engine->editorModule->dragData.extension != ".prefab") {
							gameObject = engine->sceneManagerModule->CreateGameObject(nullptr);
							gameObject->name = engine->editorModule->dragData.name;
							engine->sceneManagerModule->RenameDuplicatedGameObject(gameObject);
							ComponentSpriteRenderer* spr = (ComponentSpriteRenderer*)gameObject->GetComponent(Component::SpriteRenderer);
							spr->ChangeSprite(engine->editorModule->dragData.path);
							ComponentTransform* transform = (ComponentTransform*)gameObject->GetComponent(Component::Transform);
							transform->SetPosition(sf::Vector3f(engine->editorModule->dragData.dragSprite.getPosition().x, engine->editorModule->dragData.dragSprite.getPosition().y, 0), true);
						}
						else {
							vector<GameObject*> createdObjects;
							bool justIncrease = false;
							if (engine->sceneManagerModule->gameObjectsList.empty()) {
								justIncrease = true;
							}
							Data data;
							if (data.LoadBinary(engine->editorModule->dragData.path)) {
								int gameObjectsCount = data.GetInt("GameObjectsCount");
								for (int i = 0; i < gameObjectsCount; i++) {
									GameObject* go = new GameObject();
									data.EnterSection("GameObject_" + to_string(i));
									go->Load(data, true);
									data.LeaveSection();
									engine->sceneManagerModule->gameObjectsList.push_back(go);
									engine->sceneWindow->drawableObjects.push_back(go);
									createdObjects.push_back(go);
								}
								data.ClearData();
								for (int i = 0; i < createdObjects.size(); i++) {
									engine->sceneManagerModule->RenameDuplicatedGameObject(createdObjects[i], justIncrease);
								}
							}
						}
						engine->editorModule->dragData.clearData();
					}
					else if (ImGui::IsMouseDragging()) {
						int x = engine->inputModule->GetmousePosition().x - ((ImGui::GetItemRectMin().x - ImGui::GetCursorStartPos().x) * engine->sceneWindow->zoomValue);
						int y = engine->inputModule->GetmousePosition().y - ImGui::GetItemRectMin().y;
						LOG("Itemrect: %f", ImGui::GetItemRectMin().x);
						LOG("CursorPos: %f", ImGui::GetCursorPos().x);
						LOG("CursorScreenPos: %f", ImGui::GetCursorScreenPos().x);
						LOG("CursorStartPos: %f", ImGui::GetCursorStartPos().x);
						engine->editorModule->dragData.dragSprite.setPosition(
						engine->sceneWindow->window->mapPixelToCoords({ x,y }).x, engine->sceneWindow->window->mapPixelToCoords({ x,y }).y);
						engine->sceneWindow->window->draw(engine->editorModule->dragData.dragSprite);	
					}
				}
			}
		}


	}
	
	ImGui::EndDock();
}
