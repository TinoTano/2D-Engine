#pragma once

#include "Panel.h"
#include "Engine.h"
#include "ModuleRenderWindow.h"

class PanelAssets : public Panel
{
public:
	PanelAssets();
	virtual ~PanelAssets();

	void DrawPanel();
	void DrawChilds();
	void CheckMouseHover();
	void Resize(float w, float h);
};

