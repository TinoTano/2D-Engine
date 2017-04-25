#pragma once

#include "Module.h"
#include "Panel.h"
#include "PanelAssets.h"
#include "PanelProperties.h"
#include "PanelHierarchy.h"
#include "PanelScene.h"
#include "PanelConsole.h"
#include "GameObject.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Awake();
	bool PreUpdate();
	bool Update(float deltaTime);
	bool CleanUp();

private:
	PanelHierarchy* hierarchyPanel = nullptr;
	PanelAssets* assetsPanel = nullptr;
	PanelProperties* propertiesPanel = nullptr;
	PanelScene* scenePanel = nullptr;
	PanelConsole* consolePanel = nullptr;

public:
	vector<Panel*> editorPanels;
	ImFont* font = nullptr;
};

