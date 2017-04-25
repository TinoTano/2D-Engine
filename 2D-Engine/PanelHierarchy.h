#pragma once

#include "Panel.h"
#include "GameObject.h"

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy();
	virtual ~PanelHierarchy();

	void DrawPanel();
	void DrawChilds(GameObject* gameObject);
	void CheckMouseOver(GameObject* gameObject);
	
private:
	bool showRenameWindow = false;
	uint node = 0;
	char nodeName[30];
	bool showRenameError = false;
	GameObject* openGameObjectNode = nullptr;
};

