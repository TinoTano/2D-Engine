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
	void CheckMouseHover(GameObject* gameObject);
	void Resize(float w, float h);
	
private:
	bool showRenameWindow = false;
};

