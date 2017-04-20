#include "PanelAssets.h"
#include "Engine.h"
#include "ModuleEditor.h"
#include <experimental/filesystem>

PanelAssets::PanelAssets()
{
	active = true;
	panelName = "Assets";
	newPanel = false;
}


PanelAssets::~PanelAssets()
{
}

void PanelAssets::DrawPanel()
{
	std::string path = ASSETS_FOLDER;
	for (auto & p : experimental::filesystem::directory_iterator(path)) {
		cout << p << endl;
	}
	node = 0;
	//sprintf_s(nodeName, 30, "%s##node_%i", , node++);
	/*for (list<Resource*>::iterator it = rootFolders.begin(); it != rootFolders.end(); it++) {
		DrawChilds(*it);
	}*/
}

void PanelAssets::DrawChilds(Resource * resource)
{
	/*sprintf_s(nodeName, 30, "%s##node_%i", resource->name.c_str(), node++);
	uint flag = 0;

	if (resource->childs.empty()) {
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
	}*/
}

void PanelAssets::CheckMouseHover()
{
}
