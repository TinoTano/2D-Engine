#pragma once

#include "Panel.h"
#include "Engine.h"
#include "Resource.h"

class PanelAssets : public Panel
{
public:
	PanelAssets();
	virtual ~PanelAssets();

	void DrawPanel();
	void DrawChilds(Resource * resource);
	void CheckMouseHover();

private:
	list<Resource*> rootFolders;
	uint node = 0;
	char nodeName[30];
};

