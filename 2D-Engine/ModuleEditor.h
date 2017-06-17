#pragma once

#include "Module.h"
#include "Panel.h"
#include "PanelAssets.h"
#include "PanelProperties.h"
#include "PanelHierarchy.h"
#include "PanelScene.h"
#include "PanelConsole.h"
#include "GameObject.h"
#include "PanelAnimator.h"

struct DragObjectData {
	string fromPanel;
	string path;
	string extension;
	string name;
	bool hasData = false;
	sf::Sprite dragSprite;
	void clearData() {
		fromPanel.clear();
		path.clear();
		extension.clear();
		name.clear();
		hasData = false;
		dragSprite = sf::Sprite();
	}
};

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Awake();
	bool PreUpdate();
	bool Update(float deltaTime);
	bool CleanUp();

	void PrintLog(string log);
	void PrintWarningLog(string log);
	void PrintErrorLog(string log);

private:
	PanelHierarchy* hierarchyPanel = nullptr;
	PanelAssets* assetsPanel = nullptr;
	PanelProperties* propertiesPanel = nullptr;
	PanelScene* scenePanel = nullptr;
	PanelConsole* consolePanel = nullptr;
	PanelAnimator* animatorPanel = nullptr;

public:
	vector<Panel*> editorPanels;
	ImFont* font = nullptr;
	DragObjectData dragData;
	GameObject* draggingGameObject = nullptr;
};

