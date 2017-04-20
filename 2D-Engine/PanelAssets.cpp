#include "PanelAssets.h"
#include "Engine.h"
#include "ModuleEditor.h"

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
	node = 0;
	fs::path path(ASSETS_FOLDER);
	DrawChilds(path);
	
	//sprintf_s(nodeName, 30, "%s##node_%i", , node++);
	/*for (list<Resource*>::iterator it = rootFolders.begin(); it != rootFolders.end(); it++) {
		DrawChilds(*it);
	}*/
}

void PanelAssets::DrawChilds(fs::path path)
{
	sprintf_s(nodeName, 30, "%s##node_%i", path.filename().string().c_str(), node++);
	uint flag = 0;

	if (fs::is_empty(path)) {
		flag |= ImGuiTreeNodeFlags_Leaf;
	}
	else {
		for (auto & p : fs::directory_iterator(path)) {
			if (fs::is_directory(p)) {
				flag = 0;
				break; //if fodler contains another folder, it can't be a leaf.
			}
			flag |= ImGuiTreeNodeFlags_Leaf;
		}
	}

	//for (list<GameObject*>::iterator it = engine->sceneManagerModule->selectedGameObjects.begin(); it != engine->sceneManagerModule->selectedGameObjects.end(); it++) {
	//	if (gameObject == *it) {
	//		flag |= ImGuiTreeNodeFlags_Selected;
	//		break;
	//	}
	//}

	if (selectedFolder == path) {
		flag |= ImGuiTreeNodeFlags_Selected;
	}

	if (ImGui::TreeNodeEx(nodeName, flag))
	{
		//CheckMouseOver(gameObject);
		if (ImGui::IsItemHoveredRect() && ImGui::IsMouseClicked(0)) {
			selectedFolder = path;
		}
		for (auto & p : fs::directory_iterator(path)) {
			if (fs::is_directory(p)) {
				DrawChilds(p);
			}
		}
		ImGui::TreePop();
	}
	else {
		//CheckMouseOver(gameObject);
		/*if (ImGui::IsItemHoveredRect()) {
			selectedFolder = path;
		}*/
	}
}

void PanelAssets::CheckMouseHover()
{
}
