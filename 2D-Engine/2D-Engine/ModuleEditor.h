#pragma once

#include "Module.h"
#include "Panel.h"
#include "PanelAssets.h"
#include "PanelDetails.h"
#include "GameObject.h"

class PanelHierarchy;

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
	PanelDetails* detailsPanel = nullptr;

public:
	vector<Panel*> editorPanels;
};

