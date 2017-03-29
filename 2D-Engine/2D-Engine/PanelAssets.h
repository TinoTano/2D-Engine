#pragma once

#include "Panel.h"
#include "Engine.h"

class PanelAssets : public Panel
{
public:
	PanelAssets();
	virtual ~PanelAssets();

	void DrawPanel();
	void DrawChilds();
	void CheckMouseHover();
};

